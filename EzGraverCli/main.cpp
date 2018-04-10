#include <QCoreApplication>
#include <QThread>
#include <QTimer>

#include <iterator>
#include <algorithm>
#include <iostream>
#include <memory>
#include <exception>

#include "ezgraver.h"
#include "factory.h"
#include "specifications.h"

std::ostream& operator<<(std::ostream& lhv, QString const& rhv) {
    return lhv << rhv.toStdString();
}

void showHelp() {
    std::cout << "Usage: EzGraverCli <option> [arguments...]\n\n";
    std::cout << "Available options:\n";
    std::cout << "  v - Prints the version information\n";
    std::cout << "  a - Shows the available ports\n";
    std::cout << "  h <port> - Moves the engraver to the home position\n";
    std::cout << "  s <port> - Starts the engraving process with the burn time 60\n";
    std::cout << "  p <port> - Pauses the engraver\n";
    std::cout << "  r <port> - Resets the engraver\n";
    std::cout << "  u <port> <image> - Uploads the given image to the engraver\n";
}

void showAvailablePorts() {
    auto ports = Ez::availablePorts();
    std::cout << "Available Ports: ";
    std::ostream_iterator<QString> out{std::cout, " "};
    std::copy(ports.cbegin(), ports.cend(), out);
    std::cout << '\n';
}

void uploadImage(std::shared_ptr<Ez::EzGraver>& engraver, QList<QString> const& arguments) {
    if(arguments.size() < 2) {
        std::cout << "No image provided\n";
        return;
    }

    auto fileName = arguments[1];
    QImage image{};
    if(!image.load(fileName)) {
        std::cout << "Error while loading image '" << fileName << "'\n";
        return;
    }

    std::cout << "erasing EEPROM\n";
    auto waitTimeMs = engraver->erase();
    QThread::msleep(waitTimeMs);

    std::cout << "uploading image to EEPROM\n";
    engraver->uploadImage(image);
}

void awaitTransmission(QCoreApplication& app, std::shared_ptr<Ez::EzGraver> engraver) {
    if(engraver->serialPort()->bytesToWrite() > 0) {
        QTimer::singleShot(100, [&app, engraver]{ awaitTransmission(app, engraver );});
    } else {
        app.quit();
    }
}

void processCommand(QCoreApplication& app, char const& command, QList<QString> const& arguments) {
    try {
        auto engraver = Ez::create(arguments[0]);

        switch(command) {
        case 'h':
            engraver->home();
            break;
        case 'c':
            engraver->center();
            break;
        case 's':
            engraver->start(60);
            break;
        case 'r':
            engraver->reset();
            break;
        case 'p':
            engraver->pause();
            break;
        case 'u':
            uploadImage(engraver, arguments);
            break;
        default:
            std::cout << "Unknown command: '" << command << "'\n";
            showHelp();
        }
        awaitTransmission(app, engraver);
    } catch(std::exception const& e) {
        std::cout << "Error: " << e.what() << '\n';
        app.quit();
    }
}

void handleArguments(QCoreApplication& app) {
    auto arguments = app.arguments();
    if(arguments.size() < 2) {
        showHelp();
        app.quit();
        return;
    }

    auto command = arguments[1][0].toLatin1();
    switch(command) {
    case 'a':
        showAvailablePorts();
        app.quit();
        return;
    case 'v':
        std::cout << "EzGraver " << EZ_VERSION << '\n';
        app.quit();
        return;
    }

    if(arguments.size() < 3) {
        showHelp();
        app.quit();
        return;
    }

    processCommand(app, command, arguments.mid(2));
}

int main(int argc, char* argv[]) {
    QCoreApplication app{argc, argv};
    QTimer::singleShot(0, [&app]{ handleArguments(app); });
    return app.exec();
}
