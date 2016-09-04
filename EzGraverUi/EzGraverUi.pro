include(../common.pri)

QT += core
QT += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = EzGraverUi
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    clicklabel.cpp \
    imagelabel.cpp

HEADERS  += mainwindow.h \
    clicklabel.h \
    imagelabel.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../EzGraverLib/release/ -lEzGraverLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../EzGraverLib/debug/ -lEzGraverLib
else:unix: LIBS += -L$$OUT_PWD/../EzGraverLib/ -lEzGraverLib

INCLUDEPATH += $$PWD/../EzGraverLib
DEPENDPATH += $$PWD/../EzGraverLib
