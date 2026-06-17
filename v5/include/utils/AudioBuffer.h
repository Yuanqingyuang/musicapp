#ifndef AUDIO_BUFFER_H
#define AUDIO_BUFFER_H

#include <vector>
#include <cstdint>

namespace flstudio {
namespace utils {

class AudioBuffer {
public:
    AudioBuffer(int channels = 2, int sampleRate = 44100);
    ~AudioBuffer();

    void resize(int samples);
    void clear();

    // Write sample to buffer
    void writeSample(int channel, int sampleIndex, float value);
    float readSample(int channel, int sampleIndex) const;

    // Get raw data pointer
    float* getChannelData(int channel);
    const float* getChannelData(int channel) const;

    // Properties
    int getChannels() const { return m_channels; }
    int getSampleRate() const { return m_sampleRate; }
    int getSampleCount() const { return m_sampleCount; }

    // Mix another buffer into this one
    void mix(const AudioBuffer& other, float gain = 1.0f);

    // Apply gain
    void applyGain(float gain);

    // Get interleaved data for output
    std::vector<float> getInterleavedData() const;

private:
    int m_channels;
    int m_sampleRate;
    int m_sampleCount;
    std::vector<std::vector<float>> m_data;
};

} // namespace utils
} // namespace flstudio

#endif // AUDIO_BUFFER_H
