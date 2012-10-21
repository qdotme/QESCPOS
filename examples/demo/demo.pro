#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T22:21:42
#
#-------------------------------------------------

QT       += core

QT       += gui

TARGET = demo
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

unix: DEFINES += _TTY_POSIX_
unix: LIBS += -lqextserialport


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../QESCPOS/ -lQESCPOS
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../QESCPOS/ -lQESCPOSd
else:unix: LIBS += -L$$OUT_PWD/../../QESCPOS/ -lQESCPOS

INCLUDEPATH += $$PWD/../../QESCPOS
DEPENDPATH += $$PWD/../../QESCPOS
