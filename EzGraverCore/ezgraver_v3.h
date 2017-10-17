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

    /*!
     * Erases the EEPROM of the engraver. This is necessary before uploading
     * any new image to it.
     * Erasing the EEPROM takes a while. Sending image data to early causes
     * that some of the leading pixels are lost. Waiting for about 5 seconds
     * seems to be sufficient.
     *
     * \return The recommended time in ms to wait until uploading the image.
     */
    int erase() override;

    /*!
     * Uploads the given \a image to the EEPROM. It is mandatory to use \a erase()
     * it prior uploading an image. The image will automatically be scaled, inverted,
     * mirrored and converted to a monochrome bitmap.
     *
     * \param image The image to upload to the EEPROM for engraving.
     * \return The number of bytes being sent to the device.
     */
    int uploadImage(QImage const& image) override;

private:
    void _setBurnTime(unsigned char const& burnTime);
};

}

#endif // EZGRAVERV3_H
