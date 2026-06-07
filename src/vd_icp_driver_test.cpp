#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>

#include "vd_icp_driver.h"

int main(int argc, char *argv[]) {
  vd_icp_producer myProducer(10, 10, RGB);
  vd_icp_consumer myConsumer;
  
  myConsumer.init();
  myProducer.init();

  for (int k = 0; k < 10; k++) {
    if(myProducer.write(LOCK)) {
      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
          myProducer.imageData[i*10+j] = i*10+j;
        }
      }
    }
    myProducer.write(RELEASE);

    if(myConsumer.read(LOCK)) {
      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
          std::cout << std::to_string(myConsumer.imageData[i*10+j]) << " ";
        }
        std::cout << std::endl;
      }
    }
    myConsumer.read(RELEASE);
  }



  sem_t tmp;
  sem_init(&tmp, 1, 1);
  while (1) {
    sem_wait(&tmp);
  }

  return 0;
}