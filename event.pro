QT += core gui sql \
       pdf \
       printsupport \
       charts \
       network \
       widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calendrier.cpp \
    connexion.cpp \
    dialog.cpp \
    event.cpp \
    main.cpp \
    mainwindow.cpp \
    openaiclient.cpp

HEADERS += \
    calendrier.h \
    connexion.h \
    dialog.h \
    event.h \
    mainwindow.h \
    openaiclient.h

FORMS += \
    dialog.ui \
    mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    resss.qrc \
    img.qrc

