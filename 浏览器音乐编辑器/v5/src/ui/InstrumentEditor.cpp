#include "ui/InstrumentEditor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSlider>
#include <QListWidget>
#include <QMessageBox>

namespace flstudio {
namespace ui {

InstrumentEditor::InstrumentEditor(QWidget* parent)
    : QWidget(parent)
    , m_selectedInstrumentId(0) {

    setupUI();
}

InstrumentEditor::~InstrumentEditor() = default;

void InstrumentEditor::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    // Title
    QHBoxLayout* titleLayout = new QHBoxLayout();
    QLabel* title = new QLabel("乐器列表", this);
    title->setStyleSheet("color: #fff; font-weight: bold; font-size: 14px;");
    titleLayout->addWidget(title);

    m_addButton = new QPushButton("+ 添加", this);
    m_addButton->setStyleSheet(
        "QPushButton {"
        "background: #4CAF50;"
        "border: 1px solid #66BB6A;"
        "border-radius: 4px;"
        "color: #fff;"
        "padding: 4px 8px;"
        "font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "background: #66BB6A;"
        "}"
    );
    connect(m_addButton, &QPushButton::clicked, this, &InstrumentEditor::onAddInstrument);
    titleLayout->addWidget(m_addButton);

    m_deleteButton = new QPushButton("删除", this);
    m_deleteButton->setStyleSheet(
        "QPushButton {"
        "background: #f44336;"
        "border: 1px solid #ef5350;"
        "border-radius: 4px;"
        "color: #fff;"
        "padding: 4px 8px;"
        "font-size: 11px;"
        "}"
        "QPushButton:hover {"
        "background: #ef5350;"
        "}"
    );
    connect(m_deleteButton, &QPushButton::clicked, this, &InstrumentEditor::onDeleteInstrument);
    titleLayout->addWidget(m_deleteButton);

    mainLayout->addLayout(titleLayout);

    // Instrument list
    m_instrumentList = new QListWidget(this);
    m_instrumentList->setStyleSheet(
        "QListWidget {"
        "background: #333;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #ccc;"
        "}"
        "QListWidget::item {"
        "padding: 8px;"
        "border-bottom: 1px solid #444;"
        "}"
        "QListWidget::item:selected {"
        "background: rgba(76, 175, 80, 0.3);"
        "border-color: #4CAF50;"
        "}"
        "QListWidget::item:hover {"
        "background: #3a3a3a;"
        "}"
    );
    connect(m_instrumentList, &QListWidget::itemClicked, this, &InstrumentEditor::onInstrumentSelected);
    mainLayout->addWidget(m_instrumentList);

    // Sound editor section
    QLabel* editorTitle = new QLabel("音色编辑", this);
    editorTitle->setStyleSheet("color: #ccc; font-weight: bold; font-size: 12px; margin-top: 8px;");
    mainLayout->addWidget(editorTitle);

    // Name
    QHBoxLayout* nameLayout = new QHBoxLayout();
    QLabel* nameLabel = new QLabel("名称:", this);
    nameLabel->setStyleSheet("color: #888; font-size: 12px;");
    nameLayout->addWidget(nameLabel);

    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setStyleSheet(
        "QLineEdit {"
        "background: #333;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #e0e0e0;"
        "padding: 4px;"
        "font-size: 12px;"
        "}"
    );
    connect(m_nameEdit, &QLineEdit::editingFinished, this, &InstrumentEditor::onNameChanged);
    nameLayout->addWidget(m_nameEdit);
    mainLayout->addLayout(nameLayout);

    // Type selector
    QHBoxLayout* typeLayout = new QHBoxLayout();
    QLabel* typeLabel = new QLabel("类型:", this);
    typeLabel->setStyleSheet("color: #888; font-size: 12px;");
    typeLayout->addWidget(typeLabel);

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem("FM 合成", 0);
    m_typeCombo->addItem("加法合成", 1);
    m_typeCombo->addItem("减法合成", 2);
    m_typeCombo->addItem("铺底", 3);
    m_typeCombo->setStyleSheet(
        "QComboBox {"
        "background: #333;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #e0e0e0;"
        "padding: 4px;"
        "font-size: 12px;"
        "}"
        "QComboBox::drop-down {"
        "border: none;"
        "}"
        "QComboBox QAbstractItemView {"
        "background: #333;"
        "color: #e0e0e0;"
        "selection-background-color: #4CAF50;"
        "}"
    );
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InstrumentEditor::onTypeChanged);
    typeLayout->addWidget(m_typeCombo);
    mainLayout->addLayout(typeLayout);

    // ADSR section
    QLabel* adsrTitle = new QLabel("ADSR 包络", this);
    adsrTitle->setStyleSheet("color: #888; font-size: 11px; margin-top: 8px;");
    mainLayout->addWidget(adsrTitle);

    QString sliderStyle =
        "QSlider::groove:horizontal {"
        "height: 6px;"
        "background: #333;"
        "border-radius: 3px;"
        "}"
        "QSlider::handle:horizontal {"
        "width: 14px;"
        "height: 14px;"
        "background: #2196F3;"
        "border: 2px solid #fff;"
        "border-radius: 7px;"
        "margin: -4px 0;"
        "}";

