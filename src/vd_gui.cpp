#include <QApplication>
#include <QWidget>
#include <QThread>
#include <QVBoxLayout>

#include "vd_gui.h"
#include "vd_widgets.h"
#include "vd_icp_driver.h"

vd_gui::vd_gui() {
  int argc = 1;
  char appName[] = "app";
  char* argv[] = { appName, nullptr };

  app = new QApplication(argc, argv);
  window = new QWidget();
  rootLayout = new QVBoxLayout();

  window->resize(400, 300);
  window->setWindowTitle("Hello Qt");
  
  vd_icp_consumer myConsumer;
  myConsumer.init();
  myConsumer.read(LOCK);
  screenWidget myScreen(&myConsumer);
  
  rootLayout->addWidget(&myScreen);
  window->setLayout(rootLayout);
  window->show();
  app->exec();
  myConsumer.read(RELEASE);
  return;
}

vd_gui::~vd_gui()
{
  return;
}



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
      