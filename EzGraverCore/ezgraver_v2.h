#ifndef EZGRAVERV2_H
#define EZGRAVERV2_H

#include <QSerialPort>

#include <memory>

#include "ezgraver.h"

namespace Ez {

struct EzGraverV2 : EzGraver {
    using EzGraver::EzGraver;

    /*! Moves the engraver up. */
    void up();

    /*! Moves the engraver down. */
    void down();

    /*! Moves the engraver left. */
    void left();

    /*! Moves the engraver right. */
    void right();
};

}

#endif // EZGRAVERV2_H
