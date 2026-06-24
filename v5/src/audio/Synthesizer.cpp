#include "audio/Synthesizer.h"
#include <cmath>
#include <algorithm>

namespace flstudio {
namespace audio {

Synthesizer::Synthesizer() : m_nextInstrumentId(-1) {
}

Synthesizer::~Synthesizer() = default;

void Synthesizer::initDefaultInstruments() {
    // Piano - FM synthesis
    Instrument piano(0, "Piano");
    piano.type = SynthType::FM;
    piano.adsr = ADSR(0.005f, 0.3f, 0.4f, 0.5f);
    piano.filter = FilterSettings("lowpass", 5000.0f, 1.0f);
    piano.modulatorFreq = 1.5f;
    piano.modulatorGain = 200.0f;
    piano.color = "#4CAF50";
    addInstrument(piano);

    // Organ - Additive synthesis
    Instrument organ(0, "Organ");
    organ.type = SynthType::Additive;
    organ.adsr = ADSR(0.01f, 0.1f, 0.8f, 0.3f);
    organ.filter = FilterSettings("lowpass", 4000.0f, 0.5f);
    organ.harmonics = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    organ.amplitudes = {1.0f, 0.5f, 0.3f, 0.25f, 0.2f, 0.15f};
    organ.color = "#9C27B0";
    addInstrument(organ);

    // Synth - Subtractive synthesis
    Instrument synth(0, "Synth");
    synth.type = SynthType::Subtractive;
    synth.waveform = "sawtooth";
    synth.adsr = ADSR(0.05f, 0.2f, 0.6f, 0.4f);
    synth.filter = FilterSettings("lowpass", 2000.0f, 5.0f);
    synth.detunes = {5.0f};
    synth.color = "#2196F3";
    addInstrument(synth);

    // Pad
    Instrument pad(0, "Pad");
    pad.type = SynthType::Pad;
    pad.adsr = ADSR(0.5f, 0.5f, 0.7f, 1.0f);
    pad.filter = FilterSettings("lowpass", 3000.0f, 2.0f);
    pad.detunes = {-12.0f, -5.0f, 0.0f, 5.0f, 12.0f};
    pad.color = "#FF9800";
    addInstrument(pad);

    // Bass
    Instrument bass(0, "Bass");
    bass.type = SynthType::Subtractive;
    bass.waveform = "sawtooth";
    bass.adsr = ADSR(0.01f, 0.2f, 0.5f, 0.2f);
    bass.filter = FilterSettings("lowpass", 800.0f, 3.0f);
    bass.color = "#F44336";
    addInstrument(bass);
}

int Synthesizer::addInstrument(const Instrument& inst) {
    int id = ++m_nextInstrumentId;
    auto newInst = std::make_unique<Instrument>(inst);
    newInst->id = id;
    m_instruments.push_back(std::move(newInst));
    return id;
}

bool Synthesizer::removeInstrument(int id) {
    if (m_instruments.size() <= 1) {
        return false; // Keep at least one instrument
    }
    
    auto it = std::remove_if(m_instruments.begin(), m_instruments.end(),
        [id](const std::unique_ptr<Instrument>& inst) { return inst->id == id; });
    
    if (it != m_instruments.end()) {
        m_instruments.erase(it, m_instruments.end());
        return true;
    }
    return false;
}

Instrument* Synthesizer::getInstrument(int id) {
    for (auto& inst : m_instruments) {
        if (inst->id == id) {
            return inst.get();
        }
    }
    return m_instruments.empty() ? nullptr : m_instruments[0].get();
}

std::vector<Instrument*> Synthesizer::getAllInstruments() {
    std::vector<Instrument*> result;
    for (auto& inst : m_instruments) {
        result.push_back(inst.get());
    }
    return result;
}

void Synthesizer::renderNote(const Instrument& inst, float frequency, float startTime,
                             float duration, float velocity, utils::AudioBuffer& output,
                             int startSample) {
    switch (inst.type) {
        case SynthType::FM:
            renderFM(inst, frequency, duration, velocity, output, startSample);
            break;
        case SynthType::Additive:
            renderAdditive(inst, frequency, duration, velocity, output, startSample);
            break;
        case SynthType::Subtractive:
            renderSubtractive(inst, frequency, duration, velocity, output, startSample);
            break;
        case SynthType::Pad:
            renderPad(inst, frequency, duration, velocity, output, startSample);
            break;
    }
}

void Synthesizer::renderFM(const Instrument& inst, float frequency, float duration,
                           float velocity, utils::AudioBuffer& output, int startSample) {
    int sampleRate = output.getSampleRate();
    int numSamples = static_cast<int>(duration * sampleRate);
    int endSample = std::min(startSample + numSamples, output.getSampleCount());
    
    float phase = 0.0f;
    float modPhase = 0.0f;
    float modFreq = frequency * inst.modulatorFreq;
    
    for (int i = startSample; i < endSample; ++i) {
        float t = static_cast<float>(i - startSample) / sampleRate;
        float env = getADSREnvelope(inst.adsr, t, duration);
        
        // FM synthesis
        float modValue = std::sin(modPhase) * inst.modulatorGain;
        float carrierFreq = frequency + modValue;
        
        float sample = std::sin(phase) * env * velocity * 0.6f;
        
        // Apply pan
        float pan = 0.0f; // Center
        float leftGain = std::min(1.0f, 1.0f - pan);
        float rightGain = std::min(1.0f, 1.0f + pan);
        
        if (output.getChannels() >= 2) {
            output.writeSample(0, i, output.readSample(0, i) + sample * leftGain);
            output.writeSample(1, i, output.readSample(1, i) + sample * rightGain);
        } else {
            output.writeSample(0, i, output.readSample(0, i) + sample);
        }
        
        phase += 2.0f * M_PI * carrierFreq / sampleRate;
        modPhase += 2.0f * M_PI * modFreq / sampleRate;
    }
}

void Synthesizer::renderAdditive(const Instrument& inst, float frequency, float duration,
                                 float velocity, utils::AudioBuffer& output, int startSample) {
    int sampleRate = output.getSampleRate();
    int numSamples = static_cast<int>(duration * sampleRate);
    int endSample = std::min(startSample + numSamples, output.getSampleCount());
    
    std::vector<float> phases(inst.harmonics.size(), 0.0f);
    
    for (int i = startSample; i < endSample; ++i) {
        float t = static_cast<float>(i - startSample) / sampleRate;
        float env = getADSREnvelope(inst.adsr, t, duration);
        
        float sample = 0.0f;
        for (size_t h = 0; h < inst.harmonics.size(); ++h) {
            float harmonicFreq = frequency * inst.harmonics[h];
            sample += std::sin(phases[h]) * inst.amplitudes[h] * 0.2f;
            phases[h] += 2.0f * M_PI * harmonicFreq / sampleRate;
        }
        
        sample *= env * velocity * 0.5f;
        
        if (output.getChannels() >= 2) {
            output.writeSample(0, i, output.readSample(0, i) + sample * 0.5f);
            output.writeSample(1, i, output.readSample(1, i) + sample * 0.5f);
        } else {
            output.writeSample(0, i, output.readSample(0, i) + sample);
        }
    }
}

void Synthesizer::renderSubtractive(const Instrument& inst, float frequency, float duration,
                                    float velocity, utils::AudioBuffer& output, int startSample) {
    int sampleRate = output.getSampleRate();
    int numSamples = static_cast<int>(duration * sampleRate);
    int endSample = std::min(startSample + numSamples, output.getSampleCount());
    
    float phase1 = 0.0f;
    float phase2 = 0.0f;
    float detuneVal = inst.detunes.empty() ? 0.0f : inst.detunes[0];
    float detunedFreq = frequency * (1.0f + detuneVal / 1000.0f);
    float filterState = 0.0f;
    
    for (int i = startSample; i < endSample; ++i) {
        float t = static_cast<float>(i - startSample) / sampleRate;
        float env = getADSREnvelope(inst.adsr, t, duration);
        
        // Generate waveform
        float sample1, sample2;
        if (inst.waveform == "sawtooth") {
            sample1 = 2.0f * (phase1 / (2.0f * M_PI) - std::floor(phase1 / (2.0f * M_PI) + 0.5f));
            sample2 = 2.0f * (phase2 / (2.0f * M_PI) - std::floor(phase2 / (2.0f * M_PI) + 0.5f));
        } else if (inst.waveform == "square") {
            sample1 = phase1 < M_PI ? 1.0f : -1.0f;
            sample2 = phase2 < M_PI ? 1.0f : -1.0f;
        } else {
            sample1 = std::sin(phase1);
            sample2 = std::sin(phase2);
        }
        
        float sample = (sample1 + sample2) * 0.5f * env * velocity * 0.5f;
        
        // Simple lowpass filter
        float cutoff = inst.filter.frequency * (1.0f + 2.0f * env);
        float rc = 1.0f / (2.0f * M_PI * cutoff);
        float dt = 1.0f / sampleRate;
        float alpha = dt / (rc + dt);
        filterState += alpha * (sample - filterState);
        sample = filterState;
        
        if (output.getChannels() >= 2) {
            output.writeSample(0, i, output.readSample(0, i) + sample * 0.5f);
            output.writeSample(1, i, output.readSample(1, i) + sample * 0.5f);
        } else {
            output.writeSample(0, i, output.readSample(0, i) + sample);
        }
        
        phase1 += 2.0f * M_PI * frequency / sampleRate;
        phase2 += 2.0f * M_PI * detunedFreq / sampleRate;
    }
}

void Synthesizer::renderPad(const Instrument& inst, float frequency, float duration,
                            float velocity, utils::AudioBuffer& output, int startSample) {
    int sampleRate = output.getSampleRate();
    int numSamples = static_cast<int>(duration * sampleRate);
    int endSample = std::min(startSample + numSamples, output.getSampleCount());
    
    std::vector<float> phases(inst.detunes.size(), 0.0f);
    
    for (int i = startSample; i < endSample; ++i) {
        float t = static_cast<float>(i - startSample) / sampleRate;
        float env = getADSREnvelope(inst.adsr, t, duration);
        
        float sample = 0.0f;
        for (size_t d = 0; d < inst.detunes.size(); ++d) {
            float detunedFreq = frequency * std::pow(2.0f, inst.detunes[d] / 1200.0f);
            sample += std::sin(phases[d]);
            phases[d] += 2.0f * M_PI * detunedFreq / sampleRate;
        }
        
        sample /= inst.detunes.size();
        sample *= env * velocity * 0.3f;
        
        if (output.getChannels() >= 2) {
            output.writeSample(0, i, output.readSample(0, i) + sample * 0.5f);
            output.writeSample(1, i, output.readSample(1, i) + sample * 0.5f);
        } else {
            output.writeSample(0, i, output.readSample(0, i) + sample);
        }
    }
}

float Synthesizer::getADSREnvelope(const ADSR& adsr, float time, float duration) {
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

float Synthesizer::midiToFrequency(int midiNote) {
    return 440.0f * std::pow(2.0f, (midiNote - 69) / 12.0f);
}

QString Synthesizer::getNoteName(int midiNote) {
    static const char* notes[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    int octave = midiNote / 12 - 1;
    int noteIndex = midiNote % 12;
    return QString("%1%2").arg(notes[noteIndex]).arg(octave);
}

bool Synthesizer::isBlackKey(int midiNote) {
    static const int blackKeys[] = {1, 3, 6, 8, 10};
    int noteIndex = midiNote % 12;
    for (int key : blackKeys) {
        if (key == noteIndex) return true;
    }
    return false;
}

} // namespace audio
} // namespace flstudio
