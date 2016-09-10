include(../common.pri)

QT += core
QT += serialport

TARGET = EzGraverCli
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../EzGraverCore/release/ -lEzGraverCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../EzGraverCore/debug/ -lEzGraverCore
else:unix: LIBS += -L$$OUT_PWD/../EzGraverCore/ -lEzGraverCore

INCLUDEPATH += $$PWD/../EzGraverCore
DEPENDPATH += $$PWD/../EzGraverCore
