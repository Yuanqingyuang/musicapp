/****************************************************************************
** Meta object code from reading C++ file 'InstrumentEditor.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/ui/InstrumentEditor.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'InstrumentEditor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
namespace {
struct qt_meta_stringdata_flstudio__ui__InstrumentEditor_t {
    uint offsetsAndSizes[26];
    char stringdata0[31];
    char stringdata1[19];
    char stringdata2[1];
    char stringdata3[13];
    char stringdata4[18];
    char stringdata5[16];
    char stringdata6[19];
    char stringdata7[21];
    char stringdata8[14];
    char stringdata9[14];
    char stringdata10[6];
    char stringdata11[14];
    char stringdata12[16];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_flstudio__ui__InstrumentEditor_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_flstudio__ui__InstrumentEditor_t qt_meta_stringdata_flstudio__ui__InstrumentEditor = {
    {
        QT_MOC_LITERAL(0, 30),  // "flstudio::ui::InstrumentEditor"
        QT_MOC_LITERAL(31, 18),  // "instrumentSelected"
        QT_MOC_LITERAL(50, 0),  // ""
        QT_MOC_LITERAL(51, 12),  // "instrumentId"
        QT_MOC_LITERAL(64, 17),  // "instrumentChanged"
        QT_MOC_LITERAL(82, 15),  // "onAddInstrument"
        QT_MOC_LITERAL(98, 18),  // "onDeleteInstrument"
        QT_MOC_LITERAL(117, 20),  // "onInstrumentSelected"
        QT_MOC_LITERAL(138, 13),  // "onNameChanged"
        QT_MOC_LITERAL(152, 13),  // "onTypeChanged"
        QT_MOC_LITERAL(166, 5),  // "index"
        QT_MOC_LITERAL(172, 13),  // "onADSRChanged"
        QT_MOC_LITERAL(186, 15)   // "onFilterChanged"
    },
    "flstudio::ui::InstrumentEditor",
    "instrumentSelected",
    "",
    "instrumentId",
    "instrumentChanged",
    "onAddInstrument",
    "onDeleteInstrument",
    "onInstrumentSelected",
    "onNameChanged",
    "onTypeChanged",
    "index",
    "onADSRChanged",
    "onFilterChanged"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_flstudio__ui__InstrumentEditor[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,
       4,    1,   71,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       5,    0,   74,    2, 0x08,    5 /* Private */,
       6,    0,   75,    2, 0x08,    6 /* Private */,
       7,    0,   76,    2, 0x08,    7 /* Private */,
       8,    0,   77,    2, 0x08,    8 /* Private */,
       9,    1,   78,    2, 0x08,    9 /* Private */,
      11,    0,   81,    2, 0x08,   11 /* Private */,
      12,    0,   82,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    3,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   10,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject flstudio::ui::InstrumentEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_flstudio__ui__InstrumentEditor.offsetsAndSizes,
    qt_meta_data_flstudio__ui__InstrumentEditor,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_flstudio__ui__InstrumentEditor_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<InstrumentEditor, std::true_type>,
        // method 'instrumentSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'instrumentChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onAddInstrument'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onDeleteInstrument'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onInstrumentSelected'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNameChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onTypeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onADSRChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onFilterChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void flstudio::ui::InstrumentEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<InstrumentEditor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->instrumentSelected((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->instrumentChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->onAddInstrument(); break;
        case 3: _t->onDeleteInstrument(); break;
        case 4: _t->onInstrumentSelected(); break;
        case 5: _t->onNameChanged(); break;
        case 6: _t->onTypeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->onADSRChanged(); break;
        case 8: _t->onFilterChanged(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (InstrumentEditor::*)(int );
            if (_t _q_method = &InstrumentEditor::instrumentSelected; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (InstrumentEditor::*)(int );
            if (_t _q_method = &InstrumentEditor::instrumentChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject *flstudio::ui::InstrumentEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *flstudio::ui::InstrumentEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_flstudio__ui__InstrumentEditor.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int flstudio::ui::InstrumentEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void flstudio::ui::InstrumentEditor::instrumentSelected(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void flstudio::ui::InstrumentEditor::instrumentChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
