#include <QObject>
#include <QThread>
#include <QString>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <string>

#include "shmem.h"

// icp -> Inter-process Communication
class hvd_icp : public QObject
{
  Q_OBJECT

  public:
    explicit hvd_icp() {}
    ~hvd_icp() { shm_unlink(BackingFile); }


  signals:
    void dataReceived(QString data);
    void finished();

  public slots:
    void process()
    {
      // https://man7.org/linux/man-pages/man3/shm_open.3.html
      int fd = shm_open(BackingFile, O_CREAT | O_RDWR, AccessPerms);
      if (fd < 0) {
        emit dataReceived("shm_open failed");
        emit finished();
        return;
      }

      caddr_t memptr = (caddr_t)mmap(
        nullptr,
        ByteSize,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
      );
      
      if (memptr == (caddr_t)-1) {
        emit dataReceived("mmap failed");
        close(fd);
        emit finished();
        return;
      }

      sem_t* semptr = sem_open(SemaphoreName, 0);
      if (semptr == SEM_FAILED) {
        emit dataReceived("sem_open failed");
        munmap(memptr, ByteSize);
        close(fd);
        emit finished();
        return;
      }

      int count = 0;
      while(1) {
        sem_wait(semptr);
        emit dataReceived(QString("%1").arg(count));
        count++;
      }
    }
};