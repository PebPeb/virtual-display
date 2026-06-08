#ifndef VD_PARAMETERS_H
#define VD_PARAMETERS_H


#include <stdint.h>  
#include <pthread.h>
#include <semaphore.h>

#define SharedMemory "vd-Shm"
#define AccessPerms 0644

#define LOCK 0
#define RELEASE 1

enum vdColorFormats {
  INVALID_CF = 0,
  RGB,
  Grayscale
};

struct vd_header {
  int id;
  sem_t readRDY;
  sem_t writeRDY;
  pthread_mutex_t producer;
  pthread_mutex_t consumer;
  uint8_t colorFormat = INVALID_CF;
  uint16_t pixelW = 0;
  uint16_t pixelH = 0;
};

#endif // VD_PARAMETERS_H