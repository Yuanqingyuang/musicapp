#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <QObject>
#include <QAudioSink>
#include <QIODevice>
#include <QMediaDevices>
#include <memory>

namespace flstudio {
namespace audio {

class Scheduler;

class AudioEngine : public QIODevice {
    Q_OBJECT

public:
    explicit AudioEngine(Scheduler* scheduler, QObject* parent = nullptr);
    ~AudioEngine();

    bool initialize();
    void shutdown();
    bool isInitialized() const { return m_initialized; }

    // QIODevice interface
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;
    qint64 bytesAvailable() const override;

    // Volume control
    void setMasterVolume(float volume);
    float getMasterVolume() const { return m_masterVolume; }

signals:
    void audioError(const QString& error);

private:
    Scheduler* m_scheduler;
    std::unique_ptr<QAudioSink> m_audioSink;
    QAudioFormat m_format;
    bool m_initialized;
    float m_masterVolume;
    std::vector<float> m_mixBuffer;
};

} // namespace audio
} // namespace flstudio

#endif // AUDIO_ENGINE_H
