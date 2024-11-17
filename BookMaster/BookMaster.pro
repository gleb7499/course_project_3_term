QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0

QT += testlib gui

RC_FILE = appIc.rc

CONFIG += c++11
CONFIG += windows

win32:QMAKE_LFLAGS += -mwindows

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    Data.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Data.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
