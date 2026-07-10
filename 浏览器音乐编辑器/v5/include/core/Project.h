#ifndef PROJECT_H
#define PROJECT_H

#include "core/Track.h"
#include "core/Note.h"
#include <QString>
#include <vector>
#include <memory>

namespace flstudio {
namespace core {

class Project {
public:
    Project();
    ~Project();

    // Project properties
    QString name;
    int bpm;
    bool loop;
    int totalSteps;
    int snap;  // 4, 8, or 16

    // Tracks
    std::vector<std::shared_ptr<Track>> tracks;

    // Notes (for piano roll)
    std::vector<Note> notes;

    // Project operations
    void addTrack(const QString& name, TrackType type);
    void removeTrack(int trackId);
    std::shared_ptr<Track> getTrack(int trackId);

    void addNote(const Note& note);
    void removeNote(int index);
    void clearNotes();

    // Serialization
    bool saveToFile(const QString& filePath) const;
    bool loadFromFile(const QString& filePath);
    QString toJson() const;
    bool fromJson(const QString& json);

    // Default project setup
    void createDefaultProject();
    void clear();
};

} // namespace core
} // namespace flstudio

#endif // PROJECT_H
