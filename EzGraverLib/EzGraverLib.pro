include(../common.pri)

QT += gui
QT += serialport

TARGET = EzGraverLib
TEMPLATE = lib

DEFINES += EZGRAVERLIB_LIBRARY

SOURCES += ezgraver.cpp

HEADERS += ezgraver.h\
        ezgraverlib_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
