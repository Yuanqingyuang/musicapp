#include "ui/MixerWidget.h"
#include "core/Project.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QFrame>

namespace flstudio {
namespace ui {

MixerWidget::MixerWidget(core::Project* project, QWidget* parent)
    : QWidget(parent)
    , m_project(project) {

    setupUI();
}

MixerWidget::~MixerWidget() = default;

void MixerWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    // Title
    QLabel* title = new QLabel("混音器", this);
    title->setStyleSheet("color: #fff; font-weight: bold; font-size: 14px;");
    mainLayout->addWidget(title);

    createTrackControls();

    // Master output section
    QFrame* masterFrame = new QFrame(this);
    masterFrame->setStyleSheet(
        "QFrame {"
        "background: #2d2d2d;"
        "border: 1px solid #444;"
        "border-radius: 8px;"
        "padding: 12px;"
        "}"
    );
    QVBoxLayout* masterLayout = new QVBoxLayout(masterFrame);

    QLabel* masterLabel = new QLabel("主输出", masterFrame);
    masterLabel->setStyleSheet("color: #fff; font-weight: bold;");
    masterLayout->addWidget(masterLabel);

    QHBoxLayout* vuLayout = new QHBoxLayout();
    vuLayout->setSpacing(8);
    vuLayout->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < 2; ++i) {
        QFrame* vuMeter = new QFrame(masterFrame);
        vuMeter->setFixedSize(24, 80);
        vuMeter->setStyleSheet(
            "QFrame {"
            "background: #1a1a1a;"
            "border: 1px solid #444;"
            "border-radius: 4px;"
            "}"
        );
        vuLayout->addWidget(vuMeter);
    }

    masterLayout->addLayout(vuLayout);
    mainLayout->addWidget(masterFrame);

    mainLayout->addStretch();
}

void MixerWidget::createTrackControls() {
    // Clear existing controls
    m_volumeSliders.clear();
    m_panSliders.clear();
    m_muteButtons.clear();
    m_soloButtons.clear();

    for (const auto& track : m_project->tracks) {
        QFrame* trackFrame = new QFrame(this);
        trackFrame->setStyleSheet(
            "QFrame {"
            "background: #252525;"
            "border: 1px solid #333;"
            "border-radius: 8px;"
            "padding: 12px;"
            "}"
        );

        QVBoxLayout* trackLayout = new QVBoxLayout(trackFrame);
        trackLayout->setSpacing(8);

        // Track header
        QHBoxLayout* headerLayout = new QHBoxLayout();
        QLabel* nameLabel = new QLabel(track->name, trackFrame);
        nameLabel->setStyleSheet("color: #ccc; font-weight: bold; font-size: 13px;");
        headerLayout->addWidget(nameLabel);

        // Mute/Solo buttons
        QPushButton* muteBtn = new QPushButton("M", trackFrame);
        muteBtn->setFixedSize(24, 24);
        muteBtn->setCheckable(true);
        muteBtn->setChecked(track->mute);
        muteBtn->setProperty("trackId", track->id);
        muteBtn->setStyleSheet(
            "QPushButton {"
            "background: #555;"
            "color: #ccc;"
            "border: none;"
            "border-radius: 4px;"
            "font-size: 12px;"
            "}"
            "QPushButton:checked {"
            "background: #f44336;"
            "color: #fff;"
            "}"
        );
        connect(muteBtn, &QPushButton::clicked, this, &MixerWidget::onMuteClicked);
        headerLayout->addWidget(muteBtn);
        m_muteButtons.push_back(muteBtn);

        QPushButton* soloBtn = new QPushButton("S", trackFrame);
        soloBtn->setFixedSize(24, 24);
        soloBtn->setCheckable(true);
        soloBtn->setChecked(track->solo);
        soloBtn->setProperty("trackId", track->id);
        soloBtn->setStyleSheet(
            "QPushButton {"
            "background: #555;"
            "color: #ccc;"
            "border: none;"
            "border-radius: 4px;"
            "font-size: 12px;"
            "}"
            "QPushButton:checked {"
            "background: #FF9800;"
            "color: #fff;"
            "}"
        );
        connect(soloBtn, &QPushButton::clicked, this, &MixerWidget::onSoloClicked);
        headerLayout->addWidget(soloBtn);
        m_soloButtons.push_back(soloBtn);

        trackLayout->addLayout(headerLayout);

        // Volume slider
        QHBoxLayout* volLayout = new QHBoxLayout();
        QLabel* volLabel = new QLabel("音量", trackFrame);
        volLabel->setStyleSheet("color: #888; font-size: 11px;");
        volLayout->addWidget(volLabel);

        QSlider* volSlider = new QSlider(Qt::Horizontal, trackFrame);
        volSlider->setRange(0, 100);
        volSlider->setValue(static_cast<int>(track->volume * 100));
        volSlider->setProperty("trackId", track->id);
        volSlider->setStyleSheet(
            "QSlider::groove:horizontal {"
            "height: 8px;"
            "background: #333;"
            "border-radius: 4px;"
            "}"
            "QSlider::handle:horizontal {"
            "width: 16px;"
            "height: 16px;"
            "background: #4CAF50;"
            "border: 2px solid #fff;"
            "border-radius: 8px;"
            "margin: -4px 0;"
            "}"
        );
        connect(volSlider, &QSlider::valueChanged, this, &MixerWidget::onVolumeChanged);
        volLayout->addWidget(volSlider);
        m_volumeSliders.push_back(volSlider);

        QLabel* volValue = new QLabel(QString("%1%").arg(static_cast<int>(track->volume * 100)), trackFrame);
        volValue->setStyleSheet("color: #888; font-size: 11px;");
        volValue->setFixedWidth(35);
        volLayout->addWidget(volValue);

        trackLayout->addLayout(volLayout);

        // Pan slider
        QHBoxLayout* panLayout = new QHBoxLayout();
        QLabel* panLabel = new QLabel("声像", trackFrame);
        panLabel->setStyleSheet("color: #888; font-size: 11px;");
        panLayout->addWidget(panLabel);

        QSlider* panSlider = new QSlider(Qt::Horizontal, trackFrame);
        panSlider->setRange(-100, 100);
        panSlider->setValue(static_cast<int>(track->pan * 100));
        panSlider->setProperty("trackId", track->id);
        panSlider->setStyleSheet(
            "QSlider::groove:horizontal {"
            "height: 8px;"
            "background: #333;"
            "border-radius: 4px;"
            "}"
            "QSlider::handle:horizontal {"
            "width: 16px;"
            "height: 16px;"
            "background: #4CAF50;"
            "border: 2px solid #fff;"
            "border-radius: 8px;"
            "margin: -4px 0;"
            "}"
        );
        connect(panSlider, &QSlider::valueChanged, this, &MixerWidget::onPanChanged);
        panLayout->addWidget(panSlider);
        m_panSliders.push_back(panSlider);

        trackLayout->addLayout(panLayout);

        // Add to main layout
        QVBoxLayout* mainLayout = qobject_cast<QVBoxLayout*>(layout());
        if (mainLayout) {
            mainLayout->insertWidget(mainLayout->count() - 2, trackFrame);
        }
    }
}

