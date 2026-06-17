#include "audio/Scheduler.h"
#include <cmath>

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
    , m_sampleCounter(0) {
    
    m_timer = new QTimer(this);
    m_timer->setTimerType(Qt::PreciseTimer);
    connect(m_timer, &QTimer::timeout, this, &Scheduler::onTimer);
    
    updateSamplesPerStep();
}

Scheduler::~Scheduler() = default;

void Scheduler::play() {
    if (!m_isPlaying) {
        m_isPlaying = true;
        m_isPaused = false;
        m_sampleCounter = 0;
        m_timer->start(10); // 10ms timer for UI updates
    }
}

void Scheduler::stop() {
    m_isPlaying = false;
    m_isPaused = false;
    m_timer->stop();
    m_currentStep = 0;
    emit stepChanged(0);
    emit positionChanged(1, 1, 0);
}

void Scheduler::pause() {
    if (m_isPlaying) {
        m_isPaused = !m_isPaused;
    }
}

void Scheduler::setCurrentStep(int step) {
    m_currentStep = step;
    emit stepChanged(step);
}

void Scheduler::setBPM(int bpm) {
    m_bpm = bpm;
    updateSamplesPerStep();
}

void Scheduler::renderAudio(float* outputBuffer, int numFrames, int numChannels) {
    if (!m_isPlaying || m_isPaused) {
        // Fill with silence
        for (int i = 0; i < numFrames * numChannels; ++i) {
            outputBuffer[i] = 0.0f;
        }
        return;
    }
    
    // Create temporary buffer for mixing
    utils::AudioBuffer tempBuffer(numChannels, static_cast<int>(m_sampleRate));
    tempBuffer.resize(numFrames);
    tempBuffer.clear();
    
    // Render audio for each frame
    for (int frame = 0; frame < numFrames; ++frame) {
        // Check if we need to advance to next step
        if (m_sampleCounter >= m_samplesPerStep) {
            m_sampleCounter = 0;
            m_currentStep++;
            
            if (m_currentStep >= m_project->totalSteps) {
                if (m_project->loop) {
                    m_currentStep = 0;
                } else {
                    m_isPlaying = false;
                    emit stepChanged(0);
                    break;
                }
            }
            
            emit stepChanged(m_currentStep);
            
            int bar = m_currentStep / 16 + 1;
            int beat = (m_currentStep % 16) / 4 + 1;
            int subBeat = m_currentStep % 4;
            emit positionChanged(bar, beat, subBeat);
        }
        
        m_sampleCounter++;
    }
    
    // Render all active notes and steps
    renderCurrentStep(tempBuffer);
    
    // Copy to output buffer
    for (int frame = 0; frame < numFrames; ++frame) {
        for (int ch = 0; ch < numChannels; ++ch) {
            outputBuffer[frame * numChannels + ch] = tempBuffer.readSample(ch, frame);
        }
    }
}

void Scheduler::renderOffline(utils::AudioBuffer& output) {
    float stepDuration = 60.0f / (m_bpm * 4.0f);
    int samplesPerStep = static_cast<int>(stepDuration * output.getSampleRate());
    
    for (int step = 0; step < m_project->totalSteps; ++step) {
        int startSample = step * samplesPerStep;
        renderStep(step, output, startSample);
    }
}

void Scheduler::onTimer() {
    // Timer is used for UI updates, audio is rendered in callback
}

void Scheduler::updateSamplesPerStep() {
    float stepDuration = 60.0f / (m_bpm * 4.0f);
    m_samplesPerStep = static_cast<int>(stepDuration * m_sampleRate);
}

void Scheduler::renderCurrentStep(utils::AudioBuffer& output) {
    // Render step sequencer tracks
    for (const auto& track : m_project->tracks) {
        if (track->type != core::TrackType::Step) continue;
        if (track->mute) continue;
        
        // Check solo
        bool hasSolo = false;
        for (const auto& t : m_project->tracks) {
            if (t->solo) { hasSolo = true; break; }
        }
        if (hasSolo && !track->solo) continue;
        
        int stepIndex = m_currentStep % 16;
        if (track->steps[stepIndex]) {
            m_drums->renderDrum(track->drumSample, output, 0,
                               track->volume, track->pan);
        }
    }
    
    // Render piano roll notes
    for (const auto& note : m_project->notes) {
        if (note.startStep != m_currentStep) continue;
        
        auto track = m_project->getTrack(note.trackId);
        if (!track || track->mute) continue;
        
        // Check solo
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
                           output, 0);
    }
}

void Scheduler::renderStep(int step, utils::AudioBuffer& output, int startSample) {
    // Render step sequencer tracks
    for (const auto& track : m_project->tracks) {
        if (track->type != core::TrackType::Step) continue;
        if (track->mute) continue;
        
        bool hasSolo = false;
        for (const auto& t : m_project->tracks) {
            if (t->solo) { hasSolo = true; break; }
        }
        if (hasSolo && !track->solo) continue;
        
        int stepIndex = step % 16;
        if (track->steps[stepIndex]) {
            m_drums->renderDrum(track->drumSample, output, startSample,
                               track->volume, track->pan);
        }
    }
    
    // Render piano roll notes
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
