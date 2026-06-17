/****************************************************************************
** Meta object code from reading C++ file 'TransportWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/ui/TransportWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'TransportWidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_flstudio__ui__TransportWidget_t {
    uint offsetsAndSizes[30];
    char stringdata0[30];
    char stringdata1[12];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[13];
    char stringdata5[11];
    char stringdata6[4];
    char stringdata7[12];
    char stringdata8[5];
    char stringdata9[14];
    char stringdata10[14];
    char stringdata11[13];
    char stringdata12[6];
    char stringdata13[14];
    char stringdata14[6];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_flstudio__ui__TransportWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_flstudio__ui__TransportWidget_t qt_meta_stringdata_flstudio__ui__TransportWidget = {
    {
        QT_MOC_LITERAL(0, 29),  // "flstudio::ui::TransportWidget"
        QT_MOC_LITERAL(30, 11),  // "playClicked"
        QT_MOC_LITERAL(42, 0),  // ""
        QT_MOC_LITERAL(43, 11),  // "stopClicked"
        QT_MOC_LITERAL(55, 12),  // "pauseClicked"
        QT_MOC_LITERAL(68, 10),  // "bpmChanged"
        QT_MOC_LITERAL(79, 3),  // "bpm"
        QT_MOC_LITERAL(83, 11),  // "loopChanged"
        QT_MOC_LITERAL(95, 4),  // "loop"
        QT_MOC_LITERAL(100, 13),  // "onPlayClicked"
        QT_MOC_LITERAL(114, 13),  // "onStopClicked"
        QT_MOC_LITERAL(128, 12),  // "onBPMChanged"
        QT_MOC_LITERAL(141, 5),  // "value"
        QT_MOC_LITERAL(147, 13),  // "onLoopChanged"
        QT_MOC_LITERAL(161, 5)   // "state"
    },
    "flstudio::ui::TransportWidget",
    "playClicked",
    "",
    "stopClicked",
    "pauseClicked",
    "bpmChanged",
    "bpm",
    "loopChanged",
    "loop",
    "onPlayClicked",
    "onStopClicked",
    "onBPMChanged",
    "value",
    "onLoopChanged",
    "state"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_flstudio__ui__TransportWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   68,    2, 0x06,    1 /* Public */,
       3,    0,   69,    2, 0x06,    2 /* Public */,
       4,    0,   70,    2, 0x06,    3 /* Public */,
       5,    1,   71,    2, 0x06,    4 /* Public */,
       7,    1,   74,    2, 0x06,    6 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       9,    0,   77,    2, 0x08,    8 /* Private */,
      10,    0,   78,    2, 0x08,    9 /* Private */,
      11,    1,   79,    2, 0x08,   10 /* Private */,
      13,    1,   82,    2, 0x08,   12 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Bool,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   12,
    QMetaType::Void, QMetaType::Int,   14,

       0        // eod
};

Q_CONSTINIT const QMetaObject flstudio::ui::TransportWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_flstudio__ui__TransportWidget.offsetsAndSizes,
    qt_meta_data_flstudio__ui__TransportWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_flstudio__ui__TransportWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<TransportWidget, std::true_type>,
        // method 'playClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'stopClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'pauseClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'bpmChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'loopChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<bool, std::false_type>,
        // method 'onPlayClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onStopClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBPMChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'onLoopChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>
    >,
    nullptr
} };

void flstudio::ui::TransportWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TransportWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->playClicked(); break;
        case 1: _t->stopClicked(); break;
        case 2: _t->pauseClicked(); break;
        case 3: _t->bpmChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 4: _t->loopChanged((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 5: _t->onPlayClicked(); break;
        case 6: _t->onStopClicked(); break;
        case 7: _t->onBPMChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 8: _t->onLoopChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TransportWidget::*)();
            if (_t _q_method = &TransportWidget::playClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TransportWidget::*)();
            if (_t _q_method = &TransportWidget::stopClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (TransportWidget::*)();
            if (_t _q_method = &TransportWidget::pauseClicked; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (TransportWidget::*)(int );
            if (_t _q_method = &TransportWidget::bpmChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (TransportWidget::*)(bool );
            if (_t _q_method = &TransportWidget::loopChanged; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *flstudio::ui::TransportWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *flstudio::ui::TransportWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_flstudio__ui__TransportWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int flstudio::ui::TransportWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void flstudio::ui::TransportWidget::playClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void flstudio::ui::TransportWidget::stopClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void flstudio::ui::TransportWidget::pauseClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void flstudio::ui::TransportWidget::bpmChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void flstudio::ui::TransportWidget::loopChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
