#include "clicklabel.h"

#include <QMouseEvent>

ClickLabel::ClickLabel(QWidget* parent) : QLabel(parent) {}
ClickLabel::~ClickLabel() {}

void ClickLabel::mouseReleaseEvent(QMouseEvent*) {
    emit clicked();
}
