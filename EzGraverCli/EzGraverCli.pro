QT += core
QT += gui

CONFIG += c++11

TARGET = EzGraverCli
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../EzGraverLib/release/ -lEzGraverLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../EzGraverLib/debug/ -lEzGraverLib
else:unix: LIBS += -L$$OUT_PWD/../EzGraverLib/ -lEzGraverLib

INCLUDEPATH += $$PWD/../EzGraverLib
DEPENDPATH += $$PWD/../EzGraverLib
