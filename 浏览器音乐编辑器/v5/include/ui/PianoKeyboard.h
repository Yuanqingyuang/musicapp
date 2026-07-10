#ifndef PIANO_KEYBOARD_H
#define PIANO_KEYBOARD_H

#include <QWidget>

namespace flstudio {
namespace ui {

class PianoKeyboard : public QWidget {
    Q_OBJECT

public:
    explicit PianoKeyboard(QWidget* parent = nullptr);
    ~PianoKeyboard();

    void setActiveNotes(const std::vector<int>& notes);
    void clearActiveNotes();

signals:
    void notePressed(int midiNote);
    void noteReleased(int midiNote);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void drawKeys(QPainter& painter);
    int noteAt(int y);
    bool isBlackKey(int midiNote);
    int getWhiteKeyIndex(int midiNote);

    std::vector<int> m_activeNotes;
    int m_pressedNote;

    static constexpr int START_NOTE = 48;  // C3
    static constexpr int END_NOTE = 84;    // B5
    static constexpr int KEY_HEIGHT = 20;
};

} // namespace ui
} // namespace flstudio

#endif // PIANO_KEYBOARD_H
