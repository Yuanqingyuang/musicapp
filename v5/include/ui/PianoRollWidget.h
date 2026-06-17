#ifndef PIANO_ROLL_WIDGET_H
#define PIANO_ROLL_WIDGET_H

#include <QWidget>
#include <vector>
#include <memory>

namespace flstudio {
namespace core {
    class Project;
    struct Note;
}
namespace audio {
    class Synthesizer;
}
}

namespace flstudio {
namespace ui {

class PianoKeyboard;

class PianoRollWidget : public QWidget {
    Q_OBJECT

public:
    explicit PianoRollWidget(core::Project* project, audio::Synthesizer* synth,
                            QWidget* parent = nullptr);
    ~PianoRollWidget();

    void setSnap(int snap);
    void setZoom(int zoom);
    void setCurrentInstrument(int instrumentId);
    void updatePlayPosition(int step);

signals:
    void noteAdded(const core::Note& note);
    void noteRemoved(int index);
    void noteModified(int index, const core::Note& note);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void drawGrid(QPainter& painter);
    void drawNotes(QPainter& painter);
    void drawPlayhead(QPainter& painter);

    core::Note* noteAt(int x, int y);
    int snapStep(int step);
    int pitchAt(int y);
    int stepAt(int x);

    core::Project* m_project;
    audio::Synthesizer* m_synth;
    PianoKeyboard* m_keyboard;

    int m_snap;
    int m_zoom;
    int m_currentStep;
    int m_currentInstrumentId;

    // Mouse interaction
    bool m_dragging;
    bool m_resizing;
    int m_dragNoteIndex;
    int m_dragStartX;
    int m_dragStartY;
    int m_dragStartStep;
    int m_dragStartPitch;
    int m_dragStartDuration;

    // Constants
    static constexpr int NOTE_HEIGHT = 20;
    static constexpr int START_NOTE = 48;  // C3
    static constexpr int END_NOTE = 84;    // B5
};

} // namespace ui
} // namespace flstudio

#endif // PIANO_ROLL_WIDGET_H
