#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>

#include "vd_icp_driver.h"

int main(int argc, char *argv[]) {
  vd_icp_producer myProducer(200, 200, RGB);
  
  myProducer.init();


  if(myProducer.write(LOCK)) {
    std::cout << "Writing Data" << std::endl;
    for (int i = 0; i < 200; i++) {
      for (int j = 0; j < 200; j++) {
        myProducer.imageData[i*200+j] = 0xFF0000;
      }
    }
  }
  myProducer.write(RELEASE);

  return 0;
}