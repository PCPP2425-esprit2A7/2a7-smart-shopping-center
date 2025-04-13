QT       += core gui sql network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QT += 3dcore 3dextras 3drender 3dinput 3dlogic

TARGET = espace
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    espace.cpp \
    modifierespacesDialog.cpp \
    dimensionsdialog.cpp \
    espace3dview.cpp \
    clickablerectitem.cpp \
    connection.cpp

HEADERS += \
    mainwindow.h \
    espace.h \
    modifierespacesDialog.h \
    dimensionsdialog.h \
    espace3dview.h \
    clickablerectitem.h \
    connection.h

FORMS += \
    mainwindow.ui \
    modifierespacesDialog.ui \
    dimensionsdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    IMG.qrc \
    resources.qrc 