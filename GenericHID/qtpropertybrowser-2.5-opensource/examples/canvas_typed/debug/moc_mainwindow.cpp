/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Tue 2. Jun 18:12:32 2009
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CanvasView[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // signals: signature, parameters, type, tag, flags
      17,   12,   11,   11, 0x05,
      44,   12,   11,   11, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_CanvasView[] = {
    "CanvasView\0\0item\0itemClicked(QtCanvasItem*)\0"
    "itemMoved(QtCanvasItem*)\0"
};

const QMetaObject CanvasView::staticMetaObject = {
    { &QtCanvasView::staticMetaObject, qt_meta_stringdata_CanvasView,
      qt_meta_data_CanvasView, 0 }
};

const QMetaObject *CanvasView::metaObject() const
{
    return &staticMetaObject;
}

void *CanvasView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CanvasView))
        return static_cast<void*>(const_cast< CanvasView*>(this));
    return QtCanvasView::qt_metacast(_clname);
}

int CanvasView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtCanvasView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: itemClicked((*reinterpret_cast< QtCanvasItem*(*)>(_a[1]))); break;
        case 1: itemMoved((*reinterpret_cast< QtCanvasItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void CanvasView::itemClicked(QtCanvasItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CanvasView::itemMoved(QtCanvasItem * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_MainWindow[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      27,   11,   11,   11, 0x08,
      40,   11,   11,   11, 0x08,
      50,   11,   11,   11, 0x08,
      60,   11,   11,   11, 0x08,
      75,   11,   11,   11, 0x08,
      86,   11,   11,   11, 0x08,
     102,   97,   11,   11, 0x08,
     129,   97,   11,   11, 0x08,
     169,  154,   11,   11, 0x08,
     202,  154,   11,   11, 0x08,
     236,  154,   11,   11, 0x08,
     269,  154,   11,   11, 0x08,
     301,  154,   11,   11, 0x08,
     334,  154,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0newRectangle()\0newEllipse()\0"
    "newLine()\0newText()\0deleteObject()\0"
    "clearAll()\0fillView()\0item\0"
    "itemClicked(QtCanvasItem*)\0"
    "itemMoved(QtCanvasItem*)\0property,value\0"
    "valueChanged(QtProperty*,double)\0"
    "valueChanged(QtProperty*,QString)\0"
    "valueChanged(QtProperty*,QColor)\0"
    "valueChanged(QtProperty*,QFont)\0"
    "valueChanged(QtProperty*,QPoint)\0"
    "valueChanged(QtProperty*,QSize)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

const QMetaObject *MainWindow::metaObject() const
{
    return &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newRectangle(); break;
        case 1: newEllipse(); break;
        case 2: newLine(); break;
        case 3: newText(); break;
        case 4: deleteObject(); break;
        case 5: clearAll(); break;
        case 6: fillView(); break;
        case 7: itemClicked((*reinterpret_cast< QtCanvasItem*(*)>(_a[1]))); break;
        case 8: itemMoved((*reinterpret_cast< QtCanvasItem*(*)>(_a[1]))); break;
        case 9: valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 10: valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QColor(*)>(_a[2]))); break;
        case 12: valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QFont(*)>(_a[2]))); break;
        case 13: valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 14: valueChanged((*reinterpret_cast< QtProperty*(*)>(_a[1])),(*reinterpret_cast< const QSize(*)>(_a[2]))); break;
        default: ;
        }
        _id -= 15;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
