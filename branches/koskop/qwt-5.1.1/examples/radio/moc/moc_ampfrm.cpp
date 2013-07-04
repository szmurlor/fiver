/****************************************************************************
** Meta object code from reading C++ file 'ampfrm.h'
**
** Created: Fri Oct 10 14:38:48 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../ampfrm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ampfrm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_AmpFrame[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   10,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_AmpFrame[] = {
    "AmpFrame\0\0v\0setMaster(double)\0"
};

const QMetaObject AmpFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_AmpFrame,
      qt_meta_data_AmpFrame, 0 }
};

const QMetaObject *AmpFrame::metaObject() const
{
    return &staticMetaObject;
}

void *AmpFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AmpFrame))
	return static_cast<void*>(const_cast< AmpFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int AmpFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setMaster((*reinterpret_cast< double(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
