#ifndef MIXER_WIDGET_H
#define MIXER_WIDGET_H

#include <QWidget>
#include <QSlider>
#include <QPushButton>
#include <vector>

namespace flstudio {
namespace core {
    class Project;
    struct Track;
}
}

namespace flstudio {
namespace ui {

class MixerWidget : public QWidget {
    Q_OBJECT

public:
    explicit MixerWidget(core::Project* project, QWidget* parent = nullptr);
    ~MixerWidget();

    void refresh();

signals:
    void volumeChanged(int trackId, float volume);
    void panChanged(int trackId, float pan);
    void muteChanged(int trackId, bool mute);
    void soloChanged(int trackId, bool solo);

private slots:
    void onVolumeChanged(int value);
    void onPanChanged(int value);
    void onMuteClicked();
    void onSoloClicked();

private:
    void setupUI();
    void createTrackControls();
    void updateTrackUI(int trackIndex);

    core::Project* m_project;
    std::vector<QSlider*> m_volumeSliders;
    std::vector<QSlider*> m_panSliders;
    std::vector<QPushButton*> m_muteButtons;
    std::vector<QPushButton*> m_soloButtons;
    std::vector<QWidget*> m_vuMeters;
};

} // namespace ui
} // namespace flstudio

#endif // MIXER_WIDGET_H
