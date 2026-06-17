#include "ui/TransportWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include <QSlider>

namespace flstudio {
namespace ui {

TransportWidget::TransportWidget(QWidget* parent)
    : QWidget(parent)
    , m_isPlaying(false) {

    setupUI();
}

TransportWidget::~TransportWidget() = default;

void TransportWidget::setupUI() {
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(16);
    layout->setContentsMargins(16, 8, 16, 8);

    // Play/Stop buttons
    m_playButton = new QPushButton("▶", this);
    m_playButton->setFixedSize(48, 36);
    m_playButton->setStyleSheet(
        "QPushButton {"
        "background: #4CAF50;"
        "border: 1px solid #66BB6A;"
        "border-radius: 4px;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background: #66BB6A;"
        "}"
        "QPushButton:pressed {"
        "background: #388E3C;"
        "}"
    );
    connect(m_playButton, &QPushButton::clicked, this, &TransportWidget::onPlayClicked);
    layout->addWidget(m_playButton);

    m_stopButton = new QPushButton("■", this);
    m_stopButton->setFixedSize(48, 36);
    m_stopButton->setStyleSheet(
        "QPushButton {"
        "background: #f44336;"
        "border: 1px solid #ef5350;"
        "border-radius: 4px;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background: #ef5350;"
        "}"
        "QPushButton:pressed {"
        "background: #d32f2f;"
        "}"
    );
    connect(m_stopButton, &QPushButton::clicked, this, &TransportWidget::onStopClicked);
    layout->addWidget(m_stopButton);

    // Time display
    m_timeLabel = new QLabel("1:1:00", this);
    m_timeLabel->setStyleSheet(
        "color: #81C784;"
        "font-family: 'Courier New', monospace;"
        "font-size: 18px;"
        "font-weight: bold;"
        "min-width: 80px;"
    );
    m_timeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_timeLabel);

    // BPM control
    QVBoxLayout* bpmLayout = new QVBoxLayout();
    bpmLayout->setSpacing(2);

    QLabel* bpmLabel = new QLabel("BPM", this);
    bpmLabel->setStyleSheet("color: #888; font-size: 12px;");
    bpmLabel->setAlignment(Qt::AlignCenter);
    bpmLayout->addWidget(bpmLabel);

    m_bpmSpinBox = new QSpinBox(this);
    m_bpmSpinBox->setRange(60, 200);
    m_bpmSpinBox->setValue(120);
    m_bpmSpinBox->setFixedWidth(70);
    m_bpmSpinBox->setStyleSheet(
        "QSpinBox {"
        "background: #2a2a2a;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #e0e0e0;"
        "padding: 4px;"
        "font-size: 14px;"
        "}"
        "QSpinBox::up-button, QSpinBox::down-button {"
        "width: 16px;"
        "}"
    );
    connect(m_bpmSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &TransportWidget::onBPMChanged);
    bpmLayout->addWidget(m_bpmSpinBox);

    layout->addLayout(bpmLayout);

    // Loop checkbox
    QVBoxLayout* loopLayout = new QVBoxLayout();
    loopLayout->setSpacing(2);

    QLabel* loopLabel = new QLabel("循环", this);
    loopLabel->setStyleSheet("color: #888; font-size: 12px;");
    loopLabel->setAlignment(Qt::AlignCenter);
    loopLayout->addWidget(loopLabel);

    m_loopCheckBox = new QCheckBox(this);
    m_loopCheckBox->setChecked(true);
    m_loopCheckBox->setStyleSheet(
        "QCheckBox::indicator {"
        "width: 18px;"
        "height: 18px;"
        "}"
        "QCheckBox::indicator:checked {"
        "background: #4CAF50;"
        "border: 1px solid #66BB6A;"
        "}"
    );
    m_loopCheckBox->setAlignment(Qt::AlignCenter);
    connect(m_loopCheckBox, &QCheckBox::stateChanged,
            this, &TransportWidget::onLoopChanged);
    loopLayout->addWidget(m_loopCheckBox);

    layout->addLayout(loopLayout);

    layout->addStretch();
}

void TransportWidget::setPlaying(bool playing) {
    m_isPlaying = playing;
    if (playing) {
        m_playButton->setText("⏸");
        m_playButton->setStyleSheet(
            "QPushButton {"
            "background: #FF9800;"
            "border: 1px solid #FFB74D;"
            "border-radius: 4px;"
            "color: white;"
            "font-size: 16px;"
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background: #FFB74D;"
            "}"
        );
    } else {
        m_playButton->setText("▶");
        m_playButton->setStyleSheet(
            "QPushButton {"
            "background: #4CAF50;"
            "border: 1px solid #66BB6A;"
            "border-radius: 4px;"
            "color: white;"
            "font-size: 16px;"
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background: #66BB6A;"
            "}"
        );
    }
}

void TransportWidget::setBPM(int bpm) {
    m_bpmSpinBox->setValue(bpm);
}

void TransportWidget::setLoop(bool loop) {
    m_loopCheckBox->setChecked(loop);
}

void TransportWidget::setPosition(int bar, int beat, int subBeat) {
    m_timeLabel->setText(QString("%1:%2:%3")
        .arg(bar)
        .arg(beat)
        .arg(subBeat, 2, 10, QChar('0')));
}

void TransportWidget::onPlayClicked() {
    emit playClicked();
}

void TransportWidget::onStopClicked() {
    emit stopClicked();
}

void TransportWidget::onBPMChanged(int value) {
    emit bpmChanged(value);
}

void TransportWidget::onLoopChanged(int state) {
    emit loopChanged(state == Qt::Checked);
}

} // namespace ui
} // namespace flstudio
