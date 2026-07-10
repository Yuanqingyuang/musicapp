#include "ui/StepSequencerWidget.h"
#include "core/Project.h"
#include <QPainter>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

namespace flstudio {
namespace ui {

StepSequencerWidget::StepSequencerWidget(core::Project* project, QWidget* parent)
    : QWidget(parent)
    , m_project(project)
    , m_currentStep(0) {

    setupUI();
}

StepSequencerWidget::~StepSequencerWidget() = default;

void StepSequencerWidget::setupUI() {
    QGridLayout* grid = new QGridLayout(this);
    grid->setSpacing(BUTTON_SPACING);
    grid->setContentsMargins(16, 16, 16, 16);

    int row = 0;
    for (const auto& track : m_project->tracks) {
        if (track->type != core::TrackType::Step) continue;

        // Track name label
        QLabel* nameLabel = new QLabel(track->name);
        nameLabel->setStyleSheet("color: #cccccc; font-weight: bold; font-size: 14px;");
        nameLabel->setFixedWidth(80);
        grid->addWidget(nameLabel, row, 0);

        // Step buttons
        std::vector<QWidget*> buttons;
        for (int step = 0; step < NUM_STEPS; ++step) {
            QPushButton* btn = new QPushButton();
            btn->setFixedSize(BUTTON_SIZE, BUTTON_SIZE);
            btn->setProperty("trackId", track->id);
            btn->setProperty("stepIndex", step);
            btn->setCheckable(true);
            btn->setChecked(track->steps[step]);

            // Style based on beat position
            QString baseStyle = "QPushButton {"
                "border: 1px solid #444;"
                "border-radius: 4px;"
                "background: #333;"
                "}"
                "QPushButton:hover {"
                "background: #444;"
                "}"
                "QPushButton:checked {"
                "background: #4CAF50;"
                "border-color: #66BB6A;"
                "}"
                "QPushButton:checked:hover {"
                "background: #66BB6A;"
                "}";

            btn->setStyleSheet(baseStyle);

            connect(btn, &QPushButton::clicked, this, &StepSequencerWidget::onStepButtonClicked);

            grid->addWidget(btn, row, step + 1);
            buttons.push_back(btn);
        }

        m_stepButtons.push_back(buttons);
        row++;
    }

    // Add stretch at the bottom
    grid->setRowStretch(row, 1);
}

void StepSequencerWidget::createStepButtons() {
    // Clear existing
    m_stepButtons.clear();

    QLayout* oldLayout = layout();
    if (oldLayout) {
        QLayoutItem* item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    setupUI();
}

void StepSequencerWidget::updateStep(int step) {
    m_currentStep = step % NUM_STEPS;
    updateButtonStyles();
}

void StepSequencerWidget::refresh() {
    createStepButtons();
}

void StepSequencerWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);
}

void StepSequencerWidget::onStepButtonClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int trackId = btn->property("trackId").toInt();
    int stepIndex = btn->property("stepIndex").toInt();
    bool active = btn->isChecked();

    auto track = m_project->getTrack(trackId);
    if (track) {
        track->steps[stepIndex] = active;
    }

    emit stepToggled(trackId, stepIndex, active);
}

void StepSequencerWidget::updateButtonStyles() {
    for (size_t trackIdx = 0; trackIdx < m_stepButtons.size(); ++trackIdx) {
        for (size_t stepIdx = 0; stepIdx < m_stepButtons[trackIdx].size(); ++stepIdx) {
            QPushButton* btn = qobject_cast<QPushButton*>(m_stepButtons[trackIdx][stepIdx]);
            if (!btn) continue;

            bool isCurrentStep = (static_cast<int>(stepIdx) == m_currentStep);
            bool isChecked = btn->isChecked();

            QString style;
            if (isChecked && isCurrentStep) {
                style = "QPushButton {"
                    "border: 1px solid #FFD54F;"
                    "border-radius: 4px;"
                    "background: #FFC107;"
                    "box-shadow: 0 0 12px rgba(255, 193, 7, 0.7);"
                    "}"
                    "QPushButton:hover {"
                    "background: #FFD54F;"
                    "}";
            } else if (isCurrentStep) {
                style = "QPushButton {"
                    "border: 1px solid #FFB74D;"
                    "border-radius: 4px;"
                    "background: #FF9800;"
                    "}"
                    "QPushButton:hover {"
                    "background: #FFB74D;"
                    "}";
            } else if (isChecked) {
                style = "QPushButton {"
                    "border: 1px solid #66BB6A;"
                    "border-radius: 4px;"
                    "background: #4CAF50;"
                    "box-shadow: 0 0 8px rgba(76, 175, 80, 0.5);"
                    "}"
                    "QPushButton:hover {"
                    "background: #66BB6A;"
                    "}";
            } else {
                style = "QPushButton {"
                    "border: 1px solid #444;"
                    "border-radius: 4px;"
                    "background: #333;"
                    "}"
                    "QPushButton:hover {"
                    "background: #444;"
                    "}";
            }

            btn->setStyleSheet(style);
        }
    }
}

} // namespace ui
} // namespace flstudio
