#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "core/Project.h"
#include "audio/Synthesizer.h"
#include "audio/DrumSample.h"
#include "utils/AudioBuffer.h"
#include <QObject>
#include <QTimer>
#include <atomic>

namespace flstudio {
namespace audio {

class Scheduler : public QObject {
    Q_OBJECT

public:
    explicit Scheduler(core::Project* project, Synthesizer* synth,
                       DrumSample* drums, QObject* parent = nullptr);
    ~Scheduler();

    // Transport control
    void play();
    void stop();
    void pause();
    bool isPlaying() const { return m_isPlaying; }

    // Position
    int getCurrentStep() const { return m_currentStep; }
    void setCurrentStep(int step);

    // Settings
    void setBPM(int bpm);
    int getBPM() const { return m_bpm; }

    // Render audio for playback
    void renderAudio(float* outputBuffer, int numFrames, int numChannels);

    // Render offline for export
    void renderOffline(utils::AudioBuffer& output);

signals:
    void stepChanged(int step);
    void positionChanged(int bar, int beat, int subBeat);

private slots:
    void onTimer();

private:
    core::Project* m_project;
    Synthesizer* m_synth;
    DrumSample* m_drums;

    std::atomic<bool> m_isPlaying;
    std::atomic<bool> m_isPaused;
    std::atomic<int> m_currentStep;
    int m_bpm;
    double m_sampleRate;
    int m_samplesPerStep;
    int m_sampleCounter;

    QTimer* m_timer;

    // Calculate samples per step based on BPM
    void updateSamplesPerStep();

    // Render one step
    void renderStep(int step, utils::AudioBuffer& output, int startSample);
};

} // namespace audio
} // namespace flstudio

#endif // SCHEDULER_H
