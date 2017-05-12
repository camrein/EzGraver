#ifndef EZGRAVERV2_H
#define EZGRAVERV2_H

#include <QSerialPort>

#include <memory>

#include "ezgraver.h"

namespace Ez {

struct EzGraverV2 : EzGraver {
    /*!
     * Creates an instance of the EzGraver with protocol version 2.
     *
     * \param serial The serial port to use.
     */
    explicit EzGraverV2(std::shared_ptr<QSerialPort> serial);

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
