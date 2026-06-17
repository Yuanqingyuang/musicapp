#include "ui/PianoKeyboard.h"
#include <QPainter>
#include <QMouseEvent>

namespace flstudio {
namespace ui {

PianoKeyboard::PianoKeyboard(QWidget* parent)
    : QWidget(parent)
    , m_pressedNote(-1) {
    setFixedWidth(80);
    setMinimumHeight((END_NOTE - START_NOTE + 1) * KEY_HEIGHT);
}

PianoKeyboard::~PianoKeyboard() = default;

void PianoKeyboard::setActiveNotes(const std::vector<int>& notes) {
    m_activeNotes = notes;
    update();
}

void PianoKeyboard::clearActiveNotes() {
    m_activeNotes.clear();
    update();
}

void PianoKeyboard::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawKeys(painter);
}

void PianoKeyboard::drawKeys(QPainter& painter) {
    int numNotes = END_NOTE - START_NOTE + 1;
    int height = numNotes * KEY_HEIGHT;

    // Draw white keys first
    for (int note = START_NOTE; note <= END_NOTE; ++note) {
        if (isBlackKey(note)) continue;

        int index = getWhiteKeyIndex(note);
        int y = height - (index + 1) * KEY_HEIGHT;

        bool isActive = std::find(m_activeNotes.begin(), m_activeNotes.end(), note) != m_activeNotes.end();
        bool isPressed = (m_pressedNote == note);

        if (isPressed) {
            painter.setBrush(QColor(76, 175, 80));
        } else if (isActive) {
            painter.setBrush(QColor(100, 200, 100));
        } else {
            painter.setBrush(QColor(51, 51, 51));
        }

        painter.setPen(QColor(60, 60, 60));
        painter.drawRect(0, y, width(), KEY_HEIGHT);

        // Draw note name
        if (!isBlackKey(note)) {
            painter.setPen(QColor(136, 136, 136));
            painter.setFont(QFont("Arial", 8));
            QString noteName = QString("%1%2")
                .arg(QChar("CCDDEFFGGAAB"[note % 12]))
                .arg(note / 12 - 1);
            painter.drawText(5, y + KEY_HEIGHT - 5, noteName);
        }
    }

    // Draw black keys on top
    for (int note = START_NOTE; note <= END_NOTE; ++note) {
        if (!isBlackKey(note)) continue;

        int whiteIndex = getWhiteKeyIndex(note);
        int y = height - whiteIndex * KEY_HEIGHT - KEY_HEIGHT / 2;

        bool isActive = std::find(m_activeNotes.begin(), m_activeNotes.end(), note) != m_activeNotes.end();
        bool isPressed = (m_pressedNote == note);

        if (isPressed) {
            painter.setBrush(QColor(76, 175, 80));
        } else if (isActive) {
            painter.setBrush(QColor(100, 200, 100));
        } else {
            painter.setBrush(QColor(26, 26, 26));
        }

        painter.setPen(QColor(40, 40, 40));
        painter.drawRect(width() * 0.4, y, width() * 0.6, KEY_HEIGHT);
    }
}

void PianoKeyboard::mousePressEvent(QMouseEvent* event) {
    int note = noteAt(event->pos().y());
    if (note >= START_NOTE && note <= END_NOTE) {
        m_pressedNote = note;
        emit notePressed(note);
        update();
    }
}

void PianoKeyboard::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event)
    if (m_pressedNote >= 0) {
        emit noteReleased(m_pressedNote);
        m_pressedNote = -1;
        update();
    }
}

int PianoKeyboard::noteAt(int y) {
    int numNotes = END_NOTE - START_NOTE + 1;
    int height = numNotes * KEY_HEIGHT;
    int index = (height - y) / KEY_HEIGHT;

    // Simple mapping - this is approximate
    return START_NOTE + index;
}

bool PianoKeyboard::isBlackKey(int midiNote) {
    static const int blackKeys[] = {1, 3, 6, 8, 10};
    int noteIndex = midiNote % 12;
    for (int key : blackKeys) {
        if (key == noteIndex) return true;
    }
    return false;
}

int PianoKeyboard::getWhiteKeyIndex(int midiNote) {
    int octave = midiNote / 12;
    int noteInOctave = midiNote % 12;
    static const int whiteKeyIndices[] = {0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6};
    return octave * 7 + whiteKeyIndices[noteInOctave];
}

} // namespace ui
} // namespace flstudio
