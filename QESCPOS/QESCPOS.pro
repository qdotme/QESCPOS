#-------------------------------------------------
#
# Project created by QtCreator 2012-10-20T22:21:08
#
#-------------------------------------------------

QT       += gui

TARGET = QESCPOS
TEMPLATE = lib

DEFINES += QESCPOS_LIBRARY

unix:DEFINES += _TTY_POSIX_


SOURCES += qescpos.cpp

HEADERS += qescpos.h\
        QESCPOS_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

unix:LIBS += -lqextserialport


header_files.files = $$HEADERS
unix:header_files.path = /usr/include/QESCPOS
INSTALLS += header_files
