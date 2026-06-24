#include "core/Project.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>

namespace flstudio {
namespace core {

Project::Project()
    : name("Untitled Project")
    , bpm(120)
    , loop(true)
    , totalSteps(64)
    , snap(16) {
}

Project::~Project() = default;

void Project::addTrack(const QString& trackName, TrackType type) {
    int id = tracks.size();
    auto track = std::make_shared<Track>(id, trackName, type);
    
    if (type == TrackType::Step) {
        int stepTrackCount = 0;
        for (const auto& t : tracks) {
            if (t->type == TrackType::Step) stepTrackCount++;
        }
        
        switch (stepTrackCount) {
            case 0: track->drumSample = "kick"; break;
            case 1: track->drumSample = "snare"; break;
            case 2: track->drumSample = "hihat"; break;
            case 3: track->drumSample = "clap"; break;
            case 4: track->drumSample = "tom"; break;
            case 5: track->drumSample = "crash"; break;
            case 6: track->drumSample = "ride"; break;
            case 7: track->drumSample = "perc"; break;
            default: track->drumSample = "kick"; break;
        }
    }
    
    tracks.push_back(track);
}

void Project::removeTrack(int trackId) {
    tracks.erase(
        std::remove_if(tracks.begin(), tracks.end(),
            [trackId](const std::shared_ptr<Track>& t) { return t->id == trackId; }),
        tracks.end());
}

std::shared_ptr<Track> Project::getTrack(int trackId) {
    for (auto& track : tracks) {
        if (track->id == trackId) {
            return track;
        }
    }
    return nullptr;
}

void Project::addNote(const Note& note) {
    notes.push_back(note);
}

void Project::removeNote(int index) {
    if (index >= 0 && index < static_cast<int>(notes.size())) {
        notes.erase(notes.begin() + index);
    }
}

void Project::clearNotes() {
    notes.clear();
}

bool Project::saveToFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    stream << toJson();
    file.close();
    return true;
}

bool Project::loadFromFile(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    
    QTextStream stream(&file);
    QString json = stream.readAll();
    file.close();
    
    return fromJson(json);
}

QString Project::toJson() const {
    QJsonObject root;
    root["name"] = name;
    root["bpm"] = bpm;
    root["loop"] = loop;
    root["totalSteps"] = totalSteps;
    root["snap"] = snap;
    
    // Save tracks
    QJsonArray tracksArray;
    for (const auto& track : tracks) {
        QJsonObject trackObj;
        trackObj["id"] = track->id;
        trackObj["name"] = track->name;
        trackObj["type"] = static_cast<int>(track->type);
        trackObj["volume"] = track->volume;
        trackObj["pan"] = track->pan;
        trackObj["mute"] = track->mute;
        trackObj["solo"] = track->solo;
        trackObj["instrumentId"] = track->instrumentId;
        trackObj["drumSample"] = track->drumSample;
        
        QJsonArray stepsArray;
        for (bool step : track->steps) {
            stepsArray.append(step);
        }
        trackObj["steps"] = stepsArray;
        
        tracksArray.append(trackObj);
    }
    root["tracks"] = tracksArray;
    
    // Save notes
    QJsonArray notesArray;
    for (const auto& note : notes) {
        QJsonObject noteObj;
        noteObj["pitch"] = note.pitch;
        noteObj["startStep"] = note.startStep;
        noteObj["durationSteps"] = note.durationSteps;
        noteObj["trackId"] = note.trackId;
        noteObj["instrumentId"] = note.instrumentId;
        noteObj["velocity"] = note.velocity;
        notesArray.append(noteObj);
    }
    root["notes"] = notesArray;
    
    QJsonDocument doc(root);
    return QString(doc.toJson(QJsonDocument::Indented));
}

bool Project::fromJson(const QString& json) {
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }
    
    QJsonObject root = doc.object();
    name = root["name"].toString("Untitled Project");
    bpm = root["bpm"].toInt(120);
    loop = root["loop"].toBool(true);
    totalSteps = root["totalSteps"].toInt(64);
    snap = root["snap"].toInt(16);
    
    // Load tracks
    tracks.clear();
    QJsonArray tracksArray = root["tracks"].toArray();
    for (const auto& trackValue : tracksArray) {
        QJsonObject trackObj = trackValue.toObject();
        auto track = std::make_shared<Track>();
        track->id = trackObj["id"].toInt();
        track->name = trackObj["name"].toString();
        track->type = static_cast<TrackType>(trackObj["type"].toInt());
        track->volume = static_cast<float>(trackObj["volume"].toDouble());
        track->pan = static_cast<float>(trackObj["pan"].toDouble());
        track->mute = trackObj["mute"].toBool();
        track->solo = trackObj["solo"].toBool();
        track->instrumentId = trackObj["instrumentId"].toInt();
        track->drumSample = trackObj["drumSample"].toString();
        
        QJsonArray stepsArray = trackObj["steps"].toArray();
        for (int i = 0; i < 16 && i < stepsArray.size(); ++i) {
            track->steps[i] = stepsArray[i].toBool();
        }
        
        tracks.push_back(track);
    }
    
    // Load notes
    notes.clear();
    QJsonArray notesArray = root["notes"].toArray();
    for (const auto& noteValue : notesArray) {
        QJsonObject noteObj = noteValue.toObject();
        Note note;
        note.pitch = noteObj["pitch"].toInt();
        note.startStep = noteObj["startStep"].toInt();
        note.durationSteps = noteObj["durationSteps"].toInt();
        note.trackId = noteObj["trackId"].toInt();
        note.instrumentId = noteObj["instrumentId"].toInt();
        note.velocity = static_cast<float>(noteObj["velocity"].toDouble());
        notes.push_back(note);
    }
    
    return true;
}

void Project::createDefaultProject() {
    clear();
    name = "New Project";
    bpm = 120;
    loop = true;
    totalSteps = 64;
    snap = 16;
    
    // Add default tracks
    addTrack("Melody", TrackType::Piano);
    addTrack("Kick", TrackType::Step);
    addTrack("Snare", TrackType::Step);
    addTrack("Hi-Hat", TrackType::Step);
}

void Project::clear() {
    tracks.clear();
    notes.clear();
}

} // namespace core
} // namespace flstudio
