#include "mainwindow.h"
#include <QApplication>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle(QString{"EzGraver %1"}.arg(EZ_VERSION));

    return a.exec();
}
