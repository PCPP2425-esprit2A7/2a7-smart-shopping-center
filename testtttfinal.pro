QT += core gui sql charts network widgets \
    quick
QT += core gui sql charts network multimedia multimediawidgets
QT += printsupport
QT += widgets gui pdf

# Configuration Python
PYTHON_PATH = $$(PYTHONPATH)
isEmpty(PYTHON_PATH) {
    win32: PYTHON_PATH = C:/Users/Yasmine/AppData/Local/Programs/Python/Python311
}

INCLUDEPATH += $${PYTHON_PATH}/include
LIBS += -L$${PYTHON_PATH}/libs -lpython311

# OpenCV configuration
INCLUDEPATH += C:/opencv/build/include
LIBS += -LC:/opencv/build/x64/vc16/lib

CONFIG += c++17

# Fichiers du projet
HEADERS += \
    connection.h \
    employe.h \
    facelogindialog.h \
    mainwindow.h

SOURCES += \
    connection.cpp \
    employe.cpp \
    facelogindialog.cpp \
    main.cpp \
    mainwindow.cpp

FORMS += \
    facelogindialog.ui \
    mainwindow.ui

# Fichiers Python à déployer
OTHER_FILES += \
    face_recognition.py \
    requirements.txt

# Copie des fichiers Python lors du déploiement
defineTest(copyToDestdir) {
    files = $$1
    dir = $$2
    for(file, files) {
        win32: QMAKE_POST_LINK += $$quote(cmd /c copy /y $$shell_quote($$file) $$shell_quote($$dir)$$escape_expand(\\n\\t))
    }
    export(QMAKE_POST_LINK)
}

copyToDestdir($$PWD/face_recognition.py, $$OUT_PWD)
copyToDestdir($$PWD/requirements.txt, $$OUT_PWD)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc


