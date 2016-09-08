include(../common.pri)

QT += gui
QT += serialport

TARGET = EzGraverCore
TEMPLATE = lib

DEFINES += EZGRAVERCORE_LIBRARY

SOURCES += ezgraver.cpp

HEADERS += ezgraver.h\
        ezgravercore_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
