#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include "clicklabel.h"

class ImageLabel : public ClickLabel {
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(QImage engraveImage READ engraveImage WRITE setEngraveImage NOTIFY engraveImageChanged)
    Q_PROPERTY(QImage progressImage READ progressImage WRITE setProgressImage NOTIFY progressImageChanged)

    Q_PROPERTY(Qt::ImageConversionFlags conversionFlags READ conversionFlags WRITE setConversionFlags NOTIFY conversionFlagsChanged)

    Q_PROPERTY(bool grayscale READ grayscale WRITE setGrayscale NOTIFY grayscaleChanged)
    Q_PROPERTY(int layer READ layer WRITE setLayer NOTIFY layerChanged)
    Q_PROPERTY(int layerCount READ layerCount WRITE setLayerCount NOTIFY layerCountChanged)

    Q_PROPERTY(bool keepAspectRatio READ keepAspectRatio WRITE setKeepAspectRatio NOTIFY keepAspectRatioChanged)
    Q_PROPERTY(bool flipHorizontally READ flipHorizontally WRITE setFlipHorizontally NOTIFY flipHorizontallyChanged)
    Q_PROPERTY(bool flipVertically READ flipVertically WRITE setFlipVertically NOTIFY flipVerticallyChanged)

    Q_PROPERTY(bool transformed READ transformed WRITE setTransformed NOTIFY transformedChanged)
    Q_PROPERTY(float imageScale READ imageScale WRITE setImageScale NOTIFY imageScaleChanged)
    Q_PROPERTY(float imageRotation READ imageRotation WRITE setImageRotation NOTIFY imageRotationChanged)

    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)

public:
    /*!
     * Creates a new instance with the given \a parent.
     *
     * \param parent The parent of the label.
     */
    explicit ImageLabel(QWidget* parent=NULL);

    /*!
     * Frees all required resources upon deconstruction.
     */
    virtual ~ImageLabel();

    /*!
     * Gets the currently loaded image without the application of
     * any conversion.
     *
     * \return The currently loaded image.
     */
    QImage image() const;

    /*!
     * Changes the currently displayed image to the given image
     * and applies the selected conversion method.
     *
     * \param image The image to load.
     */
    void setImage(QImage const& image);

    /*!
     * Gets the currently active engraving image.
     *
     * \return The current engraving image.
     */
    QImage engraveImage() const;

    /*!
     * Changes the currently active engraving image.
     *
     * \param engravePixmap The image that is used for engraving.
     */
    void setEngraveImage(QImage const& engraveImage);

    /*!
     * Gets the image that represents the current engraving progress.
     *
     * \return The image representing the current progress.
     */
    QImage progressImage() const;

    /*!
     * Gets the image that represents the current engraving progress.
     *
     * \param progressImage An image that represents the current engraving progress.
     */
    void setProgressImage(QImage const& progressImage);

    /*!
     * Gets the currently selected conversion flags.
     *
     * \return The currently selected conversion flags.
     */
    Qt::ImageConversionFlags conversionFlags() const;

    /*!
     * Changes the applied conversion flags to the given one and
     * updates the currently displayed image.
     *
     * \param flags The conversion flags to use.
     */
    void setConversionFlags(Qt::ImageConversionFlags const& flags);

    /*!
     * Gets if grayscale is enabled.
     *
     * \return \c true if grayscale is enabled.
     */
    bool grayscale() const;

    /*!
     * Enables/disables the grayscale setting of the image.
     *
     * \param enabled \c true if the image should be handled as grayscale.
     */
    void setGrayscale(bool const& enabled);

    /*!
     * Gets the currently selected grayscale layer.
     *
     * \return The currently selected grayscale layer.
     */
    int layer() const;

    /*!
     * Changes the displayed grayscale layer to the provided one.
     *
     * \param layer The layer to display.
     */
    void setLayer(int const& layer);

    /*!
     * Gets the currently used number of layers.
     *
     * \return The currently user number of layers.
     */
    int layerCount() const;

    /*!
     * Changes the used number of layers.
     *
     * \param layer The number of layers.
     */
    void setLayerCount(int const& layerCount);

    /*!
     * Gets the if the aspect ratio of the image should be kept.
     *
     * \return Returns \c true if the aspect ratio should be kept.
     */
    bool keepAspectRatio() const;

    /*!
     * Sets the the aspect ratio of the image should be kept.
     *
     * \param keepAspectRatio \c true if the aspect ratio should be kept.
     */
    void setKeepAspectRatio(bool const& keepAspectRatio);

    /*!
     * Gets the if image should be flipped horizontally.
     *
     * \return Returns \c true if the image should be flipped horizontally.
     */
    bool flipHorizontally() const;

    /*!
     * Sets the the the image should be flipped horizontally.
     *
     * \param flipHorizontally \c true if the should be flipped horizontally.
     */
    void setFlipHorizontally(bool const& flipHorizontally);

    /*!
     * Gets the if image should be flipped vertically.
     *
     * \return Returns \c true if the image should be flipped vertically.
     */
    bool flipVertically() const;

    /*!
     * Sets the the the image should be flipped vertically.
     *
     * \param flipHorizontally \c true if the should be flipped vertically.
     */
    void setFlipVertically(bool const& flipVertically);

    /*!
     * Gets if the image is being transformed.
     *
     * \return Returns \c true if the image is being transformed.
     */
    bool transformed() const;

    /*!
     * Sets if the image is being transformed.
     *
     * \param transformed \c true if the image is being transformed.
     */
    void setTransformed(bool const& transformed);

    /*!
     * Gets the current image scale.
     *
     * \return The image scale. 1.0 equals to 100% (original size).
     */
    float imageScale() const;

    /*!
     * Sets the image scale of the image.
     *
     * \param imageScale The image scale. 1.0 equals to 100% (original size).
     */
    void setImageScale(float const& imageScale);

    /*!
     * Gets the current image rotation.
     *
     * \return The image rotation in degrees.
     */
    int imageRotation() const;

    /*!
     * Sets the image rotation of the image.
     *
     * \param imageRotation The image rotation in degrees.
     */
    void setImageRotation(int const& imageRotation);

    /*!
     * Gets if an image has been loaded.
     *
     * \return Returns \c true if an image is loaded.
     */
    bool imageLoaded() const;

    /*!
     * Sets the image dimensions. This enforces minimum dimensions of the
     * component with respect to the border width.
     *
     * \param dimensions The image dimensions.
     */
    void setImageDimensions(QSize const& dimensions);
