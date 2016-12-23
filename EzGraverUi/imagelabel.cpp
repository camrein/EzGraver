#include "imagelabel.h"

#include <QPainter>

#include <algorithm>

#include "ezgraver.h"

ImageLabel::ImageLabel(QWidget* parent) : ClickLabel{parent}, _image{}, _flags{Qt::DiffuseDither},
    _grayscale{false}, _layer{0} {}

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

bool ImageLabel::grayscale() const {
    return _grayscale;
}

void ImageLabel::setGrayscale(bool const& enabled) {
    _grayscale = enabled;
    updateDisplayedImage();
    emit grayscaleChanged(enabled);
}

int ImageLabel::layer() const {
    return _layer;
}

void ImageLabel::setLayer(int const& layer) {
    _layer = layer;
    updateDisplayedImage();
    emit layerChanged(layer);
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

    auto rendered = _grayscale
            ? QPixmap::fromImage(image.convertToFormat(QImage::Format_Indexed8, _colorTable(), _flags))
            : QPixmap::fromImage(image.convertToFormat(QImage::Format_Mono, _flags));
    setPixmap(rendered);
}

QVector<QRgb> ImageLabel::_colorTable() {
    QVector<QRgb> colorTable(MaxGrayscaleLayers - 1);

    int i{0};
    std::generate(colorTable.begin(), colorTable.end(), [&i] {
      int gray = (256 / (MaxGrayscaleLayers-1)) * (i++);
      return qRgb(gray, gray, gray);
    });
    colorTable.push_back(qRgb(255, 255, 255));

    return colorTable;
}

bool ImageLabel::imageLoaded() const {
    return !_image.isNull();
}

void ImageLabel::setImageDimensions(QSize const& dimensions) {
    auto span = this->lineWidth()*2;
    setMinimumWidth(dimensions.width() + span);
    setMinimumHeight(dimensions.height() + span);
}
