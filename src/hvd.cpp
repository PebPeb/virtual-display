#include "hvd.h"
#include "hvd_icp.h"


hvd::hvd() {
  int argc = 1;
  char appName[] = "app";
  char* argv[] = { appName, nullptr };

  app = new QApplication(argc, argv);
  window = new QWidget();
  window->resize(400, 300);
  window->setWindowTitle("Hello Qt");
  window->show();

  QThread* thread = new QThread();
  hvd_icp* worker = new hvd_icp();

  worker->moveToThread(thread);

  QObject::connect(thread, &QThread::started, worker, &hvd_icp::process);
  QObject::connect(worker, &hvd_icp::dataReceived,
    [this](const QString& msg){
        window->setWindowTitle(msg);
        window->update();
    });

  // shutdown chain
  QObject::connect(app, &QApplication::aboutToQuit, thread, &QThread::quit);
  QObject::connect(thread, &QThread::finished, worker, &QObject::deleteLater);
  QObject::connect(thread, &QThread::finished, thread, &QThread::deleteLater);

  QObject::connect(app, &QApplication::aboutToQuit, worker, [](){
    qDebug() << "App shutting down";
  });

  thread->start();

  app->exec();
  return;
}

hvd::~hvd()
{
  return;
}

