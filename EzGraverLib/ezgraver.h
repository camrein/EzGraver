#ifndef EZGRAVERLIB_H
#define EZGRAVERLIB_H

#include "ezgraverlib_global.h"

#include <QStringList>
#include <QImage>

#include <memory>

QT_BEGIN_NAMESPACE
class QSerialPort;
QT_END_NAMESPACE

/*!
 * Allows accessing a NEJE engraver using the serial port it was instantiated with.
 * The connection is closed as soon as the object is destroyed.
 */
struct EZGRAVERLIBSHARED_EXPORT EzGraver {
    /*!
     * Creates an instance and connects to the given \a portName.
     *
     * \param portName The port the connection should be established to.
     * \return An instance of the EzGraver as a shared pointer.
     */
    static std::shared_ptr<EzGraver> create(QString const& portName);

    /*!
     * Gets a list of all available ports.
     *
     * \return A list with all ports.
     */
    static QStringList availablePorts();

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
    void pause();

    /*! Resets the engraver. */
    void reset();

    /*! Moves the engraver to the home position. */
    void home();

    /*! Moves the engraver to the center. */
    void center();

    /*! Draws a preview of the currently loaded image. */
    void preview();

    /*! Moves the engraver up. */
    void up();

    /*! Moves the engraver down. */
    void down();

    /*! Moves the engraver left. */
    void left();

    /*! Moves the engraver right. */
    void right();

    /*!
     * Erases the EEPROM of the engraver. This is necessary before uploading
     * any new image to it.
     * Erasing the EEPROM takes a while. Sending image data to early causes
     * that some of the leading pixels are lost. Waiting for about 5 seconds
     * seems to be sufficient.
     */
    void erase();

    /*!
     * Uploads the given \a image to the EEPROM. It is mandatory to use \a erase()
     * it prior uploading an image. The image will automatically be scaled, inverted,
     * mirrored and converted to a monochrome bitmap.
     *
     * \param image The image to upload to the EEPROM for engraving.
     */
    void uploadImage(QImage const& image);

    /*!
     * Uploads any given \a image byte array to the EEPROM. It has to be a monochrome
     * bitmap of the dimensions 512x512. Every white pixel is being engraved.
     *
     * \param image The image byte array to upload to the EEPROM.
     */
    void uploadImage(QByteArray const& image);

    /*!
     * Waits until the current serial port buffer is fully written to the device.
     *
     * \param msecs The time in milliseconds to await the transmission to complete.
     */
    void awaitTransmission(int msecs=-1);

    EzGraver() = delete;
    virtual ~EzGraver();

private:

    std::shared_ptr<QSerialPort> _serial;
    explicit EzGraver(std::shared_ptr<QSerialPort> serial);

    void _transmit(unsigned char const& data);
    void _transmit(QByteArray const& data);

    void _setBurnTime(unsigned char const& burnTime);
};

#endif // EZGRAVERLIB_H
