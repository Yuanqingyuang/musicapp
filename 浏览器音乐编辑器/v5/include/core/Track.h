#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <vector>
#include <array>

namespace flstudio {
namespace core {

enum class TrackType {
    Piano,      // Piano roll track
    Step        // Step sequencer track
};

struct Track {
    int id;
    QString name;
    TrackType type;
    
    // Step sequencer data
    std::array<bool, 16> steps;
    QString drumSample;  // For step tracks
    
    // Mixer settings
    float volume;    // 0.0 - 1.0
    float pan;       // -1.0 (left) to 1.0 (right)
    bool mute;
    bool solo;
    
    // Instrument for piano tracks
    int instrumentId;

    Track(int trackId = 0, const QString& trackName = "Track", TrackType t = TrackType::Piano)
        : id(trackId), name(trackName), type(t),
          volume(0.7f), pan(0.0f), mute(false), solo(false),
          instrumentId(0) {
        steps.fill(false);
    }
};

} // namespace core
} // namespace flstudio

#endif // TRACK_H
