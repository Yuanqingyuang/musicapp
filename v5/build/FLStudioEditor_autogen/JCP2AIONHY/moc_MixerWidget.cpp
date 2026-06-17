/****************************************************************************
** Meta object code from reading C++ file 'MixerWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/ui/MixerWidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MixerWidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_flstudio__ui__MixerWidget_t {
    uint offsetsAndSizes[32];
    char stringdata0[26];
    char stringdata1[14];
    char stringdata2[1];
    char stringdata3[8];
    char stringdata4[7];
    char stringdata5[11];
    char stringdata6[4];
    char stringdata7[12];
    char stringdata8[5];
    char stringdata9[12];
    char stringdata10[5];
    char stringdata11[16];
    char stringdata12[6];
    char stringdata13[13];
    char stringdata14[14];
    char stringdata15[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_flstudio__ui__MixerWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_flstudio__ui__MixerWidget_t qt_meta_stringdata_flstudio__ui__MixerWidget = {
    {
        QT_MOC_LITERAL(0, 25),  // "flstudio::ui::MixerWidget"
        QT_MOC_LITERAL(26, 13),  // "volumeChanged"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 7),  // "trackId"
        QT_MOC_LITERAL(49, 6),  // "volume"
        QT_MOC_LITERAL(56, 10),  // "panChanged"
        QT_MOC_LITERAL(67, 3),  // "pan"
        QT_MOC_LITERAL(71, 11),  // "muteChanged"
        QT_MOC_LITERAL(83, 4),  // "mute"
        QT_MOC_LITERAL(88, 11),  // "soloChanged"
        QT_MOC_LITERAL(100, 4),  // "solo"
        QT_MOC_LITERAL(105, 15),  // "onVolumeChanged"
        QT_MOC_LITERAL(121, 5),  // "value"
        QT_MOC_LITERAL(127, 12),  // "onPanChanged"
        QT_MOC_LITERAL(140, 13),  // "onMuteClicked"
        QT_MOC_LITERAL(154, 13)   // "onSoloClicked"
    },
    "flstudio::ui::MixerWidget",
    "volumeChanged",
    "",
    "trackId",
    "volume",
    "panChanged",
    "pan",
    "muteChanged",
    "mute",
    "soloChanged",
    "solo",
    "onVolumeChanged",
    "value",
    "onPanChanged",
    "onMuteClicked",
    "onSoloClicked"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_flstudio__ui__MixerWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    2,   62,    2, 0x06,    1 /* Public */,
       5,    2,   67,    2, 0x06,    4 /* Public */,
       7,    2,   72,    2, 0x06,    7 /* Public */,
       9,    2,   77,    2, 0x06,   10 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      11,    1,   82,    2, 0x08,   13 /* Private */,
      13,    1,   85,    2, 0x08,   15 /* Private */,
      14,    0,   88,    2, 0x08,   17 /* Private */,
      15,    0,   89,    2, 0x08,   18 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::Float,    3,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,    3,   10,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject flstudio::ui::MixerWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_flstudio__ui__MixerWidget.offsetsAndSizes,
    qt_meta_data_flstudio__ui__MixerWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_flstudio__ui__MixerWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MixerWidget, std::true_type>,
        // method 'volumeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'panChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<float, std::false_type>,
        // method 'muteChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'soloChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onVolumeChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onPanChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onMuteClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onSoloClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void flstudio::ui::MixerWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MixerWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->volumeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 1: _t->panChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<float>>(_a[2]))); break;
        case 2: _t->muteChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 3: _t->soloChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 4: _t->onVolumeChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->onPanChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->onMuteClicked(); break;
        case 7: _t->onSoloClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MixerWidget::*)(int , float );
            if (_t _q_method = &MixerWidget::volumeChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MixerWidget::*)(int , float );
            if (_t _q_method = &MixerWidget::panChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MixerWidget::*)(int , bool );
            if (_t _q_method = &MixerWidget::muteChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MixerWidget::*)(int , bool );
            if (_t _q_method = &MixerWidget::soloChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *flstudio::ui::MixerWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *flstudio::ui::MixerWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_flstudio__ui__MixerWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int flstudio::ui::MixerWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void flstudio::ui::MixerWidget::volumeChanged(int _t1, float _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void flstudio::ui::MixerWidget::panChanged(int _t1, float _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void flstudio::ui::MixerWidget::muteChanged(int _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void flstudio::ui::MixerWidget::soloChanged(int _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
