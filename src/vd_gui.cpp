#include <QApplication>
#include <QWidget>
#include <QThread>

#include "vd_gui.h"

vd_gui::vd_gui() {
  int argc = 1;
  char appName[] = "app";
  char* argv[] = { appName, nullptr };

  app = new QApplication(argc, argv);
  window = new QWidget();
  window->resize(400, 300);
  window->setWindowTitle("Hello Qt");
  

  QImage image(640, 480, QImage::Format_RGB32);
  image.setPixelColor(100, 50, Qt::red);

  // QThread* thread = new QThread();
  // hvd_icp* worker = new hvd_icp();
  
  // worker->moveToThread(thread);
  
  // QObject::connect(thread, &QThread::started, worker, &hvd_icp::process);
  // QObject::connect(worker, &hvd_icp::dataReceived,
  //   [this](const QString& msg){
    //       window->setWindowTitle(msg);
    //       window->update();
    //   });
    
    // // shutdown chain
    // QObject::connect(app, &QApplication::aboutToQuit, thread, &QThread::quit);
    // QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
    // QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    
    // QObject::connect(app, &QApplication::aboutToQuit, worker, [](){
      //   qDebug() << "App shutting down";
      // });
      
      // thread->start();
      
  window->show();
  app->exec();
  return;
}

vd_gui::~vd_gui()
{
  return;
}

