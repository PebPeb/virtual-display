#include <stdio.h>
#include <iostream>
#include <semaphore.h>
#include <fcntl.h>

#include "vd_icp_driver.h"

int main(int argc, char *argv[]) {
  vd_icp_producer Test;
  Test.init();

  sem_t tmp;
  sem_init(&tmp, 1, 1);
  while (1) {
    sem_wait(&tmp);
  }

  return 0;
}