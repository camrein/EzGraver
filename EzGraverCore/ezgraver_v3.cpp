#include "ezgraver_v3.h"

#include <QDebug>
#include <QByteArray>

namespace Ez {

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
