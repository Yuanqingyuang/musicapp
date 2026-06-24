#include "audio/Scheduler.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace flstudio {
namespace audio {

Scheduler::Scheduler(core::Project* project, Synthesizer* synth,
                     DrumSample* drums, QObject* parent)
    : QObject(parent)
    , m_project(project)
    , m_synth(synth)
    , m_drums(drums)
    , m_isPlaying(false)
    , m_isPaused(false)
    , m_currentStep(0)
    , m_bpm(120)
    , m_sampleRate(44100.0)
    , m_samplesPerStep(0)
    , m_sampleCounter(0)
    , m_lastEmittedStep(-1)
{
    m_synthVoices.resize(MAX_VOICES);
    m_drumVoices.resize(MAX_VOICES);
    
    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Scheduler::onTimer);
    
    updateSamplesPerStep();
}

Scheduler::~Scheduler() = default;

void Scheduler::play() {
    if (m_isPaused && m_isPlaying) {
        m_isPaused = false;
        m_timer->start(10);
        return;
    }
    
    if (!m_isPlaying) {
        m_isPlaying = true;
        m_isPaused = false;
        m_sampleCounter = 0;
        m_lastEmittedStep = -1;
        
        for (auto& v : m_synthVoices) v.active = false;
        for (auto& v : m_drumVoices) v.active = false;
        
        m_timer->start(10);
    }
}

void Scheduler::stop() {
    m_isPlaying = false;
    m_isPaused = false;
    m_timer->stop();
    m_currentStep = 0;
    m_sampleCounter = 0;
    m_lastEmittedStep = -1;
    
    for (auto& v : m_synthVoices) v.active = false;
    for (auto& v : m_drumVoices) v.active = false;
    
    emit stepChanged(0);
    emit positionChanged(1, 1, 0);
}

void Scheduler::pause() {
    if (m_isPlaying) {
        m_isPaused = !m_isPaused;
        if (m_isPaused) {
            m_timer->stop();
        } else {
            m_timer->start(10);
        }
    }
}

void Scheduler::setCurrentStep(int step) {
    m_currentStep = step;
    m_sampleCounter = 0;
    emit stepChanged(step);
}

void Scheduler::setBPM(int bpm) {
    m_bpm = bpm;
    updateSamplesPerStep();
}

SynthVoice* Scheduler::getFreeSynthVoice() {
    for (auto& v : m_synthVoices) {
        if (!v.active) return &v;
    }
    return nullptr;
}

DrumVoice* Scheduler::getFreeDrumVoice() {
    for (auto& v : m_drumVoices) {
        if (!v.active) return &v;
    }
    return nullptr;
}

bool Scheduler::hasSoloTrack() const {
    for (const auto& t : m_project->tracks) {
        if (t->solo) return true;
    }
    return false;
}

bool Scheduler::shouldPlayTrack(int trackId) const {
    auto track = m_project->getTrack(trackId);
    if (!track) return false;
    if (track->mute) return false;
    
    bool hasSolo = hasSoloTrack();
    if (hasSolo && !track->solo) return false;
    return true;
}

void Scheduler::triggerStepNotes(int step) {
    int stepIndex = step % 16;
    
    // Trigger step sequencer drums
    for (const auto& track : m_project->tracks) {
        if (track->type != core::TrackType::Step) continue;
        if (!shouldPlayTrack(track->id)) continue;
        
        if (track->steps[stepIndex]) {
            auto* sample = m_drums->getSample(track->drumSample);
            if (sample) {
                auto* voice = getFreeDrumVoice();
                if (voice) {
                    voice->active = true;
                    voice->sample = sample;
                    voice->position = 0;
                    voice->velocity = track->volume;
                    voice->pan = track->pan;
                }
            }
        }
    }
    
    // Trigger piano roll notes
    for (const auto& note : m_project->notes) {
        if (note.startStep != step) continue;
        if (!shouldPlayTrack(note.trackId)) continue;
        
        auto inst = m_synth->getInstrument(note.instrumentId);
        if (!inst) continue;
        
        auto* voice = getFreeSynthVoice();
        if (voice) {
            voice->active = true;
            voice->instrument = inst;
            voice->frequency = Synthesizer::midiToFrequency(note.pitch);
            voice->velocity = note.velocity;
            voice->pan = 0.0f;
            
            auto track = m_project->getTrack(note.trackId);
            if (track) {
                voice->velocity *= track->volume;
                voice->pan = track->pan;
            }
            
            voice->elapsedTime = 0.0f;
            voice->duration = note.durationSteps * (60.0f / (m_bpm * 4.0f));
            voice->sampleRate = static_cast<float>(m_sampleRate);
            
            voice->phase = 0.0f;
            voice->phase2 = 0.0f;
            voice->modPhase = 0.0f;
            voice->filterState = 0.0f;
            
            if (inst->type == SynthType::Additive) {
                voice->harmonicPhases.assign(inst->harmonics.size(), 0.0f);
            }
            if (inst->type == SynthType::Pad) {
                voice->detunePhases.assign(inst->detunes.size(), 0.0f);
            }
        }
    }
}

