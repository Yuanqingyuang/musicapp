#include "audio/DrumSample.h"
#include <cmath>
#include <random>
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <cstring>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace flstudio {
namespace audio {

static thread_local std::mt19937 g_rng(42);
static thread_local std::uniform_real_distribution<float> g_noiseDist(-1.0f, 1.0f);

DrumSample::DrumSample() = default;
DrumSample::~DrumSample() = default;

float DrumSample::noise() {
    return g_noiseDist(g_rng);
}

float DrumSample::clip(float x, float max) {
    if (x > max) return max;
    if (x < -max) return -max;
    return x;
}

float DrumSample::tanhDistortion(float x, float drive) {
    return std::tanh(x * drive) / std::tanh(drive);
}

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
    int sampleChannels = sample->getChannels();
    
    float leftGain = velocity * std::min(1.0f, 1.0f - pan);
    float rightGain = velocity * std::min(1.0f, 1.0f + pan);
    
    for (int i = 0; i < sampleCount && (startSample + i) < outputCount; ++i) {
        float sampleValue = sample->readSample(0, i);
        float sampleRight = sampleChannels >= 2 ? sample->readSample(1, i) : sampleValue;
        
        if (channels >= 2) {
            output.writeSample(0, startSample + i,
                output.readSample(0, startSample + i) + sampleValue * leftGain);
            output.writeSample(1, startSample + i,
                output.readSample(1, startSample + i) + sampleRight * rightGain);
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
    int sr = 44100;
    int samples = static_cast<int>(0.4f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        float freq = 60.0f * std::exp(-t * 15.0f) + 40.0f;
        float env = std::exp(-t * 6.0f);
        float sample = std::sin(2.0f * M_PI * freq * t) * env * 0.8f;
        
        if (i < sr * 0.005f) {
            sample += noise() * std::exp(-t * 300.0f) * 0.2f;
        }
        
        buffer->writeSample(0, i, clip(sample, 0.9f));
        buffer->writeSample(1, i, clip(sample, 0.9f));
    }
    
    m_samples["kick"] = std::move(buffer);
}

void DrumSample::generateSnare() {
    int sr = 44100;
    int samples = static_cast<int>(0.3f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        float noiseVal = noise();
        float tone = std::sin(2.0f * M_PI * 180.0f * t);
        float env = std::exp(-t * 12.0f);
        float sample = (noiseVal * 0.6f + tone * 0.4f) * env * 0.7f;
        
        buffer->writeSample(0, i, clip(sample, 0.9f));
        buffer->writeSample(1, i, clip(sample * 0.98f, 0.9f));
    }
    
    m_samples["snare"] = std::move(buffer);
}

void DrumSample::generateHiHat() {
    int sr = 44100;
    int samples = static_cast<int>(0.1f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        float noiseVal = noise();
        float sample = noiseVal * std::exp(-t * 30.0f) * 0.5f;
        
        buffer->writeSample(0, i, clip(sample, 0.85f));
        buffer->writeSample(1, i, clip(sample, 0.85f));
    }
    
    m_samples["hihat"] = std::move(buffer);
}

void DrumSample::generateClap() {
    int sr = 44100;
    int samples = static_cast<int>(0.4f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    float env1 = 0.0f, env2 = 0.0f, env3 = 0.0f;
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        
        if (t >= 0.0f) env1 = std::exp(-t * 12.0f);
        if (t >= 0.015f) env2 = std::exp(-(t - 0.015f) * 12.0f) * 0.7f;
        if (t >= 0.03f) env3 = std::exp(-(t - 0.03f) * 12.0f) * 0.5f;
        
        float env = env1 + env2 + env3;
        float noiseVal = noise();
        float sample = noiseVal * env * 0.6f;
        
        buffer->writeSample(0, i, clip(sample, 0.9f));
        buffer->writeSample(1, i, clip(sample * 0.95f, 0.9f));
    }
    
    m_samples["clap"] = std::move(buffer);
}

void DrumSample::generateTom() {
    int sr = 44100;
    int samples = static_cast<int>(0.4f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        float freq = 100.0f * std::exp(-t * 8.0f) + 50.0f;
        float env = std::exp(-t * 4.0f);
        float sample = std::sin(2.0f * M_PI * freq * t) * env * 0.7f;
        
        buffer->writeSample(0, i, clip(sample, 0.9f));
        buffer->writeSample(1, i, clip(sample, 0.9f));
    }
    
    m_samples["tom"] = std::move(buffer);
}

void DrumSample::generateCrash() {
    int sr = 44100;
    int samples = static_cast<int>(0.5f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        float noiseVal = noise();
        float sample = noiseVal * std::exp(-t * 3.0f) * 0.5f;
        
        buffer->writeSample(0, i, clip(sample, 0.88f));
        buffer->writeSample(1, i, clip(sample * 1.02f, 0.88f));
    }
    
    m_samples["crash"] = std::move(buffer);
}

void DrumSample::generateRide() {
    int sr = 44100;
    int samples = static_cast<int>(0.6f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        float noiseVal = noise();
        float tone = std::sin(2.0f * M_PI * 400.0f * t) * 0.3f;
        float sample = (noiseVal * 0.7f + tone) * std::exp(-t * 5.0f) * 0.4f;
        
        buffer->writeSample(0, i, clip(sample, 0.85f));
        buffer->writeSample(1, i, clip(sample * 0.98f, 0.85f));
    }
    
    m_samples["ride"] = std::move(buffer);
}

void DrumSample::generatePerc() {
    int sr = 44100;
    int samples = static_cast<int>(0.2f * sr);
    
    auto buffer = std::make_unique<utils::AudioBuffer>(2, sr);
    buffer->resize(samples);
    
    for (int i = 0; i < samples; ++i) {
        float t = static_cast<float>(i) / sr;
        float freq = 800.0f * std::exp(-t * 20.0f);
        float sample = std::sin(2.0f * M_PI * freq * t) * std::exp(-t * 10.0f) * 0.5f;
        
        buffer->writeSample(0, i, clip(sample, 0.88f));
        buffer->writeSample(1, i, clip(sample, 0.88f));
    }
    
    m_samples["perc"] = std::move(buffer);
}

bool DrumSample::loadSample(const QString& name, const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    if (data.size() < 44) {
        return false;
    }
    
    const char* raw = data.constData();
    
    if (std::memcmp(raw, "RIFF", 4) != 0 || std::memcmp(raw + 8, "WAVE", 4) != 0) {
        return false;
    }
    
    int pos = 12;
    int dataSize = 0;
    short audioFormat = 0;
    short numChannels = 0;
    int sampleRate = 0;
    short bitsPerSample = 0;
    
    while (pos < data.size() - 8) {
        char chunkId[5] = {0};
        std::memcpy(chunkId, raw + pos, 4);
        int chunkSize = *reinterpret_cast<const int*>(raw + pos + 4);
        
        if (std::memcmp(chunkId, "fmt ", 4) == 0) {
            audioFormat = *reinterpret_cast<const short*>(raw + pos + 8);
            numChannels = *reinterpret_cast<const short*>(raw + pos + 10);
            sampleRate = *reinterpret_cast<const int*>(raw + pos + 12);
            bitsPerSample = *reinterpret_cast<const short*>(raw + pos + 22);
        } else if (std::memcmp(chunkId, "data", 4) == 0) {
            dataSize = chunkSize;
            break;
        }
        
        pos += 8 + chunkSize;
        if (chunkSize % 2 != 0) pos++;
    }
    
    if (dataSize == 0 || numChannels == 0 || sampleRate == 0 || bitsPerSample == 0) {
        return false;
    }
    
    int numSamples = dataSize / (numChannels * (bitsPerSample / 8));
    
    auto audioBuffer = std::make_unique<utils::AudioBuffer>(numChannels, sampleRate);
    audioBuffer->resize(numSamples);
    
    const unsigned char* sampleData = reinterpret_cast<const unsigned char*>(raw + pos + 8);
    
    for (int i = 0; i < numSamples; ++i) {
        for (int c = 0; c < numChannels; ++c) {
            float sample = 0.0f;
            int sampleIndex = (i * numChannels + c) * (bitsPerSample / 8);
            
            if (bitsPerSample == 16) {
                short s = *reinterpret_cast<const short*>(sampleData + sampleIndex);
                sample = static_cast<float>(s) / 32768.0f;
            } else if (bitsPerSample == 8) {
                unsigned char s = sampleData[sampleIndex];
                sample = (static_cast<float>(s) - 128.0f) / 128.0f;
            } else if (bitsPerSample == 24) {
                int s = (sampleData[sampleIndex] | 
                        (sampleData[sampleIndex + 1] << 8) | 
                        (sampleData[sampleIndex + 2] << 16));
                if (s & 0x800000) s |= 0xFF000000;
                sample = static_cast<float>(s) / 8388608.0f;
            } else if (bitsPerSample == 32) {
                if (audioFormat == 3) {
                    sample = *reinterpret_cast<const float*>(sampleData + sampleIndex);
                } else {
                    int s = *reinterpret_cast<const int*>(sampleData + sampleIndex);
                    sample = static_cast<float>(s) / 2147483648.0f;
                }
            }
            
            audioBuffer->writeSample(c, i, sample);
        }
    }
    
    m_samples[name] = std::move(audioBuffer);
    return true;
}

int DrumSample::loadSamplesFromDirectory(const QString& dirPath) {
    QDir dir(dirPath);
    if (!dir.exists()) {
        return 0;
    }
    
    QStringList filters;
    filters << "*.wav" << "*.WAV";
    dir.setNameFilters(filters);
    
    int loaded = 0;
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);
    for (const QFileInfo& fileInfo : fileList) {
        QString sampleName = fileInfo.baseName().toLower();
        if (loadSample(sampleName, fileInfo.absoluteFilePath())) {
            loaded++;
        }
    }
    
    return loaded;
}

} // namespace audio
} // namespace flstudio
