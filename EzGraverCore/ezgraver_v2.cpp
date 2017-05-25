#include "ezgraver_v2.h"

#include <QDebug>

namespace Ez {

void EzGraverV2::up() {
    qDebug() << "moving up";
    _transmit("\xf5\x01");
}

void EzGraverV2::down() {
    qDebug() << "moving down";
    _transmit("\xf5\x02");
}

void EzGraverV2::left() {
    qDebug() << "moving left";
    _transmit("\xf5\x03");
}

void EzGraverV2::right() {
    qDebug() << "moving right";
    _transmit("\xf5\x04");
}

}
