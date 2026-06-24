#include "ui/PianoRollWidget.h"
#include "ui/PianoKeyboard.h"
#include "core/Project.h"
#include "audio/Synthesizer.h"
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <algorithm>

namespace flstudio {
namespace ui {

PianoRollWidget::PianoRollWidget(core::Project* project, audio::Synthesizer* synth,
                                 QWidget* parent)
    : QWidget(parent)
    , m_project(project)
    , m_synth(synth)
    , m_snap(16)
    , m_zoom(40)
    , m_currentStep(0)
    , m_currentInstrumentId(0)
    , m_dragging(false)
    , m_resizing(false)
    , m_dragNoteIndex(-1) {

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);

    m_keyboard = new PianoKeyboard(this);
    layout->addWidget(m_keyboard);

    setMinimumHeight((END_NOTE - START_NOTE + 1) * NOTE_HEIGHT);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

PianoRollWidget::~PianoRollWidget() = default;

void PianoRollWidget::setSnap(int snap) {
    m_snap = snap;
    update();
}

void PianoRollWidget::setZoom(int zoom) {
    m_zoom = zoom;
    update();
}

void PianoRollWidget::zoomIn() {
    m_zoom = std::min(200, m_zoom + 5);
    update();
}

void PianoRollWidget::zoomOut() {
    m_zoom = std::max(10, m_zoom - 5);
    update();
}

void PianoRollWidget::setCurrentInstrument(int instrumentId) {
    m_currentInstrumentId = instrumentId;
}

void PianoRollWidget::updatePlayPosition(int step) {
    m_currentStep = step;
    update();
}

void PianoRollWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawGrid(painter);
    drawNotes(painter);
    drawPlayhead(painter);
}

void PianoRollWidget::drawGrid(QPainter& painter) {
    int width = this->width();
    int height = this->height();
    int numNotes = END_NOTE - START_NOTE + 1;

    // Background
    painter.fillRect(0, 0, width, height, QColor(34, 34, 34));

    // Vertical grid lines (steps)
    for (int step = 0; step < 64; ++step) {
        int x = step * m_zoom;
        if (x > width) break;

        if (step % 16 == 0) {
            painter.setPen(QPen(QColor(85, 85, 85), 2));
        } else if (step % 4 == 0) {
            painter.setPen(QPen(QColor(68, 68, 68), 1));
        } else {
            painter.setPen(QPen(QColor(51, 51, 51), 0.5));
        }

        painter.drawLine(x, 0, x, height);
    }

    // Horizontal grid lines (notes)
    for (int i = 0; i <= numNotes; ++i) {
        int y = i * NOTE_HEIGHT;
        painter.setPen(QPen(QColor(51, 51, 51), 0.5));
        painter.drawLine(0, y, width, y);
    }
}

void PianoRollWidget::drawNotes(QPainter& painter) {
    for (size_t i = 0; i < m_project->notes.size(); ++i) {
        const auto& note = m_project->notes[i];

        auto inst = m_synth->getInstrument(note.instrumentId);
        QColor color = inst ? QColor(inst->color) : QColor(76, 175, 80);

        int noteY = (END_NOTE - note.pitch) * NOTE_HEIGHT;
        int noteX = note.startStep * m_zoom;
        int noteWidth = note.durationSteps * m_zoom;

        // Draw note body
        painter.fillRect(noteX, noteY, noteWidth, NOTE_HEIGHT - 1, color);

        // Draw note border
        painter.setPen(color.darker(120));
        painter.drawRect(noteX, noteY, noteWidth, NOTE_HEIGHT - 1);

        // Draw note name
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 8));
        QString noteName = audio::Synthesizer::getNoteName(note.pitch);
        painter.drawText(noteX + 2, noteY + 12, noteName);
    }
}

void PianoRollWidget::drawPlayhead(QPainter& painter) {
    int x = m_currentStep * m_zoom;
    painter.setPen(QPen(QColor(255, 152, 0), 2));
    painter.drawLine(x, 0, x, height());
}

