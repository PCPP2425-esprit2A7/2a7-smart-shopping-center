/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.7.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.7.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {

#ifdef QT_MOC_HAS_STRINGDATA
struct qt_meta_stringdata_CLASSMainWindowENDCLASS_t {};
constexpr auto qt_meta_stringdata_CLASSMainWindowENDCLASS = QtMocHelpers::stringData(
    "MainWindow",
    "on_pushButton_ajouter_clicked",
    "",
    "on_pushButton_choisirImage_clicked",
    "afficherEmployes",
    "on_pushButton_supprimer_clicked",
    "on_pushButton_exporterPDF_clicked",
    "trierEmployes",
    "afficherStatistiques",
    "changerIndication",
    "rechercherEmployes",
    "setEmploye",
    "Employe",
    "employe",
    "getEmployeModifie",
    "on_pushButton_modifierPhoto_clicked",
    "on_pushButton_enregistrer_clicked",
    "on_pushButton_appliquerModif_clicked",
    "afficherDetailsEmploye",
    "QModelIndex",
    "index",
    "onPhotoClicked",
    "on_pushButton_analysePDP_clicked",
    "saveImageToDatabase",
    "pixmap",
    "on_btnVerifier_clicked",
    "genererPDF",
    "employee",
    "chargementTodoList",
    "on_btnAccederTodo_clicked",
    "toutMarquerCommeTermine",
    "on_pushButton_faceId_clicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA
} // unnamed namespace

Q_CONSTINIT static const uint qt_meta_data_CLASSMainWindowENDCLASS[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  158,    2, 0x08,    1 /* Private */,
       3,    0,  159,    2, 0x08,    2 /* Private */,
       4,    0,  160,    2, 0x08,    3 /* Private */,
       5,    0,  161,    2, 0x08,    4 /* Private */,
       6,    0,  162,    2, 0x08,    5 /* Private */,
       7,    0,  163,    2, 0x08,    6 /* Private */,
       8,    0,  164,    2, 0x08,    7 /* Private */,
       9,    0,  165,    2, 0x08,    8 /* Private */,
      10,    0,  166,    2, 0x08,    9 /* Private */,
      11,    1,  167,    2, 0x08,   10 /* Private */,
      14,    0,  170,    2, 0x108,   12 /* Private | MethodIsConst  */,
      15,    0,  171,    2, 0x08,   13 /* Private */,
      16,    0,  172,    2, 0x08,   14 /* Private */,
      17,    0,  173,    2, 0x08,   15 /* Private */,
      18,    1,  174,    2, 0x08,   16 /* Private */,
      21,    1,  177,    2, 0x08,   18 /* Private */,
      22,    0,  180,    2, 0x08,   20 /* Private */,
      23,    1,  181,    2, 0x08,   21 /* Private */,
      25,    0,  184,    2, 0x08,   23 /* Private */,
      26,    1,  185,    2, 0x08,   24 /* Private */,
      28,    0,  188,    2, 0x08,   26 /* Private */,
      29,    0,  189,    2, 0x08,   27 /* Private */,
      30,    0,  190,    2, 0x08,   28 /* Private */,
      31,    0,  191,    2, 0x08,   29 /* Private */,

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
    QMetaType::Void, 0x80000000 | 12,   13,
    0x80000000 | 12,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPixmap,   24,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariantMap,   27,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_CLASSMainWindowENDCLASS.offsetsAndSizes,
    qt_meta_data_CLASSMainWindowENDCLASS,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_stringdata_CLASSMainWindowENDCLASS_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'on_pushButton_ajouter_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_choisirImage_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherEmployes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_supprimer_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_exporterPDF_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'trierEmployes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherStatistiques'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'changerIndication'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'rechercherEmployes'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setEmploye'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const Employe &, std::false_type>,
        // method 'getEmployeModifie'
        QtPrivate::TypeAndForceComplete<Employe, std::false_type>,
        // method 'on_pushButton_modifierPhoto_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_enregistrer_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_appliquerModif_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'afficherDetailsEmploye'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'onPhotoClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QModelIndex &, std::false_type>,
        // method 'on_pushButton_analysePDP_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveImageToDatabase'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QPixmap &, std::false_type>,
        // method 'on_btnVerifier_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'genererPDF'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QMap<QString,QVariant> &, std::false_type>,
        // method 'chargementTodoList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_btnAccederTodo_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'toutMarquerCommeTermine'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_pushButton_faceId_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->on_pushButton_ajouter_clicked(); break;
        case 1: _t->on_pushButton_choisirImage_clicked(); break;
        case 2: _t->afficherEmployes(); break;
        case 3: _t->on_pushButton_supprimer_clicked(); break;
        case 4: _t->on_pushButton_exporterPDF_clicked(); break;
        case 5: _t->trierEmployes(); break;
        case 6: _t->afficherStatistiques(); break;
        case 7: _t->changerIndication(); break;
        case 8: _t->rechercherEmployes(); break;
        case 9: _t->setEmploye((*reinterpret_cast< std::add_pointer_t<Employe>>(_a[1]))); break;
        case 10: { Employe _r = _t->getEmployeModifie();
            if (_a[0]) *reinterpret_cast< Employe*>(_a[0]) = std::move(_r); }  break;
        case 11: _t->on_pushButton_modifierPhoto_clicked(); break;
        case 12: _t->on_pushButton_enregistrer_clicked(); break;
        case 13: _t->on_pushButton_appliquerModif_clicked(); break;
        case 14: _t->afficherDetailsEmploye((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 15: _t->onPhotoClicked((*reinterpret_cast< std::add_pointer_t<QModelIndex>>(_a[1]))); break;
        case 16: _t->on_pushButton_analysePDP_clicked(); break;
        case 17: _t->saveImageToDatabase((*reinterpret_cast< std::add_pointer_t<QPixmap>>(_a[1]))); break;
        case 18: _t->on_btnVerifier_clicked(); break;
        case 19: _t->genererPDF((*reinterpret_cast< std::add_pointer_t<QMap<QString,QVariant>>>(_a[1]))); break;
        case 20: _t->chargementTodoList(); break;
        case 21: _t->on_btnAccederTodo_clicked(); break;
        case 22: _t->toutMarquerCommeTermine(); break;
        case 23: _t->on_pushButton_faceId_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CLASSMainWindowENDCLASS.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 24)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 24;
    }
    return _id;
}
QT_WARNING_POP
