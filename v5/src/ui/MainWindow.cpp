#include "ui/MainWindow.h"
#include "core/Project.h"
#include "audio/AudioEngine.h"
#include "audio/Scheduler.h"
#include "audio/Synthesizer.h"
#include "audio/DrumSample.h"
#include "audio/WavExporter.h"
#include "ui/PianoRollWidget.h"
#include "ui/StepSequencerWidget.h"
#include "ui/MixerWidget.h"
#include "ui/TransportWidget.h"
#include "ui/InstrumentEditor.h"
#include <QMenuBar>
#include <QToolBar>
#include <QSplitter>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QKeyEvent>
#include <QCloseEvent>

namespace flstudio {
namespace ui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_modified(false) {

    // Initialize core components
    m_project = std::make_unique<core::Project>();
    m_project->createDefaultProject();

    m_synth = std::make_unique<audio::Synthesizer>();
    m_synth->initDefaultInstruments();

    m_drums = std::make_unique<audio::DrumSample>();
    m_drums->generateAllSamples();

    m_scheduler = std::make_unique<audio::Scheduler>(m_project.get(), m_synth.get(), m_drums.get());
    m_audioEngine = std::make_unique<audio::AudioEngine>(m_scheduler.get());
    m_exporter = std::make_unique<audio::WavExporter>();

    setupUI();
    setupMenuBar();
    setupToolBar();
    setupConnections();

    updateWindowTitle();
    resize(1400, 900);
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Transport bar
    m_transport = new TransportWidget(this);
    mainLayout->addWidget(m_transport);

    // Main splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);

    // Left side: Editor area
    QWidget* editorContainer = new QWidget(m_mainSplitter);
    QVBoxLayout* editorLayout = new QVBoxLayout(editorContainer);
    editorLayout->setSpacing(0);
    editorLayout->setContentsMargins(0, 0, 0, 0);

    // Editor stack (Piano Roll / Step Sequencer)
    m_editorStack = new QStackedWidget(editorContainer);

    m_pianoRoll = new PianoRollWidget(m_project.get(), m_synth.get(), m_editorStack);
    m_stepSequencer = new StepSequencerWidget(m_project.get(), m_editorStack);

    m_editorStack->addWidget(m_pianoRoll);
    m_editorStack->addWidget(m_stepSequencer);
    m_editorStack->setCurrentIndex(0);

    editorLayout->addWidget(m_editorStack);

    m_mainSplitter->addWidget(editorContainer);

    // Right side: Mixer + Instrument Editor
    QWidget* rightPanel = new QWidget(m_mainSplitter);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setSpacing(0);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    m_instrumentEditor = new InstrumentEditor(m_synth.get(), rightPanel);
    rightLayout->addWidget(m_instrumentEditor);

    m_mixer = new MixerWidget(m_project.get(), rightPanel);
    rightLayout->addWidget(m_mixer);

    m_mainSplitter->addWidget(rightPanel);
    rightPanel->setMinimumWidth(320);
    m_mainSplitter->setSizes({900, 400});

    mainLayout->addWidget(m_mainSplitter);
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = new QMenuBar(this);

