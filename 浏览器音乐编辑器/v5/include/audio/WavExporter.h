#ifndef WAV_EXPORTER_H
#define WAV_EXPORTER_H

#include "utils/AudioBuffer.h"
#include <QString>
#include <QObject>

namespace flstudio {
namespace audio {

class WavExporter : public QObject {
    Q_OBJECT

public:
    explicit WavExporter(QObject* parent = nullptr);
    ~WavExporter();

    // Export audio buffer to WAV file
    bool exportToFile(const QString& filePath, const utils::AudioBuffer& buffer);

    // Export with progress
    bool exportToFileWithProgress(const QString& filePath, const utils::AudioBuffer& buffer);

signals:
    void progress(int percent);
    void finished(bool success);
    void error(const QString& message);

private:
    // WAV file header structure
    struct WavHeader {
        char riff[4];           // "RIFF"
        uint32_t fileSize;      // File size - 8
        char wave[4];           // "WAVE"
        char fmt[4];            // "fmt "
        uint32_t fmtSize;       // 16
        uint16_t format;        // 1 = PCM
        uint16_t channels;      // Number of channels
        uint32_t sampleRate;    // Sample rate
        uint32_t byteRate;      // SampleRate * Channels * BitsPerSample / 8
        uint16_t blockAlign;    // Channels * BitsPerSample / 8
        uint16_t bitsPerSample; // 16
        char data[4];           // "data"
        uint32_t dataSize;      // Data size
    };

    void writeHeader(QDataStream& stream, const utils::AudioBuffer& buffer);
};

} // namespace audio
} // namespace flstudio

#endif // WAV_EXPORTER_H
