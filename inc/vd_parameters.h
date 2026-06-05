#include <stdint.h>  
#include <pthread.h>
#include <semaphore.h>

#define SharedMemory "vd-Shm"
#define SemaphoreName "vdIcpSem"
#define AccessPerms 0644

struct vd_header {
  int id;
  sem_t readRDY;
  sem_t writeRDY;
  pthread_mutex_t producer;
  pthread_mutex_t consumer;
};


