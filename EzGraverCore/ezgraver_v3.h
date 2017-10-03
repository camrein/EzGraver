#ifndef EZGRAVERV3_H
#define EZGRAVERV3_H

#include <QSerialPort>

#include <memory>

#include "ezgraver.h"

namespace Ez {

struct EzGraverV3 : EzGraver {
    using EzGraver::EzGraver;

    /*!
     * Starts the engraving process with the given \a burnTime.
     *
     * \param burnTime The burn time to use in milliseconds.
     */
    void start(unsigned char const& burnTime);

    /*!
     * Pauses the engraving process at the given location. The process
     * can be continued by invoking start.
     */
    void pause() override;

    /*! Resets the engraver. */
    void reset() override;

    /*! Moves the engraver to the home position. */
    void home() override;

    /*! Moves the engraver to the center. */
    void center() override;

    /*! Draws a preview of the currently loaded image. */
    void preview() override;

    /*! Moves the engraver up. */
    void up() override;

    /*! Moves the engraver down. */
    void down() override;

    /*! Moves the engraver left. */
    void left() override;

    /*! Moves the engraver right. */
    void right() override;

private:
    void _setBurnTime(unsigned char const& burnTime);
};

}

#endif // EZGRAVERV3_H
