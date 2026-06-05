#include <QApplication>
#include <QWidget>
#include <QThread>

class hvd
{
  private:
    QApplication* app;   // Application
    QWidget* window;     // Application Window
    
    std::thread* applicationThread; 
  public:
    hvd();
    ~hvd();
};
