#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <memory>

#include "ezgraver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(bool imageLoaded READ imageLoaded NOTIFY imageLoadedChanged)

public:
    explicit MainWindow(QWidget* parent = NULL);
    ~MainWindow();

    bool connected() const;
    bool imageLoaded() const;
signals:
    void connectedChanged(bool connected);
    void imageLoadedChanged(bool connected);

private slots:
    void on_connect_clicked();
    void on_home_clicked();
    void on_up_clicked();
    void on_left_clicked();
    void on_center_clicked();
    void on_right_clicked();
    void on_down_clicked();
    void on_upload_clicked();
    void on_preview_clicked();
    void on_start_clicked();
    void on_pause_clicked();
    void on_reset_clicked();
    void on_disconnect_clicked();
    void on_image_clicked();

    void updatePorts();

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private:
    Ui::MainWindow* _ui;
    QTimer _portTimer;

    std::shared_ptr<EzGraver> _ezGraver;
    bool _connected;
    bool _imageLoaded;

    void setupBindings();
    void setConnected(bool connected);
    void setImageLoaded(bool imageLoaded);
    void printVerbose(QString const& verbose);
    void loadImage(QString const& fileName);
};

#endif // MAINWINDOW_H
