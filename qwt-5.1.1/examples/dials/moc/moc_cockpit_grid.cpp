/****************************************************************************
** Meta object code from reading C++ file 'cockpit_grid.h'
**
** Created: Fri Oct 10 14:38:57 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../cockpit_grid.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cockpit_grid.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CockpitGrid[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      27,   12,   12,   12, 0x08,
      44,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CockpitGrid[] = {
    "CockpitGrid\0\0changeSpeed()\0changeGradient()\0"
    "changeAngle()\0"
};

const QMetaObject CockpitGrid::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_CockpitGrid,
      qt_meta_data_CockpitGrid, 0 }
};

const QMetaObject *CockpitGrid::metaObject() const
{
    return &staticMetaObject;
}

void *CockpitGrid::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CockpitGrid))
	return static_cast<void*>(const_cast< CockpitGrid*>(this));
    return QFrame::qt_metacast(_clname);
}

int CockpitGrid::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeSpeed(); break;
        case 1: changeGradient(); break;
        case 2: changeAngle(); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
