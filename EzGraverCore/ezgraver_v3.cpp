#include "ezgraver_v3.h"

#include <QDebug>
#include <QByteArray>

namespace Ez {

void EzGraverV3::start(unsigned char const& burnTime) {
    _setBurnTime(burnTime);
    qDebug() << "starting engrave process";
    _transmit(QByteArray::fromRawData("\xFF\x01\x01\x00", 4));
}

void EzGraverV3::_setBurnTime(unsigned char const& burnTime) {
    if(burnTime < 0x01 || burnTime > 0xF0) {
        throw new std::out_of_range("burntime out of range");
    }
    qDebug() << "setting burn time to:" << qPrintable(burnTime);

    char payload[5] = "\xFF\x05?\x00";
    payload[2] = burnTime;
    _transmit(QByteArray{payload, 4});
}

void EzGraverV3::pause() {
    qDebug() << "pausing engrave process";
    _transmit(QByteArray::fromRawData("\xFF\x01\x02\x00", 4));
}

void EzGraverV3::reset() {
    qDebug() << "resetting";
    _transmit(QByteArray::fromRawData("\xFF\x04\x01\x00", 4));
}

void EzGraverV3::home() {
    qDebug() << "moving to home";
    _transmit(0xF3);
}

void EzGraverV3::center() {
    qDebug() << "moving to center";
    _transmit(QByteArray::fromRawData("\xFF\x02\x01\x00", 4));
}

void EzGraverV3::preview() {
    qDebug() << "drawing image preview";
    _transmit(QByteArray::fromRawData("\xFF\x02\x02\x00", 4));
}

void EzGraverV3::up() {
    qDebug() << "moving up";
    _transmit(QByteArray::fromRawData("\xFF\x03\x01\x00", 4));
}

void EzGraverV3::down() {
    qDebug() << "moving down";
    _transmit(QByteArray::fromRawData("\xFF\x03\x02\x00", 4));
}

void EzGraverV3::left() {
    qDebug() << "moving left";
    _transmit(QByteArray::fromRawData("\xFF\x03\x03\x00", 4));
}

void EzGraverV3::right() {
    qDebug() << "moving right";
    _transmit(QByteArray::fromRawData("\xFF\x03\x04\x00", 4));
}

}