void MixerWidget::refresh() {
    // Clear and recreate
    QLayout* oldLayout = layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }
        delete oldLayout;
    }

    setupUI();
}

void MixerWidget::updateTrackUI(int trackIndex) {
    if (trackIndex < 0 || trackIndex >= static_cast<int>(m_project->tracks.size())) {
        return;
    }

    auto track = m_project->tracks[trackIndex];

    if (trackIndex < static_cast<int>(m_volumeSliders.size())) {
        m_volumeSliders[trackIndex]->setValue(static_cast<int>(track->volume * 100));
    }
    if (trackIndex < static_cast<int>(m_panSliders.size())) {
        m_panSliders[trackIndex]->setValue(static_cast<int>(track->pan * 100));
    }
    if (trackIndex < static_cast<int>(m_muteButtons.size())) {
        m_muteButtons[trackIndex]->setChecked(track->mute);
    }
    if (trackIndex < static_cast<int>(m_soloButtons.size())) {
        m_soloButtons[trackIndex]->setChecked(track->solo);
    }
}

void MixerWidget::onVolumeChanged(int value) {
    QSlider* slider = qobject_cast<QSlider*>(sender());
    if (!slider) return;

    int trackId = slider->property("trackId").toInt();
    float volume = value / 100.0f;

    auto track = m_project->getTrack(trackId);
    if (track) {
        track->volume = volume;
    }

    emit volumeChanged(trackId, volume);
}

void MixerWidget::onPanChanged(int value) {
    QSlider* slider = qobject_cast<QSlider*>(sender());
    if (!slider) return;

    int trackId = slider->property("trackId").toInt();
    float pan = value / 100.0f;

    auto track = m_project->getTrack(trackId);
    if (track) {
        track->pan = pan;
    }

    emit panChanged(trackId, pan);
}

void MixerWidget::onMuteClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int trackId = btn->property("trackId").toInt();
    bool mute = btn->isChecked();

    auto track = m_project->getTrack(trackId);
    if (track) {
        track->mute = mute;
    }

    emit muteChanged(trackId, mute);
}

void MixerWidget::onSoloClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int trackId = btn->property("trackId").toInt();

    if (btn->isChecked()) {
        // Uncheck all other solo buttons
        for (auto* soloBtn : m_soloButtons) {
            if (soloBtn != btn) {
                soloBtn->setChecked(false);
                int otherId = soloBtn->property("trackId").toInt();
                auto otherTrack = m_project->getTrack(otherId);
                if (otherTrack) otherTrack->solo = false;
            }
        }
    }

    bool solo = btn->isChecked();
    auto track = m_project->getTrack(trackId);
    if (track) {
        track->solo = solo;
    }

    emit soloChanged(trackId, solo);
}

} // namespace ui
} // namespace flstudio
