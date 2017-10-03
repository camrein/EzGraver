#ifndef EZGRAVERV3_H
#define EZGRAVERV3_H

#include <QSerialPort>

#include <memory>

#include "ezgraver.h"

namespace Ez {

struct EzGraverV3 : EzGraver {
    using EzGraver::EzGraver;

    /*! Moves the engraver up. */
    void up() override;

    /*! Moves the engraver down. */
    void down() override;

    /*! Moves the engraver left. */
    void left() override;

    /*! Moves the engraver right. */
    void right() override;
};

}

#endif // EZGRAVERV3_H
