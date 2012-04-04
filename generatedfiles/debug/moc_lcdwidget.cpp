/****************************************************************************
** Meta object code from reading C++ file 'lcdwidget.h'
**
** Created: Thu 5. Apr 06:52:15 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "..\..\lcdwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'lcdwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_LCDWidget[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      23,   11,   10,   10, 0x05,
      71,   46,   10,   10, 0x05,
      97,   10,   10,   10, 0x05,
     107,  105,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
     124,   10,   10,   10, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_LCDWidget[] = {
    "LCDWidget\0\0nRow,nCol,s\0write(int,int,QString)\0"
    "nRow,nCol,bEnable,bBlink\0"
    "cursor(int,int,bool,bool)\0clear()\0n\0"
    "functionSet(int)\0onCursorTimer()\0"
};

const QMetaObject LCDWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_LCDWidget,
      qt_meta_data_LCDWidget, 0 }
};

const QMetaObject *LCDWidget::metaObject() const
{
    return &staticMetaObject;
}

void *LCDWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LCDWidget))
        return static_cast<void*>(const_cast< LCDWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int LCDWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: write((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 1: cursor((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 2: clear(); break;
        case 3: functionSet((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: onCursorTimer(); break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void LCDWidget::write(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LCDWidget::cursor(int _t1, int _t2, bool _t3, bool _t4)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LCDWidget::clear()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void LCDWidget::functionSet(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_END_MOC_NAMESPACE
