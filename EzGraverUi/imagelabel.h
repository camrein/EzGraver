#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include "clicklabel.h"

class ImageLabel : public ClickLabel {
    Q_OBJECT
    Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(Qt::ImageConversionFlags conversionFlags READ conversionFlags WRITE setConversionFlags NOTIFY conversionFlagsChanged)
    Q_PROPERTY(bool grayscale READ grayscale WRITE setGrayscale NOTIFY grayscaleChanged)
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)

public:
    /*! The maximum number of grayscale layers. */
    static int const MaxGrayscaleLayers{8};

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
     * Fired as soon as an image has been loaded.
     *
     * \param imageLoaded \c true if an image is loaded.
     */
    void imageLoadedChanged(bool imageLoaded);
private:
    QImage _image;
    Qt::ImageConversionFlags _flags;
    bool _grayscale;
    int _layer;

    void updateDisplayedImage();
    QImage _createGrayscaleImage(QImage const& original) const;
    QVector<QRgb> _createColorTable() const;
};

#endif // IMAGELABEL_H
