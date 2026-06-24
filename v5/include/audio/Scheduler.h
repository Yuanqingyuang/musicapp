#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "core/Project.h"
#include "audio/Synthesizer.h"
#include "audio/DrumSample.h"
#include "utils/AudioBuffer.h"
#include <QObject>
#include <QTimer>
#include <atomic>
#include <vector>

namespace flstudio {
namespace audio {

// Voice state for real-time synthesis of a single note
struct SynthVoice {
    bool active;
    const Instrument* instrument;
    float frequency;
    float velocity;
    float pan;
    
    float elapsedTime;
    float duration;
    float sampleRate;
    
    // Oscillator phases
    float phase;
    float phase2;
    float modPhase;
    std::vector<float> harmonicPhases;
    std::vector<float> detunePhases;
    
    // Filter state
    float filterState;
    
    SynthVoice() 
        : active(false), instrument(nullptr), frequency(0.0f), velocity(0.0f),
          pan(0.0f), elapsedTime(0.0f), duration(0.0f), sampleRate(44100.0f),
          phase(0.0f), phase2(0.0f), modPhase(0.0f), filterState(0.0f) {}
};

// Drum voice state for playing back drum samples
struct DrumVoice {
    bool active;
    const utils::AudioBuffer* sample;
    int position;
    float velocity;
    float pan;
    
    DrumVoice() : active(false), sample(nullptr), position(0), velocity(0.0f), pan(0.0f) {}
};

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
    bool isPaused() const { return m_isPaused; }

    // Position
    int getCurrentStep() const { return m_currentStep; }
    void setCurrentStep(int step);

    // Settings
    void setBPM(int bpm);
    int getBPM() const { return m_bpm; }
    
    void setSampleRate(double rate) { m_sampleRate = rate; updateSamplesPerStep(); }

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
    
    // Active voices for real-time playback
    static constexpr int MAX_VOICES = 64;
    std::vector<SynthVoice> m_synthVoices;
    std::vector<DrumVoice> m_drumVoices;

    QTimer* m_timer;
    int m_lastEmittedStep;

    // Calculate samples per step based on BPM
    void updateSamplesPerStep();
    
    // Trigger notes for a given step
    void triggerStepNotes(int step);
    
    // Find a free voice slot
    SynthVoice* getFreeSynthVoice();
    DrumVoice* getFreeDrumVoice();
    
    // Process one frame of audio for all active voices
    void processVoices(float* left, float* right);
    
    // Check if any track is soloed
    bool hasSoloTrack() const;
    
    // Check if a track should play (considering mute/solo)
    bool shouldPlayTrack(int trackId) const;

    // Render one step (for offline export)
    void renderStep(int step, utils::AudioBuffer& output, int startSample);
};

} // namespace audio
} // namespace flstudio

#endif // SCHEDULER_H
