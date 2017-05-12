#include "ezgraver_factory.h"

#include <stdexcept>
#include <QString>
#include <QSerialPort>
#include <QDebug>

#include "ezgraver_v1.h"

namespace Ez {

std::shared_ptr<EzGraver> create(QString const& portName, Protocol protocol) {
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

    switch(protocol) {
    case Protocol::v1:
        return std::shared_ptr<EzGraver>{new EzGraverV1(serial)};
    default:
        throw std::invalid_argument{"unsupported protocol selected"};
    }

}

}

