#include "imagelabel.h"

#include <QPainter>

#include <algorithm>

#include "ezgraver.h"
#include "specifications.h"

ImageLabel::ImageLabel(QWidget* parent) : ClickLabel{parent} {}

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

int ImageLabel::layerCount() const {
    return _layerCount;
}

void ImageLabel::setLayerCount(int const& layerCount) {
    _layerCount = layerCount;
    updateDisplayedImage();
    emit layerCountChanged(layerCount);
}

bool ImageLabel::keepAspectRatio() const {
    return _keepAspectRatio;
}

void ImageLabel::setKeepAspectRatio(bool const& keepAspectRatio) {
    _keepAspectRatio = keepAspectRatio;
    updateDisplayedImage();
    emit keepAspectRatioChanged(keepAspectRatio);
}

bool ImageLabel::flipHorizontally() const {
    return _flipHorizontally;
}

void ImageLabel::setFlipHorizontally(bool const& flipHorizontally) {
    _flipHorizontally = flipHorizontally;
    updateDisplayedImage();
    emit flipHorizontallyChanged(flipHorizontally);
}

bool ImageLabel::flipVertically() const {
    return _flipVertically;
}

void ImageLabel::setFlipVertically(bool const& flipVertically) {
    _flipVertically = flipVertically;
    updateDisplayedImage();
    emit flipVerticallyChanged(flipVertically);
}

bool ImageLabel::transformed() const {
    return _transformed;
}

void ImageLabel::setTransformed(bool const& transformed) {
    _transformed = transformed;
    updateDisplayedImage();
    emit transformedChanged(transformed);
}

float ImageLabel::imageScale() const {
    return _imageScale;
}

void ImageLabel::setImageScale(float const& imageScale) {
    _imageScale = imageScale;
    updateDisplayedImage();
    emit imageScaleChanged(imageScale);
}

int ImageLabel::imageRotation() const {
    return _imageRotation;
}

void ImageLabel::setImageRotation(int const& imageRotation) {
    _imageRotation = imageRotation;
    updateDisplayedImage();
    emit imageRotationChanged(imageRotation);
}

void ImageLabel::updateDisplayedImage() {
    if(!imageLoaded()) {
        return;
    }

    // Draw white background, otherwise transparency is converted to black.
    QImage image{QSize{Ez::Specifications::ImageWidth, Ez::Specifications::ImageHeight}, QImage::Format_ARGB32};
    image.fill(QColor{Qt::white});
    QPainter painter{&image};

    QImage flipped{_image.mirrored(_flipHorizontally, _flipVertically)};

    if(_transformed) {
        QTransform rotation{};
        rotation.rotate(_imageRotation);
        auto rotated = flipped.transformed(rotation);

        auto scaled = rotated.scaled(rotated.width() * _imageScale, rotated.height() * _imageScale);
        QPoint position{(image.width() - scaled.width()) / 2, (image.height() - scaled.height()) / 2};

        painter.drawImage(position, scaled);
    } else if(_keepAspectRatio) {
        // As at this time, the target image is quadratic, scaling according the larger dimension is sufficient.
        auto scaled = (flipped.width() > flipped.height() ? flipped.scaledToWidth(image.width()) : flipped.scaledToHeight(image.height()));
        auto position = (flipped.width() > flipped.height() ? QPoint{0, (image.height() - scaled.height()) / 2} : QPoint{(image.width() - scaled.width()) / 2, 0});
        painter.drawImage(position, scaled);
    } else {
        painter.drawImage(QPoint{0, 0}, flipped.scaled(image.size()));
    }

    auto rendered = _grayscale
            ? QPixmap::fromImage(_createGrayscaleImage(image))
            : QPixmap::fromImage(image.convertToFormat(QImage::Format_Mono, _flags));
    setPixmap(rendered);
}

QImage ImageLabel::_createGrayscaleImage(QImage const& original) const {
    auto colorTable = _createColorTable();
    QImage grayed = original.convertToFormat(QImage::Format_Indexed8, colorTable, _flags);
    if(_layer == 0) {
        return grayed;
    }

    auto visibleLayer = _layer-1;
    int i{0};
    std::transform(colorTable.begin(), colorTable.end(), colorTable.begin(), [&i,visibleLayer](QRgb) {
        return i++ == visibleLayer ? qRgb(0, 0, 0) : qRgb(255, 255, 255);
    });
    grayed.setColorTable(colorTable);

    return grayed.convertToFormat(QImage::Format_Mono, _flags);
}

QVector<QRgb> ImageLabel::_createColorTable() const {
    QVector<QRgb> colorTable(_layerCount - 1);

    int i{0};
    std::generate(colorTable.begin(), colorTable.end(), [this, &i] {
      int gray = (256 / (_layerCount-1)) * (i++);
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
