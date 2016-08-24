#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QtWidgets/QLabel>

struct ImageLabel : public QLabel {
    ImageLabel();

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // IMAGELABEL_H
