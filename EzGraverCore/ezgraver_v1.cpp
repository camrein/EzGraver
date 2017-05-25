#include "ezgraver_v1.h"

#include <QDebug>

namespace Ez {

void EzGraverV1::up() {
    qDebug() << "moving up";
    _transmit(0xF5);
}

void EzGraverV1::down() {
    qDebug() << "moving down";
    _transmit(0xF6);
}

void EzGraverV1::left() {
    qDebug() << "moving left";
    _transmit(0xF7);
}

void EzGraverV1::right() {
    qDebug() << "moving right";
    _transmit(0xF8);
}

}
