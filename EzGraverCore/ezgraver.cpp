#include "ezgraver.h"

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QBitmap>
#include <QBuffer>

#include <iterator>
#include <algorithm>
#include <functional>

namespace Ez {

EzGraver::EzGraver(std::shared_ptr<QSerialPort> serial, Protocol protocol) : _serial{serial}, _protocol{protocol} {}

void EzGraver::start(unsigned char const& burnTime) {
    _setBurnTime(burnTime);
    qDebug() << "starting engrave process";
    _transmit(0xF1);
}

void EzGraver::_setBurnTime(unsigned char const& burnTime) {
    if(burnTime < 0x01 || burnTime > 0xF0) {
        throw new std::out_of_range("burntime out of range");
    }
    qDebug() << "setting burn time to:" << qPrintable(burnTime);
    _transmit(burnTime);
}

void EzGraver::pause() {
    qDebug() << "pausing engrave process";
    _transmit(0xF2);
}

void EzGraver::reset() {
    qDebug() << "resetting";
    _transmit(0xF9);
}

void EzGraver::home() {
    qDebug() << "moving to home";
    _transmit(0xF3);
}

void EzGraver::center() {
    qDebug() << "moving to center";
    _transmit(0xFB);
}

void EzGraver::preview() {
    qDebug() << "drawing image preview";
    _transmit(0xF4);
}

void EzGraver::up() {
    qDebug() << "moving up";
    _transmit(0xF5);
}

void EzGraver::down() {
    qDebug() << "moving down";
    _transmit(0xF6);
}

void EzGraver::left() {
    qDebug() << "moving left";
    _transmit(0xF7);
}

void EzGraver::right() {
    qDebug() << "moving right";
    _transmit(0xF8);
}

void EzGraver::erase() {
    qDebug() << "erasing EEPROM";
    _transmit(QByteArray{8, '\xFE'});
}

int EzGraver::uploadImage(QImage const& originalImage) {
    qDebug() << "converting image to bitmap";
    QImage image{originalImage
            .scaled(ImageWidth, ImageHeight)
            .mirrored()
            .convertToFormat(QImage::Format_Mono)};
    image.invertPixels();

    QByteArray bytes{};
    QBuffer buffer{&bytes};
    image.save(&buffer, "BMP");
    return uploadImage(bytes);
}

int EzGraver::uploadImage(QByteArray const& image) {
    qDebug() << "uploading image";
    // Data is chunked in order to get at least some progress updates
    _transmit(image, 8192);
    return image.size();
}

void EzGraver::awaitTransmission(int msecs) {
    _serial->waitForBytesWritten(msecs);
}

std::shared_ptr<QSerialPort> EzGraver::serialPort() {
    return _serial;
}

void EzGraver::_transmit(unsigned char const& data) {
    _transmit(QByteArray{1, static_cast<char>(data)});
}

void EzGraver::_transmit(QByteArray const& data) {
    qDebug() << "transmitting" << data.size() << "bytes:" << data.toHex();
    _serial->write(data);
    _serial->flush();
}

void EzGraver::_transmit(QByteArray const& data, int chunkSize) {
    qDebug() << "transmitting" << data.size() << "bytes in chunks of size" << chunkSize;
    for(int i{0}; i < data.size(); i += chunkSize) {
        _serial->write(data.mid(i, chunkSize));
        _serial->flush();
    }
}

EzGraver::~EzGraver() {
    qDebug() << "EzGraver is being destroyed, closing serial port";
    _serial->close();
}

QStringList EzGraver::availablePorts() {
    auto toPortName = [](QSerialPortInfo const& port) { return port.portName(); };
    auto ports = QSerialPortInfo::availablePorts();
    QStringList result{};

    std::transform(ports.cbegin(), ports.cend(), std::back_inserter<QStringList>(result), toPortName);
    return result;
}

std::shared_ptr<EzGraver> EzGraver::create(QString const& portName, Protocol protocol) {
    qDebug() << "instantiating EzGraver on port" << portName;

    std::shared_ptr<QSerialPort> serial{new QSerialPort(portName)};
    serial->setBaudRate(QSerialPort::Baud57600, QSerialPort::AllDirections);
    serial->setParity(QSerialPort::Parity::NoParity);
    serial->setDataBits(QSerialPort::DataBits::Data8);
    serial->setStopBits(QSerialPort::StopBits::OneStop);

    if(!serial->open(QIODevice::ReadWrite)) {
        qDebug() << "failed to establish a connection on port" << portName;
        qDebug() << serial->errorString();
        throw std::runtime_error{QString{"failed to connect to port %1 (%2)"}.arg(portName, serial->errorString()).toStdString()};
    }

    return std::shared_ptr<EzGraver>{new EzGraver(serial, protocol)};
}

}
