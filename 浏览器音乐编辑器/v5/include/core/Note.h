#ifndef NOTE_H
#define NOTE_H

#include <cstdint>

namespace flstudio {
namespace core {

struct Note {
    int pitch;           // MIDI note number (48-84 for C3-B5)
    int startStep;       // Start position in 16th note steps
    int durationSteps;   // Duration in 16th note steps
    int trackId;         // Associated track ID
    int instrumentId;    // Instrument ID
    float velocity;      // Note velocity (0.0 - 1.0)

    Note()
        : pitch(60), startStep(0), durationSteps(4),
          trackId(0), instrumentId(0), velocity(0.8f) {}

    Note(int p, int start, int dur, int track = 0, int inst = 0, float vel = 0.8f)
        : pitch(p), startStep(start), durationSteps(dur),
          trackId(track), instrumentId(inst), velocity(vel) {}
};

} // namespace core
} // namespace flstudio

#endif // NOTE_H
