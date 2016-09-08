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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../EzGraverCore/release/ -lEzGraverCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../EzGraverCore/debug/ -lEzGraverCore
else:unix: LIBS += -L$$OUT_PWD/../EzGraverCore/ -lEzGraverCore

INCLUDEPATH += $$PWD/../EzGraverCore
DEPENDPATH += $$PWD/../EzGraverCore
