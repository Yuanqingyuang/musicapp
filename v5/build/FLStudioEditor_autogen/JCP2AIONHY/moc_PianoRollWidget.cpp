/****************************************************************************
** Meta object code from reading C++ file 'PianoRollWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../include/ui/PianoRollWidget.h"
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PianoRollWidget.h' doesn't include <QObject>."
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
struct qt_meta_stringdata_flstudio__ui__PianoRollWidget_t {
    uint offsetsAndSizes[20];
    char stringdata0[30];
    char stringdata1[10];
    char stringdata2[1];
    char stringdata3[11];
    char stringdata4[5];
    char stringdata5[12];
    char stringdata6[6];
    char stringdata7[13];
    char stringdata8[7];
    char stringdata9[8];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_flstudio__ui__PianoRollWidget_t::offsetsAndSizes) + ofs), len 
Q_CONSTINIT static const qt_meta_stringdata_flstudio__ui__PianoRollWidget_t qt_meta_stringdata_flstudio__ui__PianoRollWidget = {
    {
        QT_MOC_LITERAL(0, 29),  // "flstudio::ui::PianoRollWidget"
        QT_MOC_LITERAL(30, 9),  // "noteAdded"
        QT_MOC_LITERAL(40, 0),  // ""
        QT_MOC_LITERAL(41, 10),  // "core::Note"
        QT_MOC_LITERAL(52, 4),  // "note"
        QT_MOC_LITERAL(57, 11),  // "noteRemoved"
        QT_MOC_LITERAL(69, 5),  // "index"
        QT_MOC_LITERAL(75, 12),  // "noteModified"
        QT_MOC_LITERAL(88, 6),  // "zoomIn"
        QT_MOC_LITERAL(95, 7)   // "zoomOut"
    },
    "flstudio::ui::PianoRollWidget",
    "noteAdded",
    "",
    "core::Note",
    "note",
    "noteRemoved",
    "index",
    "noteModified",
    "zoomIn",
    "zoomOut"
};
#undef QT_MOC_LITERAL
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_flstudio__ui__PianoRollWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,
       7,    2,   50,    2, 0x06,    5 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       8,    0,   55,    2, 0x0a,    8 /* Public */,
       9,    0,   56,    2, 0x0a,    9 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 3,    6,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject flstudio::ui::PianoRollWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_flstudio__ui__PianoRollWidget.offsetsAndSizes,
    qt_meta_data_flstudio__ui__PianoRollWidget,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_flstudio__ui__PianoRollWidget_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<PianoRollWidget, std::true_type>,
        // method 'noteAdded'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const core::Note &, std::false_type>,
        // method 'noteRemoved'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'noteModified'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        QtPrivate::TypeAndForceComplete<const core::Note &, std::false_type>,
        // method 'zoomIn'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'zoomOut'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void flstudio::ui::PianoRollWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PianoRollWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->noteAdded((*reinterpret_cast< std::add_pointer_t<core::Note>>(_a[1]))); break;
        case 1: _t->noteRemoved((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->noteModified((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<core::Note>>(_a[2]))); break;
        case 3: _t->zoomIn(); break;
        case 4: _t->zoomOut(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PianoRollWidget::*)(const core::Note & );
            if (_t _q_method = &PianoRollWidget::noteAdded; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PianoRollWidget::*)(int );
            if (_t _q_method = &PianoRollWidget::noteRemoved; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PianoRollWidget::*)(int , const core::Note & );
            if (_t _q_method = &PianoRollWidget::noteModified; *reinterpret_cast<_t *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject *flstudio::ui::PianoRollWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *flstudio::ui::PianoRollWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_flstudio__ui__PianoRollWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int flstudio::ui::PianoRollWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void flstudio::ui::PianoRollWidget::noteAdded(const core::Note & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void flstudio::ui::PianoRollWidget::noteRemoved(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void flstudio::ui::PianoRollWidget::noteModified(int _t1, const core::Note & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