void PianoRollWidget::mousePressEvent(QMouseEvent* event) {
    int x = event->pos().x();
    int y = event->pos().y();

    core::Note* clickedNote = noteAt(x, y);

    if (clickedNote) {
        int noteX = clickedNote->startStep * m_zoom;
        int noteWidth = clickedNote->durationSteps * m_zoom;

        if (event->button() == Qt::RightButton) {
            // Remove note
            for (size_t i = 0; i < m_project->notes.size(); ++i) {
                if (&m_project->notes[i] == clickedNote) {
                    m_project->removeNote(static_cast<int>(i));
                    emit noteRemoved(static_cast<int>(i));
                    update();
                    return;
                }
            }
        }

        if (x - noteX < 8 || noteX + noteWidth - x < 8) {
            m_resizing = true;
            m_dragNoteIndex = static_cast<int>(clickedNote - &m_project->notes[0]);
            m_dragStartX = x;
            m_dragStartDuration = clickedNote->durationSteps;
        } else {
            m_dragging = true;
            m_dragNoteIndex = static_cast<int>(clickedNote - &m_project->notes[0]);
            m_dragStartX = x;
            m_dragStartY = y;
            m_dragStartStep = clickedNote->startStep;
            m_dragStartPitch = clickedNote->pitch;
        }
    } else if (event->button() == Qt::LeftButton) {
        // Create new note
        int step = x / m_zoom;
        int pitch = END_NOTE - y / NOTE_HEIGHT;

        if (pitch >= START_NOTE && pitch <= END_NOTE && step >= 0 && step < 64) {
            int snappedStep = snapStep(step);
            core::Note newNote;
            newNote.pitch = pitch;
            newNote.startStep = snappedStep;
            newNote.durationSteps = (m_snap == 16) ? 4 : (m_snap == 8) ? 2 : 1;
            newNote.trackId = 0;
            newNote.instrumentId = m_currentInstrumentId;
            newNote.velocity = 0.8f;

            m_project->addNote(newNote);
            emit noteAdded(newNote);
            update();
        }
    }
}

void PianoRollWidget::mouseMoveEvent(QMouseEvent* event) {
    int x = event->pos().x();
    int y = event->pos().y();

    if (m_dragging && m_dragNoteIndex >= 0 && m_dragNoteIndex < static_cast<int>(m_project->notes.size())) {
        auto& note = m_project->notes[m_dragNoteIndex];
        int deltaStep = (x - m_dragStartX) / m_zoom;
        int deltaPitch = (m_dragStartY - y) / NOTE_HEIGHT;

        note.startStep = std::max(0, std::min(63, snapStep(m_dragStartStep + deltaStep)));
        note.pitch = std::max(START_NOTE, std::min(END_NOTE, m_dragStartPitch + deltaPitch));
        emit noteModified(m_dragNoteIndex, note);
        update();
    } else if (m_resizing && m_dragNoteIndex >= 0 && m_dragNoteIndex < static_cast<int>(m_project->notes.size())) {
        auto& note = m_project->notes[m_dragNoteIndex];
        int deltaStep = (x - m_dragStartX) / m_zoom;
        note.durationSteps = std::max(1, m_dragStartDuration + deltaStep);
        emit noteModified(m_dragNoteIndex, note);
        update();
    }
}

void PianoRollWidget::mouseReleaseEvent(QMouseEvent* event) {
    Q_UNUSED(event)
    m_dragging = false;
    m_resizing = false;
    m_dragNoteIndex = -1;
}

void PianoRollWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    update();
}

void PianoRollWidget::wheelEvent(QWheelEvent* event) {
    int delta = event->angleDelta().y() / 120;
    m_zoom = std::max(20, std::min(100, m_zoom + delta * 5));
    update();
}

core::Note* PianoRollWidget::noteAt(int x, int y) {
    int step = x / m_zoom;
    int pitch = END_NOTE - y / NOTE_HEIGHT;

    for (auto& note : m_project->notes) {
        if (note.startStep <= step && note.startStep + note.durationSteps > step &&
            note.pitch == pitch) {
            return &note;
        }
    }
    return nullptr;
}

int PianoRollWidget::snapStep(int step) {
    int snapValue = 64 / m_snap;
    return (step + snapValue / 2) / snapValue * snapValue;
}

int PianoRollWidget::pitchAt(int y) {
    return END_NOTE - y / NOTE_HEIGHT;
}

int PianoRollWidget::stepAt(int x) {
    return x / m_zoom;
}

} // namespace ui
} // namespace flstudio
