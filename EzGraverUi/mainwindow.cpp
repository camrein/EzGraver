#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QMimeData>
#include <QFileDialog>
#include <QBitmap>
#include <QIcon>
#include <QThreadPool>
#include <QDebug>
#include <QImageReader>

#include <stdexcept>
#include <algorithm>
#include <iterator>

#include "factory.h"
#include "specifications.h"

MainWindow::MainWindow(QWidget* parent)
        :  QMainWindow{parent}, _ui{new Ui::MainWindow},
          _portTimer{}, _image{}, _settings{"EzGraver", "EzGraver"}, _ezGraver{}, _bytesWrittenProcessor{[](qint64){}}, _connected{false} {
    _ui->setupUi(this);
    setAcceptDrops(true);

    connect(&_portTimer, &QTimer::timeout, this, &MainWindow::updatePorts);
    _portTimer.start(PortUpdateDelay);

    _initBindings();
    _initConversionFlags();
    _initProtocols();
    _setConnected(false);

    _ui->image->setImageDimensions(QSize{Ez::Specifications::ImageWidth, Ez::Specifications::ImageHeight});
}

MainWindow::~MainWindow() {
    delete _ui;
}

void MainWindow::_initBindings() {
    connect(_ui->burnTime, &QSlider::valueChanged, [this](int const& v) { _ui->burnTimeLabel->setText(QString::number(v)); });

    connect(this, &MainWindow::connectedChanged, _ui->ports, &QComboBox::setDisabled);
    connect(this, &MainWindow::connectedChanged, _ui->connect, &QPushButton::setDisabled);
    connect(this, &MainWindow::connectedChanged, _ui->disconnect, &QPushButton::setEnabled);

    connect(this, &MainWindow::connectedChanged, _ui->home, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->up, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->left, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->center, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->right, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->down, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->preview, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->start, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->pause, &QPushButton::setEnabled);
    connect(this, &MainWindow::connectedChanged, _ui->reset, &QPushButton::setEnabled);
    connect(_ui->conversionFlags, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int const& index) {
        _ui->image->setConversionFlags(static_cast<Qt::ImageConversionFlags>(_ui->conversionFlags->itemData(index).toInt()));
    });

    connect(_ui->layered, &QCheckBox::toggled, _ui->selectedLayer, &QSpinBox::setEnabled);
    connect(_ui->layered, &QCheckBox::toggled, _ui->layerCount, &QSpinBox::setEnabled);
    connect(_ui->layered, &QCheckBox::toggled, _ui->image, &ImageLabel::setGrayscale);
    connect(_ui->layerCount, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), _ui->image, &ImageLabel::setLayerCount);
    connect(_ui->layerCount, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), _ui->selectedLayer, &QSpinBox::setMaximum);
    connect(_ui->selectedLayer, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), _ui->image, &ImageLabel::setLayer);

    auto uploadEnabled = [this] {
        _ui->upload->setEnabled(_ui->image->imageLoaded() && _connected && (!_ui->layered->isChecked() || _ui->selectedLayer->value() > 0));
    };
    connect(this, &MainWindow::connectedChanged, uploadEnabled);
    connect(_ui->image, &ImageLabel::imageLoadedChanged, uploadEnabled);
    connect(_ui->selectedLayer, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), uploadEnabled);
    connect(_ui->layered, &QCheckBox::toggled, uploadEnabled);

    connect(_ui->keepAspectRatio, &QCheckBox::toggled, _ui->image, &ImageLabel::setKeepAspectRatio);

    connect(_ui->scaled, &QCheckBox::toggled, _ui->imageScale, &QSlider::setEnabled);
    connect(_ui->scaled, &QCheckBox::toggled, _ui->resetImageScale, &QSlider::setEnabled);
    connect(_ui->scaled, &QCheckBox::toggled, _ui->image, &ImageLabel::setScaled);
    connect(_ui->imageScale, &QSlider::valueChanged, [this](int const& v) { _ui->imageScaleLabel->setText(QString::number(v)); });
    connect(_ui->imageScale, &QSlider::valueChanged, [this](int const& v) { _ui->image->setImageScale(v / 100.0); });
    connect(_ui->resetImageScale, &QPushButton::clicked, [this] { _ui->imageScale->setValue(100); });

    connect(this, &MainWindow::connectedChanged, _ui->protocolVersion, &QComboBox::setDisabled);
}

void MainWindow::_initConversionFlags() {
    _ui->conversionFlags->addItem("DiffuseDither", Qt::DiffuseDither);
    _ui->conversionFlags->addItem("OrderedDither", Qt::OrderedDither);
    _ui->conversionFlags->addItem("ThresholdDither", Qt::ThresholdDither);
    _ui->conversionFlags->setCurrentIndex(0);
}

void MainWindow::_initProtocols() {
    auto protocols = Ez::protocols();
    for(auto protocol : protocols) {
        _ui->protocolVersion->addItem(QString{"v%1"}.arg(protocol), protocol);
    }

    auto selectedProtocol = _settings.value("protocol", 1).toInt();
    if(protocols.contains(selectedProtocol)) {
        _ui->protocolVersion->setCurrentText(QString{"v%1"}.arg(selectedProtocol));
    }
}

void MainWindow::_printVerbose(QString const& verbose) {
    _ui->verbose->appendPlainText(verbose);
}

