#include <QWidget>
#include <QImage>
#include <QPainter>

#include "vd_icp_driver.h"

class screenWidget : public QWidget
{
  public:
    screenWidget(vd_icp_consumer* consumer) : image (
      (uchar*) consumer->imageData,
      consumer->shmPtr->pixelW,
      consumer->shmPtr->pixelH,
      consumer->shmPtr->pixelW * sizeof(typeof(*consumer->imageData)),
      QImage::Format_ARGB32
    ) {};

    void setImage();

  protected:
    void paintEvent(QPaintEvent*); 

  private:
    QImage image;
};