#include <QCoreApplication>
#include <QThread>

#include <iterator>
#include <algorithm>
#include <iostream>
#include <memory>
#include <exception>

#include "ezgraver.h"
#include "ezgraver_factory.h"

using namespace Ez;

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
    auto ports = EzGraver::availablePorts();
    std::cout << "Available Ports: ";
    std::ostream_iterator<QString> out{std::cout, " "};
    std::copy(ports.cbegin(), ports.cend(), out);
    std::cout << '\n';
}

void uploadImage(std::shared_ptr<EzGraver>& engraver, QList<QString> const& arguments) {
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
    engraver->erase();
    engraver->awaitTransmission();
    QThread::msleep(EzGraver::EraseTimeMs);

    std::cout << "uploading image to EEPROM\n";
    engraver->uploadImage(image);
}

void processCommand(char const& command, QList<QString> const& arguments) {
    try {
        std::shared_ptr<EzGraver> engraver{Ez::create(arguments[0])};

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

        engraver->awaitTransmission();
    } catch(std::exception const& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
}

void handleArguments(QStringList const& arguments) {
    if(arguments.size() < 2) {
        showHelp();
        return;
    }

    auto command = arguments[1][0].toLatin1();
    switch(command) {
    case 'a':
        showAvailablePorts();
        return;
    case 'v':
        std::cout << "EzGraver " << EZ_VERSION << '\n';
        return;
    }

    if(arguments.size() < 3) {
        showHelp();
        return;
    }

    processCommand(command, arguments.mid(2));
}

int main(int argc, char* argv[]) {
    QCoreApplication app{argc, argv};

    QStringList arguments{};
    std::copy(argv, argv+argc, std::back_insert_iterator<QStringList>(arguments));
    handleArguments(arguments);
}
