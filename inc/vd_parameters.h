#include <stdint.h>  
#include <pthread.h>


#define SharedMemory "vd-Shm"
#define SemaphoreName "vdIcpSem"
#define AccessPerms 0644

struct vd_header {
  int id;
  pthread_mutex_t producer;
  pthread_mutex_t consumer;
};


