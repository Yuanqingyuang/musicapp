#ifndef DRUM_SAMPLE_H
#define DRUM_SAMPLE_H

#include "utils/AudioBuffer.h"
#include <QString>
#include <QStringList>
#include <map>
#include <memory>

namespace flstudio {
namespace audio {

class DrumSample {
public:
    DrumSample();
    ~DrumSample();

    // Generate built-in drum samples
    void generateAllSamples();
    
    // Get a drum sample by name
    utils::AudioBuffer* getSample(const QString& name);
    
    // Render a drum hit to output buffer
    void renderDrum(const QString& name, utils::AudioBuffer& output,
                    int startSample, float velocity, float pan);

    // Available drum names
    QStringList getAvailableDrums() const;

    // Load external WAV sample
    bool loadSample(const QString& name, const QString& filePath);
    
    // Load all samples from a directory (WAV files)
    int loadSamplesFromDirectory(const QString& dirPath);

private:
    std::map<QString, std::unique_ptr<utils::AudioBuffer>> m_samples;

    // Sample generation
    void generateKick();
    void generateSnare();
    void generateHiHat();
    void generateClap();
    void generateTom();
    void generateCrash();
    void generateRide();
    void generatePerc();
    
    // Utility
    static float noise();
    static float clip(float x, float max = 0.95f);
    static float tanhDistortion(float x, float drive);
};

} // namespace audio
} // namespace flstudio

#endif // DRUM_SAMPLE_H
