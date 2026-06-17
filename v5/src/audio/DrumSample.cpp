#include "audio/DrumSample.h"
#include <cmath>
#include <random>

namespace flstudio {
namespace audio {

DrumSample::DrumSample() = default;
DrumSample::~DrumSample() = default;

void DrumSample::generateAllSamples() {
    generateKick();
    generateSnare();
    generateHiHat();
    generateClap();
    generateTom();
    generateCrash();
    generateRide();
    generatePerc();
}

utils::AudioBuffer* DrumSample::getSample(const QString& name) {
    auto it = m_samples.find(name);
    if (it != m_samples.end()) {
        return it->second.get();
    }
    return nullptr;
}

void DrumSample::renderDrum(const QString& name, utils::AudioBuffer& output,
                            int startSample, float velocity, float pan) {
    utils::AudioBuffer* sample = getSample(name);
    if (!sample) return;
    
    int sampleCount = sample->getSampleCount();
    int outputCount = output.getSampleCount();
    int channels = output.getChannels();
    
    float leftGain = velocity * std::min(1.0f, 1.0f - pan);
    float rightGain = velocity * std::min(1.0f, 1.0f + pan);
    
    for (int i = 0; i < sampleCount && (startSample + i) < outputCount; ++i) {
        float sampleValue = sample->readSample(0, i);
        
        if (channels >= 2) {
            output.writeSample(0, startSample + i,
                output.readSample(0, startSample + i) + sampleValue * leftGain);
            output.writeSample(1, startSample + i,
                output.readSample(1, startSample + i) + sampleValue * rightGain);
        } else {
            output.writeSample(0, startSample + i,
                output.readSample(0, startSample + i) + sampleValue * velocity);
        }
    }
}

QStringList DrumSample::getAvailableDrums() const {
    QStringList drums;
    for (const auto& pair : m_samples) {
        drums.append(pair.first);
    }
    return drums;
}

void DrumSample::generateKick() {
    int sampleRate = 44100;
    float duration = 0.5f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float freq = 60.0f * std::exp(-t * 20.0f);
        float env = std::exp(-t * 8.0f);
        float sample = std::sin(2.0f * M_PI * freq * t) * env * 0.9f;
        
        // Add click at start
        if (i < sampleRate * 0.01f) {
            sample += (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f) * std::exp(-t * 500.0f) * 0.3f;
        }
        
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["kick"] = std::move(buffer);
}

void DrumSample::generateSnare() {
    int sampleRate = 44100;
    float duration = 0.3f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float noise = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float tone = std::sin(2.0f * M_PI * 180.0f * t);
        float env = std::exp(-t * 15.0f);
        float sample = (noise * 0.7f + tone * 0.3f) * env * 0.7f;
        
        if (t > 0.05f) {
            sample *= std::exp(-(t - 0.05f) * 30.0f);
        }
        
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["snare"] = std::move(buffer);
}

void DrumSample::generateHiHat() {
    int sampleRate = 44100;
    float duration = 0.1f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float noise = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float filtered = noise * (1.0f - std::exp(-t * 100.0f));
        float sample = filtered * std::exp(-t * 40.0f) * 0.5f;
        
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["hihat"] = std::move(buffer);
}

void DrumSample::generateClap() {
    int sampleRate = 44100;
    float duration = 0.3f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float noise = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float env = 0.0f;
        
        for (int j = 0; j < 3; ++j) {
            float clickTime = j * 0.015f;
            if (t >= clickTime) {
                env += std::exp(-(t - clickTime) * 25.0f) * 0.5f;
            }
        }
        
        float sample = noise * env * 0.6f;
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["clap"] = std::move(buffer);
}

void DrumSample::generateTom() {
    int sampleRate = 44100;
    float duration = 0.4f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float freq = 100.0f * std::exp(-t * 10.0f);
        float env = std::exp(-t * 6.0f);
        float sample = std::sin(2.0f * M_PI * freq * t) * env * 0.7f;
        
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["tom"] = std::move(buffer);
}

void DrumSample::generateCrash() {
    int sampleRate = 44100;
    float duration = 1.0f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float noise = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float sample = noise * std::exp(-t * 3.0f) * 0.5f;
        
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["crash"] = std::move(buffer);
}

void DrumSample::generateRide() {
    int sampleRate = 44100;
    float duration = 0.5f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float noise = static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f;
        float tone = std::sin(2.0f * M_PI * 400.0f * t) * 0.3f;
        float sample = (noise * 0.7f + tone) * std::exp(-t * 5.0f) * 0.4f;
        
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["ride"] = std::move(buffer);
}

void DrumSample::generatePerc() {
    int sampleRate = 44100;
    float duration = 0.2f;
    int samples = static_cast<int>(duration * sampleRate);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(1, sampleRate);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float freq = 800.0f * std::exp(-t * 20.0f);
        float sample = std::sin(2.0f * M_PI * freq * t) * std::exp(-t * 15.0f) * 0.5f;
        
        buffer->writeSample(0, i, sample);
    }
    
    m_samples["perc"] = std::move(buffer);
}

} // namespace audio
} // namespace flstudio
