#include "imagelabel.h"

#include <QPainter>

#include "EZGraver.h"

ImageLabel::ImageLabel(QWidget* parent) : ClickLabel{parent}, _image{}, _flags{Qt::DiffuseDither} {}
ImageLabel::~ImageLabel() {}

QImage ImageLabel::image() const {
    return _image;
}

void ImageLabel::setImage(QImage const& image) {
    _image = image;
    updateDisplayedImage();
    emit imageLoadedChanged(true);
    emit imageChanged(image);
}

Qt::ImageConversionFlags ImageLabel::conversionFlags() const {
    return _flags;
}

void ImageLabel::setConversionFlags(Qt::ImageConversionFlags const& flags) {
    _flags = flags;
    updateDisplayedImage();
    emit conversionFlagsChanged(flags);
}

void ImageLabel::updateDisplayedImage() {
    if(!imageLoaded()) {
        return;
    }

    // Draw white background, otherwise transparency is converted to black.
    QImage image{QSize{EzGraver::ImageWidth, EzGraver::ImageHeight}, QImage::Format_ARGB32};
    image.fill(QColor{Qt::white});
    QPainter painter{&image};
    painter.drawImage(0, 0, _image.scaled(image.size()));

    setPixmap(QPixmap::fromImage(image.convertToFormat(QImage::Format_Mono, _flags)));
}

bool ImageLabel::imageLoaded() const {
    return !_image.isNull();
}

#include <QDebug>
void ImageLabel::setImageDimensions(QSize const& dimensions) {
    auto span = this->lineWidth()*2;
    qDebug() << "span:" << span;
    setMinimumWidth(dimensions.width() + span);
    setMinimumHeight(dimensions.height() + span);
}
