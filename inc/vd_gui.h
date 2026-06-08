#include <QApplication>
#include <QWidget>
#include <QThread>
#include <QVBoxLayout>

class vd_gui : public QObject
{
  Q_OBJECT

  private:
    QApplication* app;   // Application
    QWidget* window;     // Application Window
    
    QVBoxLayout* rootLayout;

    std::thread* applicationThread; 
  
  signals:
    void dataReceived(QString data);
    void finished();

  public:
    vd_gui();
    ~vd_gui();
};
