#include "audio/WavExporter.h"
#include <QFile>
#include <QDataStream>
#include <cmath>

namespace flstudio {
namespace audio {

WavExporter::WavExporter(QObject* parent) : QObject(parent) {
}

WavExporter::~WavExporter() = default;

bool WavExporter::exportToFile(const QString& filePath, const utils::AudioBuffer& buffer) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error("无法创建文件: " + filePath);
        return false;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    int sampleRate = buffer.getSampleRate();
    int channels = buffer.getChannels();
    int sampleCount = buffer.getSampleCount();
    int bitsPerSample = 16;
    int bytesPerSample = bitsPerSample / 8;
    int dataSize = sampleCount * channels * bytesPerSample;

    // Write WAV header
    // RIFF chunk
    stream.writeRawData("RIFF", 4);
    stream << static_cast<quint32>(36 + dataSize);
    stream.writeRawData("WAVE", 4);

    // fmt chunk
    stream.writeRawData("fmt ", 4);
    stream << static_cast<quint32>(16);           // Subchunk1Size
    stream << static_cast<quint16>(1);            // AudioFormat (PCM)
    stream << static_cast<quint16>(channels);     // NumChannels
    stream << static_cast<quint32>(sampleRate);   // SampleRate
    stream << static_cast<quint32>(sampleRate * channels * bytesPerSample); // ByteRate
    stream << static_cast<quint16>(channels * bytesPerSample); // BlockAlign
    stream << static_cast<quint16>(bitsPerSample); // BitsPerSample

    // data chunk
    stream.writeRawData("data", 4);
    stream << static_cast<quint32>(dataSize);

    // Write audio data
    for (int i = 0; i < sampleCount; ++i) {
        for (int c = 0; c < channels; ++c) {
            float sample = buffer.readSample(c, i);
            // Clamp to [-1, 1]
            sample = std::max(-1.0f, std::min(1.0f, sample));
            // Convert to 16-bit integer
            qint16 intSample = static_cast<qint16>(sample * 32767.0f);
            stream << intSample;
        }
    }

    file.close();
    emit finished(true);
    return true;
}

bool WavExporter::exportToFileWithProgress(const QString& filePath, const utils::AudioBuffer& buffer) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        emit error("无法创建文件: " + filePath);
        return false;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    int sampleRate = buffer.getSampleRate();
    int channels = buffer.getChannels();
    int sampleCount = buffer.getSampleCount();
    int bitsPerSample = 16;
    int bytesPerSample = bitsPerSample / 8;
    int dataSize = sampleCount * channels * bytesPerSample;

    // Write WAV header
    stream.writeRawData("RIFF", 4);
    stream << static_cast<quint32>(36 + dataSize);
    stream.writeRawData("WAVE", 4);
    stream.writeRawData("fmt ", 4);
    stream << static_cast<quint32>(16);
    stream << static_cast<quint16>(1);
    stream << static_cast<quint16>(channels);
    stream << static_cast<quint32>(sampleRate);
    stream << static_cast<quint32>(sampleRate * channels * bytesPerSample);
    stream << static_cast<quint16>(channels * bytesPerSample);
    stream << static_cast<quint16>(bitsPerSample);
    stream.writeRawData("data", 4);
    stream << static_cast<quint32>(dataSize);

    // Write audio data with progress
    int progressInterval = sampleCount / 100;
    if (progressInterval < 1) progressInterval = 1;

    for (int i = 0; i < sampleCount; ++i) {
        for (int c = 0; c < channels; ++c) {
            float sample = buffer.readSample(c, i);
            sample = std::max(-1.0f, std::min(1.0f, sample));
            qint16 intSample = static_cast<qint16>(sample * 32767.0f);
            stream << intSample;
        }

        if (i % progressInterval == 0) {
            emit progress(i * 100 / sampleCount);
        }
    }

    file.close();
    emit progress(100);
    emit finished(true);
    return true;
}

} // namespace audio
} // namespace flstudio
