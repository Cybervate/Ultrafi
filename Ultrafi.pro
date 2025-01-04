QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    audioengine.cpp \
    downloadyt.cpp \
    equalizer.cpp \
    folderreader.cpp \
    main.cpp \
    mainwindow.cpp \
    metastructures.cpp \
    reverbdialog.cpp

HEADERS += \
    audioengine.h \
    downloadyt.h \
    equalizer.h \
    folderreader.h \
    mainwindow.h \
    metastructures.h \
    reverbdialog.h

FORMS += \
    downloadyt.ui \
    equalizer.ui \
    mainwindow.ui \
    reverbdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

LIBS += -ltag

unix:!macx: LIBS += -L$$PWD/../../../Desktop/irrKlang-64bit-1.6.0/bin/linux-gcc-64/ -lIrrKlang -lstk -lvlc
unix:!macx: LIBS += -L$$PWD/../../../Desktop/irrKlang-64bit-1.6.0/lib/

INCLUDEPATH += $$PWD/../../../Desktop/irrKlang-64bit-1.6.0/include
DEPENDPATH += $$PWD/../../../Desktop/irrKlang-64bit-1.6.0/include
INCLUDEPATH += ./
DEPENDPATH += ./

RESOURCES += \
    Icons.qrc

DISTFILES += \
    README.md \
    mdimages/home.png
