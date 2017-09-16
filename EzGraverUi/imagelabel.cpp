#include "imagelabel.h"

#include <QPainter>

#include <algorithm>

#include "ezgraver.h"

ImageLabel::ImageLabel(QWidget* parent) : ClickLabel{parent} {
    resetProgressImage();
    connect(&_refreshTimer, &QTimer::timeout, this, &ImageLabel::_updateDisplayedImage);
}

ImageLabel::~ImageLabel() {}

QImage ImageLabel::image() const {
    return _image;
}

void ImageLabel::setImage(QImage const& image) {
    _image = image;
    _updateEngraveImage();
    emit imageLoadedChanged(true);
    emit imageChanged(image);
}

QImage ImageLabel::engraveImage() const {
    return _engraveImage;
}

void ImageLabel::setEngraveImage(QImage const& engraveImage) {
    _engraveImage = engraveImage;
    _updateDisplayedImage();
    emit engraveImageChanged(engraveImage);
}

void ImageLabel::setPixelEngraved(QPoint const& location) {
    if(!_refreshTimer.isActive()) {
        _refreshTimer.start(ImageRefreshIntervalDelay);
    }

    _progressImage.setPixel(location, qRgb(255, 0, 0));
    emit progressImageChanged(_progressImage);
}

QImage ImageLabel::progressImage() const {
    return _progressImage;
}

void ImageLabel::setProgressImage(QImage const& progressImage) {
    _progressImage = progressImage;
    _updateDisplayedImage();
    emit progressImageChanged(progressImage);
}

void ImageLabel::resetProgressImage() {
    QImage image{QSize{Ez::Specifications::ImageWidth, Ez::Specifications::ImageHeight}, QImage::Format_ARGB32};
    image.fill(qRgba(0, 0, 0, 0));
    setProgressImage(image);
}

Qt::ImageConversionFlags ImageLabel::conversionFlags() const {
    return _flags;
}

void ImageLabel::setConversionFlags(Qt::ImageConversionFlags const& flags) {
    _flags = flags;
    _updateEngraveImage();
    emit conversionFlagsChanged(flags);
}

bool ImageLabel::grayscale() const {
    return _grayscale;
}

void ImageLabel::setGrayscale(bool const& enabled) {
    _grayscale = enabled;
    _updateEngraveImage();
    emit grayscaleChanged(enabled);
}

int ImageLabel::layer() const {
    return _layer;
}

void ImageLabel::setLayer(int const& layer) {
    _layer = layer;
    _updateEngraveImage();
    emit layerChanged(layer);
}

int ImageLabel::layerCount() const {
    return _layerCount;
}

void ImageLabel::setLayerCount(int const& layerCount) {
    _layerCount = layerCount;
    _updateEngraveImage();
    emit layerCountChanged(layerCount);
}

bool ImageLabel::keepAspectRatio() const {
    return _keepAspectRatio;
}

void ImageLabel::setKeepAspectRatio(bool const& keepAspectRatio) {
    _keepAspectRatio = keepAspectRatio;
    _updateEngraveImage();
    emit keepAspectRatioChanged(keepAspectRatio);
}

bool ImageLabel::flipHorizontally() const {
    return _flipHorizontally;
}

void ImageLabel::setFlipHorizontally(bool const& flipHorizontally) {
    _flipHorizontally = flipHorizontally;
    _updateEngraveImage();
    emit flipHorizontallyChanged(flipHorizontally);
}

bool ImageLabel::flipVertically() const {
    return _flipVertically;
}

void ImageLabel::setFlipVertically(bool const& flipVertically) {
    _flipVertically = flipVertically;
    _updateEngraveImage();
    emit flipVerticallyChanged(flipVertically);
}

bool ImageLabel::transformed() const {
    return _transformed;
}

void ImageLabel::setTransformed(bool const& transformed) {
    _transformed = transformed;
    _updateEngraveImage();
    emit transformedChanged(transformed);
}

float ImageLabel::imageScale() const {
    return _imageScale;
}

void ImageLabel::setImageScale(float const& imageScale) {
    _imageScale = imageScale;
    _updateEngraveImage();
    emit imageScaleChanged(imageScale);
}

int ImageLabel::imageRotation() const {
    return _imageRotation;
}

void ImageLabel::setImageRotation(int const& imageRotation) {
    _imageRotation = imageRotation;
    _updateEngraveImage();
    emit imageRotationChanged(imageRotation);
}

void ImageLabel::_updateEngraveImage() {
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
        painter.drawImage(QPoint{}, flipped.scaled(image.size()));
    }

    auto targetImage = _grayscale ? _createGrayscaleImage(image) : image.convertToFormat(QImage::Format_Mono, _flags);
    setEngraveImage(targetImage);
}

void ImageLabel::_updateDisplayedImage() {
    QImage image{QSize{Ez::Specifications::ImageWidth, Ez::Specifications::ImageHeight}, QImage::Format_ARGB32};
    QPainter painter{&image};
    painter.drawImage(QPoint{}, _engraveImage);
    painter.drawImage(QPoint{}, _progressImage);
    setPixmap(QPixmap::fromImage(image));
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
