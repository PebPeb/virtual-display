#include <stdio.h>
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>

#include "vd_icp_driver.h"

int main(int argc, char *argv[]) {
  vd_icp_producer Test;
  Test.init();

  sem_t* semptr = sem_open(SemaphoreName,
    O_CREAT,      
    AccessPerms, 
    0); 

  int count = 0;
  while(1) {
    sem_wait(semptr);
  }

  return 0;
}