float getADSR(const ADSR& adsr, float time, float duration) {
    if (time < adsr.attack) {
        return time / adsr.attack;
    } else if (time < adsr.attack + adsr.decay) {
        float decayProgress = (time - adsr.attack) / adsr.decay;
        return 1.0f - (1.0f - adsr.sustain) * decayProgress;
    } else if (time < duration - adsr.release) {
        return adsr.sustain;
    } else if (time < duration) {
        float releaseProgress = (time - (duration - adsr.release)) / adsr.release;
        return adsr.sustain * (1.0f - releaseProgress);
    }
    return 0.0f;
}

void Scheduler::processVoices(float* left, float* right) {
    *left = 0.0f;
    *right = 0.0f;
    
    float dt = 1.0f / static_cast<float>(m_sampleRate);
    
    // Process synth voices
    for (auto& voice : m_synthVoices) {
        if (!voice.active || !voice.instrument) continue;
        
        const Instrument& inst = *voice.instrument;
        float t = voice.elapsedTime;
        float env = getADSR(inst.adsr, t, voice.duration);
        
        float sample = 0.0f;
        
        switch (inst.type) {
            case SynthType::FM: {
                float modFreq = voice.frequency * inst.modulatorFreq;
                float modValue = std::sin(voice.modPhase) * inst.modulatorGain;
                float carrierFreq = voice.frequency + modValue;
                sample = std::sin(voice.phase) * env * 0.6f;
                
                voice.phase += 2.0f * static_cast<float>(M_PI) * carrierFreq * dt;
                voice.modPhase += 2.0f * static_cast<float>(M_PI) * modFreq * dt;
                break;
            }
            case SynthType::Additive: {
                for (size_t h = 0; h < inst.harmonics.size() && h < voice.harmonicPhases.size(); ++h) {
                    float harmonicFreq = voice.frequency * inst.harmonics[h];
                    sample += std::sin(voice.harmonicPhases[h]) * inst.amplitudes[h] * 0.2f;
                    voice.harmonicPhases[h] += 2.0f * static_cast<float>(M_PI) * harmonicFreq * dt;
                }
                sample *= env * 0.5f;
                break;
            }
            case SynthType::Subtractive: {
                float detuneVal = inst.detunes.empty() ? 0.0f : inst.detunes[0];
                float detunedFreq = voice.frequency * (1.0f + detuneVal / 1000.0f);
                
                float s1, s2;
                if (inst.waveform == "sawtooth") {
                    s1 = 2.0f * (voice.phase / (2.0f * static_cast<float>(M_PI)) - std::floor(voice.phase / (2.0f * static_cast<float>(M_PI)) + 0.5f));
                    s2 = 2.0f * (voice.phase2 / (2.0f * static_cast<float>(M_PI)) - std::floor(voice.phase2 / (2.0f * static_cast<float>(M_PI)) + 0.5f));
                } else if (inst.waveform == "square") {
                    s1 = voice.phase < static_cast<float>(M_PI) ? 1.0f : -1.0f;
                    s2 = voice.phase2 < static_cast<float>(M_PI) ? 1.0f : -1.0f;
                } else {
                    s1 = std::sin(voice.phase);
                    s2 = std::sin(voice.phase2);
                }
                
                sample = (s1 + s2) * 0.5f * env * 0.5f;
                
                float cutoff = inst.filter.frequency * (1.0f + 2.0f * env);
                float rc = 1.0f / (2.0f * static_cast<float>(M_PI) * cutoff);
                float alpha = dt / (rc + dt);
                voice.filterState += alpha * (sample - voice.filterState);
                sample = voice.filterState;
                
                voice.phase += 2.0f * static_cast<float>(M_PI) * voice.frequency * dt;
                voice.phase2 += 2.0f * static_cast<float>(M_PI) * detunedFreq * dt;
                break;
            }
            case SynthType::Pad: {
                for (size_t d = 0; d < inst.detunes.size() && d < voice.detunePhases.size(); ++d) {
                    float detunedFreq = voice.frequency * std::pow(2.0f, inst.detunes[d] / 1200.0f);
                    sample += std::sin(voice.detunePhases[d]);
                    voice.detunePhases[d] += 2.0f * static_cast<float>(M_PI) * detunedFreq * dt;
                }
                sample /= static_cast<float>(inst.detunes.size());
                sample *= env * 0.3f;
                break;
            }
        }
        
        float leftGain = std::min(1.0f, 1.0f - voice.pan);
        float rightGain = std::min(1.0f, 1.0f + voice.pan);
        
        *left += sample * voice.velocity * leftGain;
        *right += sample * voice.velocity * rightGain;
        
        voice.elapsedTime += dt;
        if (voice.elapsedTime >= voice.duration) {
            voice.active = false;
        }
    }
    
    // Process drum voices
    for (auto& voice : m_drumVoices) {
        if (!voice.active || !voice.sample) continue;
        
        if (voice.position >= voice.sample->getSampleCount()) {
            voice.active = false;
            continue;
        }
        
        float s = voice.sample->readSample(0, voice.position);
        float leftGain = std::min(1.0f, 1.0f - voice.pan);
        float rightGain = std::min(1.0f, 1.0f + voice.pan);
        
        *left += s * voice.velocity * leftGain;
        *right += s * voice.velocity * rightGain;
        
        voice.position++;
    }
}

