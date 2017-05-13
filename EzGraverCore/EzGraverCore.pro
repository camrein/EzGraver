include(../common.pri)

QT += core
QT += serialport

TARGET = EzGraverCore
TEMPLATE = lib

DEFINES += EZGRAVERCORE_LIBRARY

SOURCES += ezgraver.cpp \
    ezgraver_v1.cpp \
    ezgraver_factory.cpp \
    ezgraver_v2.cpp

HEADERS += ezgraver.h\
        ezgravercore_global.h \
    ezgraver_v1.h \
    ezgraver_factory.h \
    ezgraver_v2.h \
    specifications.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
