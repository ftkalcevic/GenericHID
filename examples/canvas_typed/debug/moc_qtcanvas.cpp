/****************************************************************************
** Meta object code from reading C++ file 'qtcanvas.h'
**
** Created: Tue 2. Jun 18:12:32 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qtcanvas.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtcanvas.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QtCanvas[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      20,    9,    9,    9, 0x0a,
      30,    9,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_QtCanvas[] = {
    "QtCanvas\0\0resized()\0advance()\0update()\0"
};

const QMetaObject QtCanvas::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtCanvas,
      qt_meta_data_QtCanvas, 0 }
};

const QMetaObject *QtCanvas::metaObject() const
{
    return &staticMetaObject;
}

void *QtCanvas::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtCanvas))
        return static_cast<void*>(const_cast< QtCanvas*>(this));
    return QObject::qt_metacast(_clname);
}

int QtCanvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: resized(); break;
        case 1: advance(); break;
        case 2: update(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QtCanvas::resized()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_QtCanvasView[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       1,   22, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      21,   14,   13,   13, 0x0a,
      51,   13,   13,   13, 0x08,

 // properties: name, type, flags
      77,   72, 0x01095103,

       0        // eod
};

static const char qt_meta_stringdata_QtCanvasView[] = {
    "QtCanvasView\0\0enable\0setHighQualityRendering(bool)\0"
    "updateContentsSize()\0bool\0"
    "highQualityRendering\0"
};

const QMetaObject QtCanvasView::staticMetaObject = {
    { &QScrollArea::staticMetaObject, qt_meta_stringdata_QtCanvasView,
      qt_meta_data_QtCanvasView, 0 }
};

const QMetaObject *QtCanvasView::metaObject() const
{
    return &staticMetaObject;
}

void *QtCanvasView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QtCanvasView))
        return static_cast<void*>(const_cast< QtCanvasView*>(this));
    return QScrollArea::qt_metacast(_clname);
}

int QtCanvasView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QScrollArea::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setHighQualityRendering((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: updateContentsSize(); break;
        default: ;
        }
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = highQualityRendering(); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setHighQualityRendering(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 1;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
