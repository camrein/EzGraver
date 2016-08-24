#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T09:05:45
#
#-------------------------------------------------

QT += gui
QT += serialport

CONFIG += c++11

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
