#include <QCoreApplication>
#include <QThread>

#include <algorithm>
#include <iostream>
#include <memory>
#include <exception>

#include "ezgraver.h"

std::ostream& operator<<(std::ostream& rhv, QString const& lhv) {
    return rhv << lhv.toStdString();
}

void showHelp() {
    std::cout << "Available options:\n";
    std::cout << "  <port> a - Shows the available ports\n";
    std::cout << "  <port> h - Moves the engraver to the home position\n";
    std::cout << "  <port> s [burnTime=60] - Starts the engraving process with the given burn time\n";
    std::cout << "  <port> p - Pauses the engraver\n";
    std::cout << "  <port> r - Resets the engraver\n";
    std::cout << "  <port> u <image> - Uploads the given image to the engraver\n";
}

void showAvailablePorts() {
    auto ports {EzGraver::availablePorts()};
    std::cout << "Available Ports: ";
    std::ostream_iterator<QString> out{std::cout, " "};
    std::copy(ports.cbegin(), ports.cend(), out);
    std::cout << '\n';
}

void uploadImage(std::shared_ptr<EzGraver>& engraver, QStringList const& arguments) {
    if(arguments.size() < 3) {
        std::cout << "No image provided\n";
        return;
    }

    auto fileName{arguments[2]};
    QImage image{};
    if(!image.load(fileName)) {
        std::cout << "Error while loading image '" << fileName << "'\n";
        return;
    }

    std::cout << "erasing EEPROM\n";
    engraver->erase();
    engraver->awaitTransmission();
    QThread::sleep(6);

    std::cout << "uploading image to EEPROM\n";
    engraver->uploadImage(image);
}

void handleArguments(QStringList const& arguments) {
    if(arguments.size() < 2) {
        showHelp();
        return;
    }

    try {
        std::shared_ptr<EzGraver> engraver{EzGraver::create(arguments[0])};

        auto argument{arguments[1][0].toLatin1()};
        switch(argument) {
        case 'a':
            showAvailablePorts();
            break;
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
            std::cout << "Unknown argument: '" << argument << "'\n";
            showHelp();
        }

        engraver->awaitTransmission();
    } catch(std::exception const& e) {
        std::cout << "Error: " << e.what() << '\n';
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication app{argc, argv};

    QStringList arguments{};
//    std::copy(argv, argv+argc, std::back_insert_iterator<QStringList>(arguments));
    arguments.push_back("COM1");
    arguments.push_back("u");
    arguments.push_back("C:/Users/Christoph/Desktop/bi9ql1K.png");
    handleArguments(arguments);
    /*for(auto i = 0; i < argc; ++i) {
        arguments.push_back(argv[i]);
    }*/


    //return app.exec();
}
