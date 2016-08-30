#include "imagelabel.h"

ImageLabel::ImageLabel(QWidget* parent) : ClickLabel{parent}, _image{}, _flags{Qt::AutoColor} {}
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
    QImage image = _image.scaled(512, 512).convertToFormat(QImage::Format_Mono, _flags);
    setPixmap(QPixmap::fromImage(image));
}

bool ImageLabel::imageLoaded() const {
    return !_image.isNull();
}
