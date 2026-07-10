#ifndef TRANSPORT_WIDGET_H
#define TRANSPORT_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QCheckBox>

namespace flstudio {
namespace ui {

class TransportWidget : public QWidget {
    Q_OBJECT

public:
    explicit TransportWidget(QWidget* parent = nullptr);
    ~TransportWidget();

    void setPlaying(bool playing);
    void setBPM(int bpm);
    void setLoop(bool loop);
    void setPosition(int bar, int beat, int subBeat);

signals:
    void playClicked();
    void stopClicked();
    void pauseClicked();
    void bpmChanged(int bpm);
    void loopChanged(bool loop);

private slots:
    void onPlayClicked();
    void onStopClicked();
    void onBPMChanged(int value);
    void onLoopChanged(int state);

private:
    void setupUI();

    QPushButton* m_playButton;
    QPushButton* m_stopButton;
    QLabel* m_timeLabel;
    QSpinBox* m_bpmSpinBox;
    QCheckBox* m_loopCheckBox;
    bool m_isPlaying;
};

} // namespace ui
} // namespace flstudio

#endif // TRANSPORT_WIDGET_H
