/****************************************************************************
** Meta object code from reading C++ file 'scalepicker.h'
**
** Created: Fri Oct 10 14:38:41 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../scalepicker.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'scalepicker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ScalePicker[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      24,   13,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_ScalePicker[] = {
    "ScalePicker\0\0axis,value\0clicked(int,double)\0"
};

const QMetaObject ScalePicker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ScalePicker,
      qt_meta_data_ScalePicker, 0 }
};

const QMetaObject *ScalePicker::metaObject() const
{
    return &staticMetaObject;
}

void *ScalePicker::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ScalePicker))
	return static_cast<void*>(const_cast< ScalePicker*>(this));
    return QObject::qt_metacast(_clname);
}

int ScalePicker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: clicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ScalePicker::clicked(int _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