    // File menu
    QMenu* fileMenu = menuBar->addMenu("文件");
    fileMenu->addAction("新建项目", this, &MainWindow::onNewProject, QKeySequence::New);
    fileMenu->addAction("打开项目...", this, &MainWindow::onOpenProject, QKeySequence::Open);
    fileMenu->addAction("保存项目", this, &MainWindow::onSaveProject, QKeySequence::Save);
    fileMenu->addAction("另存为...", this, &MainWindow::onSaveProjectAs, QKeySequence::SaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction("导出音频...", this, &MainWindow::onExportAudio, QKeySequence("Ctrl+E"));
    fileMenu->addSeparator();
    fileMenu->addAction("退出", this, &QWidget::close, QKeySequence::Quit);

    // Edit menu (placeholder items - not yet implemented)
    QMenu* editMenu = menuBar->addMenu("编辑");
    QAction* undoAction = editMenu->addAction("撤销");
    undoAction->setEnabled(false);
    undoAction->setShortcut(QKeySequence::Undo);
    QAction* redoAction = editMenu->addAction("重做");
    redoAction->setEnabled(false);
    redoAction->setShortcut(QKeySequence::Redo);
    editMenu->addSeparator();
    QAction* cutAction = editMenu->addAction("剪切");
    cutAction->setEnabled(false);
    cutAction->setShortcut(QKeySequence::Cut);
    QAction* copyAction = editMenu->addAction("复制");
    copyAction->setEnabled(false);
    copyAction->setShortcut(QKeySequence::Copy);
    QAction* pasteAction = editMenu->addAction("粘贴");
    pasteAction->setEnabled(false);
    pasteAction->setShortcut(QKeySequence::Paste);

    // View menu
    QMenu* viewMenu = menuBar->addMenu("视图");
    viewMenu->addAction("钢琴卷帘", this, &MainWindow::onSwitchToPianoRoll, QKeySequence("Ctrl+1"));
    viewMenu->addAction("步进音序器", this, &MainWindow::onSwitchToStepSequencer, QKeySequence("Ctrl+2"));
    viewMenu->addSeparator();
    viewMenu->addAction("全屏", this, [this]() {
        setWindowState(windowState() ^ Qt::WindowFullScreen);
    }, QKeySequence("F11"));

    // Transport menu
    QMenu* transportMenu = menuBar->addMenu("传输");
    transportMenu->addAction("播放/暂停", this, &MainWindow::onTogglePlay, QKeySequence("Space"));
    transportMenu->addAction("停止", this, &MainWindow::onStop, QKeySequence("Escape"));

    // Help menu
    QMenu* helpMenu = menuBar->addMenu("帮助");
    helpMenu->addAction("关于", this, &MainWindow::onAbout);
    helpMenu->addAction("快捷键", this, &MainWindow::onShortcuts);

    setMenuBar(menuBar);
}

void MainWindow::setupToolBar() {
    QToolBar* toolBar = addToolBar("主工具栏");
    toolBar->setMovable(false);
    toolBar->setStyleSheet(
        "QToolBar {"
        "background: #2a2a2a;"
        "border-bottom: 1px solid #333;"
        "padding: 4px;"
        "}"
    );

    // Add stretch to push controls to the right
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolBar->addWidget(spacer);

    // Snap selector
    QLabel* snapLabel = new QLabel("网格:", this);
    snapLabel->setStyleSheet("color: #888; font-size: 12px;");
    toolBar->addWidget(snapLabel);
    
    QComboBox* snapCombo = new QComboBox(toolBar);
    snapCombo->addItem("1/4", 4);
    snapCombo->addItem("1/8", 8);
    snapCombo->addItem("1/16", 16);
    snapCombo->setCurrentIndex(2);
    snapCombo->setFixedWidth(80);
    snapCombo->setStyleSheet(
        "QComboBox {"
        "background: #333;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #e0e0e0;"
        "padding: 4px;"
        "font-size: 12px;"
        "}"
        "QComboBox QAbstractItemView {"
        "background: #333;"
        "color: #e0e0e0;"
        "}"
    );
    connect(snapCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this, snapCombo](int) {
        int snap = snapCombo->currentData().toInt();
        m_pianoRoll->setSnap(snap);
    });
    toolBar->addWidget(snapCombo);

    toolBar->addSeparator();

    // Zoom buttons
    QPushButton* zoomInBtn = new QPushButton("+", toolBar);
    zoomInBtn->setFixedSize(28, 28);
    zoomInBtn->setStyleSheet(
        "QPushButton {"
        "background: #333;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #e0e0e0;"
        "font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "background: #444;"
        "}"
    );
    connect(zoomInBtn, &QPushButton::clicked, m_pianoRoll, &PianoRollWidget::zoomIn);
    toolBar->addWidget(zoomInBtn);

