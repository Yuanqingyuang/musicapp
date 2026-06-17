#ifndef INSTRUMENT_EDITOR_H
#define INSTRUMENT_EDITOR_H

#include <QWidget>
#include <QListWidget>
#include <QComboBox>
#include <QSlider>
#include <QPushButton>
#include <QLineEdit>

namespace flstudio {
namespace audio {
    class Synthesizer;
    struct Instrument;
}
}

namespace flstudio {
namespace ui {

class InstrumentEditor : public QWidget {
    Q_OBJECT

public:
    explicit InstrumentEditor(audio::Synthesizer* synth, QWidget* parent = nullptr);
    ~InstrumentEditor();

    void refresh();
    int getSelectedInstrumentId() const;

signals:
    void instrumentSelected(int instrumentId);
    void instrumentChanged(int instrumentId);

private slots:
    void onAddInstrument();
    void onDeleteInstrument();
    void onInstrumentSelected();
    void onNameChanged();
    void onTypeChanged(int index);
    void onADSRChanged();
    void onFilterChanged();

private:
    void setupUI();
    void updateInstrumentList();
    void updateEditorFields();

    audio::Synthesizer* m_synth;
    int m_selectedInstrumentId;

    // UI elements
    QListWidget* m_instrumentList;
    QPushButton* m_addButton;
    QPushButton* m_deleteButton;
    QLineEdit* m_nameEdit;
    QComboBox* m_typeCombo;
    
    // ADSR sliders
    QSlider* m_attackSlider;
    QSlider* m_decaySlider;
    QSlider* m_sustainSlider;
    QSlider* m_releaseSlider;
    
    // Filter controls
    QComboBox* m_filterTypeCombo;
    QSlider* m_filterFreqSlider;
    QSlider* m_filterQSlider;
};

} // namespace ui
} // namespace flstudio

#endif // INSTRUMENT_EDITOR_H