    // Attack row
    QHBoxLayout* attackRow = new QHBoxLayout();
    QLabel* attackLabel = new QLabel("Attack", this);
    attackLabel->setStyleSheet("color: #888; font-size: 10px; width: 40px;");
    attackRow->addWidget(attackLabel);
    m_attackSlider = new QSlider(Qt::Horizontal, this);
    m_attackSlider->setRange(0, 100);
    m_attackSlider->setStyleSheet(sliderStyle);
    connect(m_attackSlider, &QSlider::valueChanged, this, &InstrumentEditor::onADSRChanged);
    attackRow->addWidget(m_attackSlider);
    mainLayout->addLayout(attackRow);

    // Decay row
    QHBoxLayout* decayRow = new QHBoxLayout();
    QLabel* decayLabel = new QLabel("Decay", this);
    decayLabel->setStyleSheet("color: #888; font-size: 10px; width: 40px;");
    decayRow->addWidget(decayLabel);
    m_decaySlider = new QSlider(Qt::Horizontal, this);
    m_decaySlider->setRange(0, 100);
    m_decaySlider->setStyleSheet(sliderStyle);
    connect(m_decaySlider, &QSlider::valueChanged, this, &InstrumentEditor::onADSRChanged);
    decayRow->addWidget(m_decaySlider);
    mainLayout->addLayout(decayRow);

    // Sustain row
    QHBoxLayout* sustainRow = new QHBoxLayout();
    QLabel* sustainLabel = new QLabel("Sustain", this);
    sustainLabel->setStyleSheet("color: #888; font-size: 10px; width: 40px;");
    sustainRow->addWidget(sustainLabel);
    m_sustainSlider = new QSlider(Qt::Horizontal, this);
    m_sustainSlider->setRange(0, 100);
    m_sustainSlider->setStyleSheet(sliderStyle);
    connect(m_sustainSlider, &QSlider::valueChanged, this, &InstrumentEditor::onADSRChanged);
    sustainRow->addWidget(m_sustainSlider);
    mainLayout->addLayout(sustainRow);

    // Release row
    QHBoxLayout* releaseRow = new QHBoxLayout();
    QLabel* releaseLabel = new QLabel("Release", this);
    releaseLabel->setStyleSheet("color: #888; font-size: 10px; width: 40px;");
    releaseRow->addWidget(releaseLabel);
    m_releaseSlider = new QSlider(Qt::Horizontal, this);
    m_releaseSlider->setRange(0, 100);
    m_releaseSlider->setStyleSheet(sliderStyle);
    connect(m_releaseSlider, &QSlider::valueChanged, this, &InstrumentEditor::onADSRChanged);
    releaseRow->addWidget(m_releaseSlider);
    mainLayout->addLayout(releaseRow);

    // Filter section
    QLabel* filterTitle = new QLabel("滤波器", this);
    filterTitle->setStyleSheet("color: #888; font-size: 11px; margin-top: 8px;");
    mainLayout->addWidget(filterTitle);

    QHBoxLayout* filterLayout = new QHBoxLayout();

    m_filterTypeCombo = new QComboBox(this);
    m_filterTypeCombo->addItem("低通", "lowpass");
    m_filterTypeCombo->addItem("高通", "highpass");
    m_filterTypeCombo->addItem("带通", "bandpass");
    m_filterTypeCombo->setStyleSheet(
        "QComboBox {"
        "background: #333;"
        "border: 1px solid #444;"
        "border-radius: 4px;"
        "color: #e0e0e0;"
        "padding: 4px;"
        "font-size: 12px;"
        "}"
    );
    connect(m_filterTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &InstrumentEditor::onFilterChanged);
    filterLayout->addWidget(m_filterTypeCombo);

    m_filterFreqSlider = new QSlider(Qt::Horizontal, this);
    m_filterFreqSlider->setRange(100, 10000);
    m_filterFreqSlider->setStyleSheet(
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
    connect(m_filterFreqSlider, &QSlider::valueChanged, this, &InstrumentEditor::onFilterChanged);
    filterLayout->addWidget(m_filterFreqSlider);

    mainLayout->addLayout(filterLayout);

    mainLayout->addStretch();
}

void InstrumentEditor::updateInstrumentList() {
    m_instrumentList->clear();
}

void InstrumentEditor::updateEditorFields() {
}

void InstrumentEditor::refresh() {
    updateInstrumentList();
    updateEditorFields();
}

int InstrumentEditor::getSelectedInstrumentId() const {
    return m_selectedInstrumentId;
}

void InstrumentEditor::onAddInstrument() {
}

void InstrumentEditor::onDeleteInstrument() {
}

void InstrumentEditor::onInstrumentSelected() {
    QListWidgetItem* item = m_instrumentList->currentItem();
    if (!item) return;

    m_selectedInstrumentId = item->data(Qt::UserRole).toInt();
    updateEditorFields();
    emit instrumentSelected(m_selectedInstrumentId);
}

void InstrumentEditor::onNameChanged() {
}

void InstrumentEditor::onTypeChanged(int) {
}

void InstrumentEditor::onADSRChanged() {
}

void InstrumentEditor::onFilterChanged() {
}

} // namespace ui
} // namespace flstudio
