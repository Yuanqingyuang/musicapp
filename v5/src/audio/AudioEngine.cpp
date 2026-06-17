#include "audio/AudioEngine.h"
#include "audio/Scheduler.h"
#include <QAudioFormat>
#include <QAudioSink>
#include <QMediaDevices>

namespace flstudio {
namespace audio {

AudioEngine::AudioEngine(Scheduler* scheduler, QObject* parent)
    : QIODevice(parent)
    , m_scheduler(scheduler)
    , m_initialized(false)
    , m_masterVolume(0.8f) {
}

AudioEngine::~AudioEngine() {
    shutdown();
}

bool AudioEngine::initialize() {
    if (m_initialized) return true;

    QAudioFormat format;
    format.setSampleRate(44100);
    format.setChannelCount(2);
    format.setSampleFormat(QAudioFormat::Float);

    QMediaDevices* devices = new QMediaDevices(this);
    QAudioDevice defaultDevice = devices->defaultAudioOutput();

    if (!defaultDevice.isFormatSupported(format)) {
        format = defaultDevice.preferredFormat();
    }

    m_format = format;

    m_audioSink = std::make_unique<QAudioSink>(defaultDevice, format);
    m_audioSink->setBufferSize(8192);
    m_audioSink->setVolume(m_masterVolume);

    open(QIODevice::ReadOnly);
    m_audioSink->start(this);

    m_initialized = true;
    return true;
}

void AudioEngine::shutdown() {
    if (m_audioSink) {
        m_audioSink->stop();
    }
    close();
    m_initialized = false;
}

qint64 AudioEngine::readData(char* data, qint64 maxlen) {
    if (!m_scheduler) return 0;

    int bytesPerFrame = m_format.bytesPerFrame();
    int numFrames = static_cast<int>(maxlen / bytesPerFrame);
    int numChannels = m_format.channelCount();

    float* floatData = reinterpret_cast<float*>(data);

    m_scheduler->renderAudio(floatData, numFrames, numChannels);

    return numFrames * bytesPerFrame;
}

qint64 AudioEngine::writeData(const char* data, qint64 len) {
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

qint64 AudioEngine::bytesAvailable() const {
    return 4096 * m_format.bytesPerFrame();
}

void AudioEngine::setMasterVolume(float volume) {
    m_masterVolume = volume;
    if (m_audioSink) {
        m_audioSink->setVolume(volume);
    }
}

} // namespace audio
} // namespace flstudio