signals:
    /*!
     * Fired as soon as the image has been changed.
     *
     * \param image The new image in its original state (unconverted).
     */
    void imageChanged(QImage const& image);

    /*!
     * Fired as soon as the engraving pixmap changed.
     *
     * \param engravePixmap The pixmap representing the current image to engrave.
     */
    void engraveImageChanged(QImage const& engraveImage);

    /*!
     * Fired as soon as the progress image changed.
     *
     * \param progressImage The image representing the current engraving progress.
     */
    void progressImageChanged(QImage const& progressImage);

    /*!
     * Fired as soon as the conversion flags hav been changed.
     *
     * \param flags The newly applied conversion flags.
     */
    void conversionFlagsChanged(Qt::ImageConversionFlags const& flags);

    /*!
     * Fired as soon as grayscale has been enabled or disabled.
     *
     * \param enabled \c true if grayscale has been enabled.
     */
    void grayscaleChanged(bool const& enabled);

    /*!
     * Fired as soon as the layer of the grayscale image was changed.
     *
     * \param layer The currently selected layer.
     */
    void layerChanged(int const& layer);

    /*!
     * Fired as soon as the layer the number of layers changed.
     *
     * \param layerCount The currently active number of layers.
     */
    void layerCountChanged(int const& layerCount);

    /*!
     * Fired as soon as the keep aspect ratio setting flag changed.
     *
     * \param keepAspectRatio The currently active setting of the aspect ratio.
     */
    void keepAspectRatioChanged(bool const& keepAspectRatio);

    /*!
     * Fired as soon as the image is flipped horizontally.
     *
     * \param flipHorizontally \c true if the image is flipped horizontally.
     */
    void flipHorizontallyChanged(bool const& flipHorizontally);

    /*!
     * Fired as soon as the image is flipped vertically.
     *
     * \param flipVertically \c true if the image is flipped vertically.
     */
    void flipVerticallyChanged(bool const& flipVertically);

    /*!
     * Fired as soon as the image is being transformed.
     *
     * \param transformed \c true if the image is being transformed.
     */
    void transformedChanged(bool const& transformed);

    /*!
     * Fired as soon as the image scale changed.
     *
     * \param imageScale The current image scale. 1.0 equals to 100%.
     */
    void imageScaleChanged(float const& imageScale);

    /*!
     * Fired as soon as the image rotation changed.
     *
     * \param imageRotation The current image rotation in degrees.
     */
    void imageRotationChanged(int const& imageRotation);

    /*!
     * Fired as soon as an image has been loaded.
     *
     * \param imageLoaded \c true if an image is loaded.
     */
    void imageLoadedChanged(bool imageLoaded);
private:
    QImage _image{};
    QImage _engraveImage{};
    QImage _progressImage{};

    Qt::ImageConversionFlags _flags{Qt::DiffuseDither};
    bool _grayscale{false};
    int _layer{0};
    int _layerCount{3};

    bool _keepAspectRatio{false};
    bool _flipHorizontally{false};
    bool _flipVertically{false};

    bool _transformed{false};
    float _imageScale{1.0};
    int _imageRotation{0};

    void updateDisplayedImage();
    QImage _createGrayscaleImage(QImage const& original) const;
    QVector<QRgb> _createColorTable() const;
};

#endif // IMAGELABEL_H
