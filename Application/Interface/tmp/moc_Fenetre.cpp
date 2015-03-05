/****************************************************************************
** Meta object code from reading C++ file 'Fenetre.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Poker/Fenetre.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Fenetre.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Fenetre_t {
    QByteArrayData data[12];
    char stringdata[118];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_Fenetre_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_Fenetre_t qt_meta_stringdata_Fenetre = {
    {
QT_MOC_LITERAL(0, 0, 7),
QT_MOC_LITERAL(1, 8, 8),
QT_MOC_LITERAL(2, 17, 0),
QT_MOC_LITERAL(3, 18, 13),
QT_MOC_LITERAL(4, 32, 15),
QT_MOC_LITERAL(5, 48, 14),
QT_MOC_LITERAL(6, 63, 13),
QT_MOC_LITERAL(7, 77, 7),
QT_MOC_LITERAL(8, 85, 5),
QT_MOC_LITERAL(9, 91, 6),
QT_MOC_LITERAL(10, 98, 8),
QT_MOC_LITERAL(11, 107, 9)
    },
    "Fenetre\0tourFini\0\0affichageLogs\0"
    "demarragePartie\0prochainJoueur\0"
    "partieTermine\0checker\0miser\0suivre\0"
    "relancer\0seCoucher\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Fenetre[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       3,    0,   65,    2, 0x0a,
       4,    0,   66,    2, 0x0a,
       5,    0,   67,    2, 0x0a,
       6,    0,   68,    2, 0x0a,
       7,    0,   69,    2, 0x0a,
       8,    0,   70,    2, 0x0a,
       9,    0,   71,    2, 0x0a,
      10,    0,   72,    2, 0x0a,
      11,    0,   73,    2, 0x0a,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Fenetre::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Fenetre *_t = static_cast<Fenetre *>(_o);
        switch (_id) {
        case 0: _t->tourFini(); break;
        case 1: _t->affichageLogs(); break;
        case 2: _t->demarragePartie(); break;
        case 3: _t->prochainJoueur(); break;
        case 4: _t->partieTermine(); break;
        case 5: _t->checker(); break;
        case 6: _t->miser(); break;
        case 7: _t->suivre(); break;
        case 8: _t->relancer(); break;
        case 9: _t->seCoucher(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Fenetre::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Fenetre::tourFini)) {
                *result = 0;
            }
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Fenetre::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Fenetre.data,
      qt_meta_data_Fenetre,  qt_static_metacall, 0, 0}
};


const QMetaObject *Fenetre::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Fenetre::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Fenetre.stringdata))
        return static_cast<void*>(const_cast< Fenetre*>(this));
    return QWidget::qt_metacast(_clname);
}

int Fenetre::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void Fenetre::tourFini()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
