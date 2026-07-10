#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
class QSplitter;
class QStackedWidget;
class QToolBar;
QT_END_NAMESPACE

namespace flstudio {
namespace core {
    class Project;
}
namespace ui {
    class PianoRollWidget;
    class StepSequencerWidget;
    class MixerWidget;
    class TransportWidget;
    class InstrumentEditor;
}
}

namespace flstudio {
namespace ui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onNewProject();
    void onOpenProject();
    void onSaveProject();
    void onSaveProjectAs();
    void onExportAudio();
    void onAbout();
    void onShortcuts();

    void onPlay();
    void onStop();
    void onTogglePlay();
    void onBPMChanged(int bpm);
    void onLoopChanged(bool loop);

    void onSwitchToPianoRoll();
    void onSwitchToStepSequencer();

    void onStepChanged(int step);
    void onPositionChanged(int bar, int beat, int subBeat);

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupConnections();
    void updateWindowTitle();

    std::unique_ptr<core::Project> m_project;

    // UI components
    QSplitter* m_mainSplitter;
    QStackedWidget* m_editorStack;
    TransportWidget* m_transport;
    PianoRollWidget* m_pianoRoll;
    StepSequencerWidget* m_stepSequencer;
    MixerWidget* m_mixer;
    InstrumentEditor* m_instrumentEditor;

    // State
    QString m_currentFilePath;
    bool m_modified;
};

} // namespace ui
} // namespace flstudio

#endif // MAIN_WINDOW_H