    QPushButton* zoomOutBtn = new QPushButton("-", toolBar);
    zoomOutBtn->setFixedSize(28, 28);
    zoomOutBtn->setStyleSheet(
        "QPushButton {"
        "background: #333;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #e0e0e0;"
        "font-size: 14px;"
        "}"
        "QPushButton:hover {"
        "background: #444;"
        "}"
    );
    connect(zoomOutBtn, &QPushButton::clicked, m_pianoRoll, &PianoRollWidget::zoomOut);
    toolBar->addWidget(zoomOutBtn);
}

void MainWindow::setupConnections() {
    // Transport
    connect(m_transport, &TransportWidget::playClicked, this, &MainWindow::onTogglePlay);
    connect(m_transport, &TransportWidget::stopClicked, this, &MainWindow::onStop);
    connect(m_transport, &TransportWidget::bpmChanged, this, [this](int bpm) {
        m_scheduler->setBPM(bpm);
        m_project->bpm = bpm;
    });
    connect(m_transport, &TransportWidget::loopChanged, this, [this](bool loop) {
        m_project->loop = loop;
    });

    // Scheduler
    connect(m_scheduler.get(), &audio::Scheduler::stepChanged, this, &MainWindow::onStepChanged);
    connect(m_scheduler.get(), &audio::Scheduler::positionChanged, this, &MainWindow::onPositionChanged);

    // Piano roll
    connect(m_pianoRoll, &PianoRollWidget::noteAdded, this, [this]() {
        m_modified = true;
        updateWindowTitle();
    });
    connect(m_pianoRoll, &PianoRollWidget::noteRemoved, this, [this]() {
        m_modified = true;
        updateWindowTitle();
    });

    // Instrument editor
    connect(m_instrumentEditor, &InstrumentEditor::instrumentSelected, this, [this](int id) {
        m_pianoRoll->setCurrentInstrument(id);
    });
}

void MainWindow::updateWindowTitle() {
    QString title = "FL Studio Editor v5";
    if (!m_currentFilePath.isEmpty()) {
        title += " - " + QFileInfo(m_currentFilePath).fileName();
    }
    if (m_modified) {
        title += " *";
    }
    setWindowTitle(title);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (m_modified) {
        auto reply = QMessageBox::question(this, "未保存的更改",
            "项目有未保存的更改，是否保存？",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            onSaveProject();
            event->accept();
        } else if (reply == QMessageBox::Discard) {
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Space:
            onTogglePlay();
            break;
        case Qt::Key_Escape:
            onStop();
            break;
        default:
            QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::onNewProject() {
    if (m_modified) {
        auto reply = QMessageBox::question(this, "未保存的更改",
            "是否保存当前项目？",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

        if (reply == QMessageBox::Save) {
            onSaveProject();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }

    m_project->createDefaultProject();
    m_currentFilePath.clear();
    m_modified = false;
    updateWindowTitle();

    m_pianoRoll->update();
    m_stepSequencer->refresh();
    m_mixer->refresh();
}

void MainWindow::onOpenProject() {
    QString filePath = QFileDialog::getOpenFileName(this, "打开项目", QString(),
        "FL Studio 项目 (*.flproj);;JSON 文件 (*.json);;所有文件 (*)");

    if (!filePath.isEmpty()) {
        if (m_project->loadFromFile(filePath)) {
            m_currentFilePath = filePath;
            m_modified = false;
            updateWindowTitle();

            m_transport->setBPM(m_project->bpm);
            m_transport->setLoop(m_project->loop);

            m_pianoRoll->update();
            m_stepSequencer->refresh();
            m_mixer->refresh();
            m_instrumentEditor->refresh();
        } else {
            QMessageBox::critical(this, "错误", "无法打开项目文件。");
        }
    }
}

void MainWindow::onSaveProject() {
    if (m_currentFilePath.isEmpty()) {
        onSaveProjectAs();
    } else {
        if (m_project->saveToFile(m_currentFilePath)) {
            m_modified = false;
            updateWindowTitle();
        } else {
            QMessageBox::critical(this, "错误", "无法保存项目文件。");
        }
    }
}

void MainWindow::onSaveProjectAs() {
    QString filePath = QFileDialog::getSaveFileName(this, "保存项目", QString(),
        "FL Studio 项目 (*.flproj);;JSON 文件 (*.json)");

    if (!filePath.isEmpty()) {
        if (m_project->saveToFile(filePath)) {
            m_currentFilePath = filePath;
            m_modified = false;
            updateWindowTitle();
        } else {
            QMessageBox::critical(this, "错误", "无法保存项目文件。");
        }
    }
}

void MainWindow::onExportAudio() {
    QString filePath = QFileDialog::getSaveFileName(this, "导出音频", QString(),
        "WAV 文件 (*.wav)");

    if (!filePath.isEmpty()) {
        // Render offline
        int sampleRate = 44100;
        float duration = m_project->totalSteps * (60.0f / (m_project->bpm * 4.0f));
        int samples = static_cast<int>(duration * sampleRate);

        utils::AudioBuffer buffer(2, sampleRate);
        buffer.resize(samples);
        buffer.clear();

        m_scheduler->renderOffline(buffer);

        if (m_exporter->exportToFile(filePath, buffer)) {
            QMessageBox::information(this, "导出完成", "音频已成功导出。");
        } else {
            QMessageBox::critical(this, "导出失败", "无法导出音频文件。");
        }
    }
}

void MainWindow::onAbout() {
    QMessageBox::about(this, "关于 FL Studio Editor v5",
        "<h2>FL Studio Style Music Editor v5.0</h2>"
        "<p>一款类似 FL Studio 的音乐编辑软件</p>"
        "<p><b>功能特性：</b></p>"
        "<ul>"
        "<li>钢琴卷帘编辑器</li>"
        "<li>步进音序器</li>"
        "<li>多乐器音源支持</li>"
        "<li>FM/加法/减法合成</li>"
        "<li>混音器面板</li>"
        "<li>WAV 导出</li>"
        "</ul>"
        "<p>纯本地 C++/Qt 实现</p>"
        "<p>2024 Music Editor Team</p>");
}

void MainWindow::onShortcuts() {
    QMessageBox::information(this, "快捷键",
        "<h2>键盘快捷键</h2>"
        "<table>"
        "<tr><td><b>播放/暂停</b></td><td>Space</td></tr>"
        "<tr><td><b>停止</b></td><td>Escape</td></tr>"
        "<tr><td><b>新建项目</b></td><td>Ctrl+N</td></tr>"
        "<tr><td><b>打开项目</b></td><td>Ctrl+O</td></tr>"
        "<tr><td><b>保存项目</b></td><td>Ctrl+S</td></tr>"
        "<tr><td><b>导出音频</b></td><td>Ctrl+E</td></tr>"
        "<tr><td><b>钢琴卷帘</b></td><td>Ctrl+1</td></tr>"
        "<tr><td><b>步进音序器</b></td><td>Ctrl+2</td></tr>"
        "<tr><td><b>全屏</b></td><td>F11</td></tr>"
        "</table>"
        "<p><b>钢琴卷帘操作：</b></p>"
        "<ul>"
        "<li>左键点击：创建音符</li>"
        "<li>右键点击：删除音符</li>"
        "<li>拖拽：移动/调整音符</li>"
        "</ul>");
}

void MainWindow::onPlay() {
    m_audioEngine->initialize();
    m_scheduler->play();
    m_transport->setPlaying(true);
}

void MainWindow::onStop() {
    m_scheduler->stop();
    m_transport->setPlaying(false);
}

void MainWindow::onTogglePlay() {
    if (!m_scheduler->isPlaying()) {
        onPlay();
    } else if (m_scheduler->isPaused()) {
        m_scheduler->pause();
        m_transport->setPlaying(true);
    } else {
        m_scheduler->pause();
        m_transport->setPlaying(false);
    }
}

void MainWindow::onBPMChanged(int bpm) {
    m_scheduler->setBPM(bpm);
    m_project->bpm = bpm;
}

void MainWindow::onLoopChanged(bool loop) {
    m_project->loop = loop;
}

void MainWindow::onSwitchToPianoRoll() {
    m_editorStack->setCurrentIndex(0);
}

void MainWindow::onSwitchToStepSequencer() {
    m_editorStack->setCurrentIndex(1);
}

void MainWindow::onStepChanged(int step) {
    m_pianoRoll->updatePlayPosition(step);
    m_stepSequencer->updateStep(step);
}

void MainWindow::onPositionChanged(int bar, int beat, int subBeat) {
    m_transport->setPosition(bar, beat, subBeat);
}

} // namespace ui
} // namespace flstudio
