#ifndef EZGRAVERV1_H
#define EZGRAVERV1_H

#include <QSerialPort>

#include <memory>

#include "protocol.h"
#include "ezgraver.h"

namespace Ez {

struct EzGraverV1 : EzGraver {
    /*!
     * Creates an instance of the EzGraver.
     *
     * \param serial The serial port to use.
     */
    explicit EzGraverV1(std::shared_ptr<QSerialPort> serial);

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

#endif // EZGRAVERV1_H
