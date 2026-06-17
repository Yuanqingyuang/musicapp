#include "utils/AudioBuffer.h"
#include <algorithm>
#include <cstring>

namespace flstudio {
namespace utils {

AudioBuffer::AudioBuffer(int channels, int sampleRate)
    : m_channels(channels)
    , m_sampleRate(sampleRate)
    , m_sampleCount(0) {
    m_data.resize(channels);
}

AudioBuffer::~AudioBuffer() = default;

void AudioBuffer::resize(int samples) {
    m_sampleCount = samples;
    for (auto& channel : m_data) {
        channel.resize(samples);
    }
}

void AudioBuffer::clear() {
    for (auto& channel : m_data) {
        std::fill(channel.begin(), channel.end(), 0.0f);
    }
}

void AudioBuffer::writeSample(int channel, int sampleIndex, float value) {
    if (channel >= 0 && channel < m_channels && sampleIndex >= 0 && sampleIndex < m_sampleCount) {
        m_data[channel][sampleIndex] = value;
    }
}

float AudioBuffer::readSample(int channel, int sampleIndex) const {
    if (channel >= 0 && channel < m_channels && sampleIndex >= 0 && sampleIndex < m_sampleCount) {
        return m_data[channel][sampleIndex];
    }
    return 0.0f;
}

float* AudioBuffer::getChannelData(int channel) {
    if (channel >= 0 && channel < m_channels) {
        return m_data[channel].data();
    }
    return nullptr;
}

const float* AudioBuffer::getChannelData(int channel) const {
    if (channel >= 0 && channel < m_channels) {
        return m_data[channel].data();
    }
    return nullptr;
}

void AudioBuffer::mix(const AudioBuffer& other, float gain) {
    int mixSamples = std::min(m_sampleCount, other.m_sampleCount);
    int mixChannels = std::min(m_channels, other.m_channels);
    
    for (int c = 0; c < mixChannels; ++c) {
        for (int i = 0; i < mixSamples; ++i) {
            m_data[c][i] += other.m_data[c][i] * gain;
        }
    }
}

void AudioBuffer::applyGain(float gain) {
    for (auto& channel : m_data) {
        for (auto& sample : channel) {
            sample *= gain;
        }
    }
}

std::vector<float> AudioBuffer::getInterleavedData() const {
    std::vector<float> interleaved;
    interleaved.reserve(m_sampleCount * m_channels);
    
    for (int i = 0; i < m_sampleCount; ++i) {
        for (int c = 0; c < m_channels; ++c) {
            interleaved.push_back(m_data[c][i]);
        }
    }
    
    return interleaved;
}

} // namespace utils
} // namespace flstudio
