#include <QWidget>
#include <QImage>
#include <QPainter>

#include "vd_widgets.h"

void screenWidget::setImage() {
  update();
}

void screenWidget::paintEvent(QPaintEvent*) {
  // image.fill(Qt::red);
  QPainter painter(this);
  painter.drawImage(0, 0, image);
}
