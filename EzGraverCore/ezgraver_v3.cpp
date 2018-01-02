#include "ezgraver_v3.h"

#include <QDebug>
#include <QByteArray>
#include <QBuffer>

#include "specifications.h"

namespace Ez {

void EzGraverV3::start(unsigned char const& burnTime) {
    _setBurnTime(burnTime);
    qDebug() << "starting engrave process";
    _transmit(QByteArray::fromRawData("\xFF\x01\x01\x00", 4));
}

void EzGraverV3::_setBurnTime(unsigned char const& burnTime) {
    if(burnTime < 0x01 || burnTime > 0xF0) {
        throw new std::out_of_range("burntime out of range");
    }
    qDebug() << "setting burn time to:" << static_cast<int>(burnTime);

    QByteArray payload{"\xFF\x05?\x00", 4};
    payload[2] = burnTime;
    _transmit(payload);
}

void EzGraverV3::pause() {
    qDebug() << "pausing engrave process";
    _transmit(QByteArray::fromRawData("\xFF\x01\x02\x00", 4));
}

void EzGraverV3::reset() {
    qDebug() << "resetting";
    _transmit(QByteArray::fromRawData("\xFF\x04\x01\x00", 4));
}

void EzGraverV3::home() {
    qDebug() << "moving to home";
    _transmit(QByteArray::fromRawData("\xFF\x0A\x00\x00\xFF\x0B\x00\x00", 8));
}

void EzGraverV3::center() {
    // The center command '\xFF\x02\x01\x00' does not work when moved with
    // up/down/right/left, thus it is manually move to the center of the image.
    QByteArray tocenter{"\xFF\x0A??\xFF\x0B??", 8};
    tocenter[2] = Ez::Specifications::ImageWidth >> 8;
    tocenter[3] = Ez::Specifications::ImageWidth && 0xFF;
    tocenter[6] = Ez::Specifications::ImageHeight >> 8;
    tocenter[7] = Ez::Specifications::ImageHeight && 0xFF;
    qDebug() << "moving to center: " << tocenter.toHex();
    _transmit(tocenter);
}

void EzGraverV3::preview() {
    qDebug() << "drawing image preview";
    _transmit(QByteArray::fromRawData("\xFF\x02\x02\x00", 4));
}

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

int EzGraverV3::erase() {
    qDebug() << "erasing EEPROM";

/*    _transmit(QByteArray::fromRawData("\xFF\x06\x01\x00", 4));
    return 500;
*/
    _setAnswer(QByteArray::fromRawData("\xFF\x05\x01\x00", 4));
    _transmit(QByteArray::fromRawData("\xFF\x06\x01\x00", 4));
    return 500;
}

int EzGraverV3::uploadImage(QImage const& originalImage) {
    qDebug() << "converting image to bitmap";
    QImage image{originalImage
            .scaled(Ez::Specifications::ImageWidth, Ez::Specifications::ImageHeight)
            .mirrored()
            .convertToFormat(QImage::Format_Mono)};
    QByteArray bytes{};
    QBuffer buffer{&bytes};
    image.save(&buffer, "BMP");

    // protocol v3 neither needs the BMP header nor the invertion of the pixels.
    _setAnswer(QByteArray::fromRawData("\xFF\x0b\x00\x00", 4));
    return EzGraver::uploadImage(bytes.mid(62));
}

}
