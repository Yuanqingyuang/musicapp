#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include "utils/AudioBuffer.h"
#include <QString>
#include <vector>

namespace flstudio {
namespace audio {

enum class SynthType {
    FM,          // Frequency Modulation
    Additive,    // Additive synthesis
    Subtractive, // Subtractive synthesis
    Pad          // Pad sound with multiple oscillators
};

struct ADSR {
    float attack;
    float decay;
    float sustain;
    float release;

    ADSR(float a = 0.01f, float d = 0.2f, float s = 0.5f, float r = 0.3f)
        : attack(a), decay(d), sustain(s), release(r) {}
};

struct FilterSettings {
    QString type;  // "lowpass", "highpass", "bandpass"
    float frequency;
    float q;

    FilterSettings(const QString& t = "lowpass", float f = 2000.0f, float qv = 1.0f)
        : type(t), frequency(f), q(qv) {}
};

struct Instrument {
    int id;
    QString name;
    SynthType type;
    ADSR adsr;
    FilterSettings filter;
    QString waveform;  // "sine", "square", "sawtooth", "triangle"
    float modulatorFreq;
    float modulatorGain;
    std::vector<float> harmonics;
    std::vector<float> amplitudes;
    std::vector<float> detunes;
    QString color;

    Instrument(int instrumentId = 0, const QString& instrumentName = "Default")
        : id(instrumentId), name(instrumentName), type(SynthType::Subtractive),
          waveform("sawtooth"), modulatorFreq(1.5f), modulatorGain(200.0f),
          color("#4CAF50") {
        harmonics = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
        amplitudes = {1.0f, 0.5f, 0.3f, 0.2f, 0.1f};
        detunes = {0.0f};
    }
};

class Synthesizer {
public:
    Synthesizer();
    ~Synthesizer();

    // Initialize default instruments
    void initDefaultInstruments();

    // Instrument management
    int addInstrument(const Instrument& inst);
    bool removeInstrument(int id);
    Instrument* getInstrument(int id);
    std::vector<Instrument*> getAllInstruments();

    // Synthesis methods
    void renderNote(const Instrument& inst, float frequency, float startTime,
                    float duration, float velocity, utils::AudioBuffer& output,
                    int startSample);

    // Utility
    static float midiToFrequency(int midiNote);
    static QString getNoteName(int midiNote);
    static bool isBlackKey(int midiNote);

private:
    std::vector<std::unique_ptr<Instrument>> m_instruments;
    int m_nextInstrumentId;

    // Synthesis algorithms
    void renderFM(const Instrument& inst, float frequency, float duration,
                  float velocity, utils::AudioBuffer& output, int startSample);
    void renderAdditive(const Instrument& inst, float frequency, float duration,
                        float velocity, utils::AudioBuffer& output, int startSample);
    void renderSubtractive(const Instrument& inst, float frequency, float duration,
                           float velocity, utils::AudioBuffer& output, int startSample);
    void renderPad(const Instrument& inst, float frequency, float duration,
                   float velocity, utils::AudioBuffer& output, int startSample);

    // ADSR envelope
    float getADSREnvelope(const ADSR& adsr, float time, float duration);

    // Filter simulation (simple one-pole)
    float applyFilter(float input, float& state, float cutoff, float q);
};

} // namespace audio
} // namespace flstudio

#endif // SYNTHESIZER_H
