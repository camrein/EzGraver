#include "ezgraver_factory.h"

#include <QString>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

#include <stdexcept>

#include "ezgraver.h"
#include "ezgraver_v1.h"
#include "ezgraver_v2.h"

namespace Ez {

std::shared_ptr<EzGraver> create(QString const& portName, int protocol) {
    qDebug() << "instantiating EzGraver on port" << portName << "with protocol version" << protocol;

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

    switch(protocol) {
    case 1:
        return std::shared_ptr<EzGraver>{new EzGraverV1(serial)};
    case 2:
        return std::shared_ptr<EzGraver>{new EzGraverV2(serial)};
    default:
        throw std::invalid_argument{QString{"unsupported protocol '%1' selected"}.arg(protocol).toStdString()};
    }
}

std::vector<int> protocols() {
    return std::vector<int>{1, 2};
}

QStringList availablePorts() {
    auto toPortName = [](QSerialPortInfo const& port) { return port.portName(); };
    auto ports = QSerialPortInfo::availablePorts();
    QStringList result{};

    std::transform(ports.cbegin(), ports.cend(), std::back_inserter<QStringList>(result), toPortName);
    return result;
}

}