void Scheduler::renderAudio(float* outputBuffer, int numFrames, int numChannels) {
    if (!m_isPlaying || m_isPaused) {
        for (int i = 0; i < numFrames * numChannels; ++i) {
            outputBuffer[i] = 0.0f;
        }
        return;
    }
    
    for (int frame = 0; frame < numFrames; ++frame) {
        if (m_sampleCounter >= m_samplesPerStep) {
            m_sampleCounter = 0;
            int step = static_cast<int>(m_currentStep) + 1;
            
            if (step >= m_project->totalSteps) {
                if (m_project->loop) {
                    step = 0;
                } else {
                    m_isPlaying = false;
                    m_currentStep = 0;
                    emit stepChanged(0);
                    for (int f = frame; f < numFrames; ++f) {
                        for (int ch = 0; ch < numChannels; ++ch) {
                            outputBuffer[f * numChannels + ch] = 0.0f;
                        }
                    }
                    return;
                }
            }
            
            m_currentStep = step;
            triggerStepNotes(step);
        }
        
        float left, right;
        processVoices(&left, &right);
        
        if (numChannels >= 2) {
            outputBuffer[frame * numChannels] = left;
            outputBuffer[frame * numChannels + 1] = right;
        } else {
            outputBuffer[frame * numChannels] = (left + right) * 0.5f;
        }
        
        m_sampleCounter++;
    }
}

void Scheduler::onTimer() {
    int step = static_cast<int>(m_currentStep);
    if (step != m_lastEmittedStep) {
        m_lastEmittedStep = step;
        emit stepChanged(step);
        
        int bar = step / 16 + 1;
        int beat = (step % 16) / 4 + 1;
        int subBeat = step % 4;
        emit positionChanged(bar, beat, subBeat);
    }
}

void Scheduler::updateSamplesPerStep() {
    float stepDuration = 60.0f / (m_bpm * 4.0f);
    m_samplesPerStep = static_cast<int>(stepDuration * m_sampleRate);
}

void Scheduler::renderOffline(utils::AudioBuffer& output) {
    float stepDuration = 60.0f / (m_bpm * 4.0f);
    int samplesPerStep = static_cast<int>(stepDuration * output.getSampleRate());
    
    for (int step = 0; step < m_project->totalSteps; ++step) {
        int startSample = step * samplesPerStep;
        renderStep(step, output, startSample);
    }
}

void Scheduler::renderStep(int step, utils::AudioBuffer& output, int startSample) {
    int stepIndex = step % 16;
    
    for (const auto& track : m_project->tracks) {
        if (track->type != core::TrackType::Step) continue;
        if (track->mute) continue;
        
        bool hasSolo = false;
        for (const auto& t : m_project->tracks) {
            if (t->solo) { hasSolo = true; break; }
        }
        if (hasSolo && !track->solo) continue;
        
        if (track->steps[stepIndex]) {
            m_drums->renderDrum(track->drumSample, output, startSample,
                               track->volume, track->pan);
        }
    }
    
    for (const auto& note : m_project->notes) {
        if (note.startStep != step) continue;
        
        auto track = m_project->getTrack(note.trackId);
        if (!track || track->mute) continue;
        
        bool hasSolo = false;
        for (const auto& t : m_project->tracks) {
            if (t->solo) { hasSolo = true; break; }
        }
        if (hasSolo && !track->solo) continue;
        
        auto inst = m_synth->getInstrument(note.instrumentId);
        if (!inst) continue;
        
        float freq = Synthesizer::midiToFrequency(note.pitch);
        float duration = note.durationSteps * (60.0f / (m_bpm * 4.0f));
        
        m_synth->renderNote(*inst, freq, 0.0f, duration, note.velocity * track->volume,
                           output, startSample);
    }
}

} // namespace audio
} // namespace flstudio
