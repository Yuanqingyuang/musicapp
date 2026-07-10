#ifndef STEP_SEQUENCER_WIDGET_H
#define STEP_SEQUENCER_WIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <vector>

namespace flstudio {
namespace core {
    class Project;
    struct Track;
}
}

namespace flstudio {
namespace ui {

class StepSequencerWidget : public QWidget {
    Q_OBJECT

public:
    explicit StepSequencerWidget(core::Project* project, QWidget* parent = nullptr);
    ~StepSequencerWidget();

    void updateStep(int step);
    void refresh();

signals:
    void stepToggled(int trackId, int stepIndex, bool active);

protected:
    void paintEvent(QPaintEvent* event) override;

private slots:
    void onStepButtonClicked();

private:
    void setupUI();
    void createStepButtons();
    void updateButtonStyles();

    core::Project* m_project;
    std::vector<std::vector<QWidget*>> m_stepButtons;
    int m_currentStep;

    static constexpr int NUM_STEPS = 16;
    static constexpr int BUTTON_SIZE = 32;
    static constexpr int BUTTON_SPACING = 4;
};

} // namespace ui
} // namespace flstudio

#endif // STEP_SEQUENCER_WIDGET_H