void MainWindow::updatePorts() {
    QStringList ports{Ez::availablePorts()};
    ports.insert(0, "");

    QString original{_ui->ports->currentText()};
    _ui->ports->clear();
    _ui->ports->addItems(ports);

    if(ports.contains(original)) {
        _ui->ports->setCurrentText(original);
    }
}

void MainWindow::_loadImage(QString const& fileName) {
    _printVerbose(QString{"loading image: %1"}.arg(fileName));

    QImage image{};
    if(!image.load(fileName)) {
        _printVerbose("failed to load image");
        return;
    }

    _ui->image->setImage(image);
}

bool MainWindow::connected() const {
    return _connected;
}

void MainWindow::_setConnected(bool connected) {
    _connected = connected;
    emit connectedChanged(connected);
}

void MainWindow::bytesWritten(qint64 bytes) {
    _bytesWrittenProcessor(bytes);
}

void MainWindow::updateProgress(qint64 bytes) {
    qDebug() << "Bytes written:" << bytes;
    auto progress = _ui->progress->value() + bytes;
    _ui->progress->setValue(progress);
    if(progress >= _ui->progress->maximum()) {
        _printVerbose("upload completed");
        _bytesWrittenProcessor = [](qint64){};
    }
}

void MainWindow::on_connect_clicked() {
    try {
        auto protocol = _ui->protocolVersion->currentData().toInt();
        _printVerbose(QString{"connecting to port %1 with protocol version %2"}.arg(_ui->ports->currentText()).arg(protocol));
        _ezGraver = Ez::create(_ui->ports->currentText(), protocol);
        _printVerbose("connection established successfully");
        _setConnected(true);

        _settings.setValue("protocol", protocol);

        connect(_ezGraver->serialPort().get(), &QSerialPort::bytesWritten, this, &MainWindow::bytesWritten);
    } catch(std::exception const& e) {
        _printVerbose(QString{"Error: %1"}.arg(e.what()));
    }
}

void MainWindow::on_home_clicked() {
    _printVerbose("moving to home");
    _ezGraver->home();
}

void MainWindow::on_up_clicked() {
    _ezGraver->up();
}

void MainWindow::on_left_clicked() {
    _ezGraver->left();
}

void MainWindow::on_center_clicked() {
    _printVerbose("moving to center");
    _ezGraver->center();
}

void MainWindow::on_right_clicked() {
    _ezGraver->right();
}

void MainWindow::on_down_clicked() {
    _ezGraver->down();
}

void MainWindow::on_upload_clicked() {
    _printVerbose("erasing EEPROM");
    _ezGraver->erase();

    QImage image{_ui->image->pixmap()->toImage()};
    QTimer* eraseProgressTimer{new QTimer{this}};
    _ui->progress->setValue(0);
    _ui->progress->setMaximum(Ez::Specifications::EraseTimeMs);

    auto eraseProgress = std::bind(&MainWindow::_eraseProgressed, this, eraseProgressTimer, image);
    connect(eraseProgressTimer, &QTimer::timeout, eraseProgress);
    eraseProgressTimer->start(EraseProgressDelay);
}

void MainWindow::_eraseProgressed(QTimer* eraseProgressTimer, QImage const& image) {
    auto value = _ui->progress->value() + EraseProgressDelay;
    _ui->progress->setValue(value);
    if(value < Ez::Specifications::EraseTimeMs) {
        return;
    }
    eraseProgressTimer->stop();

    _uploadImage(image);
}

void MainWindow::_uploadImage(QImage const& image) {
    _bytesWrittenProcessor = std::bind(&MainWindow::updateProgress, this, std::placeholders::_1);
    _printVerbose("uploading image to EEPROM");
    auto bytes = _ezGraver->uploadImage(image);
    _ui->progress->setValue(0);
    _ui->progress->setMaximum(bytes);
}

void MainWindow::on_preview_clicked() {
    _printVerbose("drawing preview");
    _ezGraver->preview();
}

void MainWindow::on_start_clicked() {
    _printVerbose(QString{"starting engrave process with burn time %1"}.arg(_ui->burnTime->value()));
    _ezGraver->start(_ui->burnTime->value());
}

void MainWindow::on_pause_clicked() {
    _printVerbose("pausing engrave process");
    _ezGraver->pause();
}

void MainWindow::on_reset_clicked() {
    _printVerbose("resetting engraver");
    _ezGraver->reset();
}

void MainWindow::on_disconnect_clicked() {
    _printVerbose("disconnecting");
    _setConnected(false);
    _ezGraver.reset();
    _printVerbose("disconnected");
}

void MainWindow::on_image_clicked() {
    auto supportedFormats = QImageReader::supportedImageFormats();
    QStringList fileExtensions{};
    std::transform(supportedFormats.cbegin(), supportedFormats.cend(), std::back_inserter(fileExtensions), [](QByteArray const& format) {
        return "*." + format;
    });

    qDebug() << "supported file extensions:" << fileExtensions;

    auto fileName = QFileDialog::getOpenFileName(this, "Open Image", "", QString{"Images (%1)"}.arg(fileExtensions.join(" ")));
    if(!fileName.isNull()) {
        _loadImage(fileName);
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    if(event->mimeData()->hasUrls() && event->mimeData()->urls().count() == 1) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event) {
    QString fileName{event->mimeData()->urls()[0].toLocalFile()};
    _loadImage(fileName);
}

