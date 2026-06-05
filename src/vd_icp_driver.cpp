#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>

#include "vd_icp_driver.h"

// ----------------------------------------------------------

vd_icp_driver::vd_icp_driver() {}
vd_icp_driver::~vd_icp_driver() {}

/**
 * @brief Initialize the device driver
 * 
 */
void vd_icp_driver::init() {
  if(discover_device()) {     // Try and find a valid device
    printf("Connected To Device\n");
  } else {                    // Create a new device
    printf("Creating New Device\n");\
    create_device();
  }
  configured = true;
}

/**
 * @brief Discovers if there are current devices running
 * if there is no device then it creates a new one. Initiates
 * a connection between the device which is either as a producer
 * or consumer.
 * 
 */
bool vd_icp_driver::discover_device(){
  struct dirent* entry;
  DIR* dir = opendir("/dev/shm");

  if (!dir) {
    perror("opendir");
    return false;
  }

  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;
    if (name.rfind(SharedMemory, 0) == 0) {
      int fd = shm_open(name.c_str(), O_RDWR, AccessPerms);

      if (fd == -1) {
        std::perror("shm_open failed");
        return false;
      }

      vd_header* currVdDriver = (vd_header*) mmap(
        NULL, 
        sizeof(vd_header), 
        PROT_READ | PROT_WRITE, 
        MAP_SHARED, 
        fd, 
        0
      );

      // Try to connect
      if (try_connecting(currVdDriver)) {
        return true;
      }
    }
  }

  closedir(dir);
  return false;
}

/**
 * @brief Creates a new device. A device is given a ID
 * number that goes in ascending order.
 * 
 * @return true 
 * @return false 
 */
bool vd_icp_driver::create_device() {
  struct dirent* entry;
  DIR* dir = opendir("/dev/shm");         // Getting the shared memory directory

  if (!dir) {
    perror("opendir");
    return false;
  }
  
  // Step through all valid shared memorys and pull out ID numbers
  // currently takes the largest shared memory available
  std::vector<int> takenIDs;              // List of the ID numbers already in use
  while ((entry = readdir(dir)) != nullptr) {
    std::string name = entry->d_name;
    if (name.rfind(SharedMemory, 0) == 0) {
      int fd = shm_open(name.c_str(), O_RDONLY, AccessPerms);

      if (fd == -1) {
        std::perror("shm_open failed");
        return false;
      }

      vd_header* currVdDriver = (vd_header*) mmap(
        NULL, 
        sizeof(vd_header), 
        PROT_READ, 
        MAP_SHARED, 
        fd, 
        0
      );

      takenIDs.push_back(currVdDriver->id);
      close(fd);
    }
  }
  closedir(dir);
  
  // Find next ID number (files are already in descending order)
  int id = 0;
  for (int i = takenIDs.size() - 1; i >= 0; i--) {
    if(takenIDs[i] == id) {
      id += 1;
    } else {
      break;
    }
  }

  // Creating new shared memory
  sharedMemoryFile = std::string(SharedMemory) + "-" + std::to_string(id);
  int fd = shm_open(sharedMemoryFile.c_str(), O_CREAT | O_RDWR, AccessPerms);
  if (fd == -1) {
    std::perror("shm_open failed");
    return false;
  }

  if (ftruncate(fd, sizeof(vd_header)) == -1) {
    std::perror("ftruncate failed");
    close(fd);
    return false;
  } 

  shmPtr = (vd_header*) mmap(
    NULL, 
    sizeof(vd_header), 
    PROT_READ | PROT_WRITE, 
    MAP_SHARED, 
    fd, 
    0
  );

  shmPtr->id = id;
  init_mutex();
  sem_init(&shmPtr->readRDY, 1, 1);
  sem_init(&shmPtr->writeRDY, 1, 1);

  return true;
}

// ----------------------------------------------------------

vd_icp_producer::vd_icp_producer() {}
vd_icp_producer::~vd_icp_producer() {}

/**
 * @brief Tries to connect to the mutex
 * 
 * @param vdObj 
 */
bool vd_icp_producer::try_connecting(vd_header* vdObj) {
  int r = pthread_mutex_trylock(&vdObj->producer);
  if (r == EOWNERDEAD) {            // Mutex owner died 
    pthread_mutex_consistent(&vdObj->producer);
    r = 0;
  }
  if (!r) { return true; }
  return false;
}

/**
 * @brief Initiates mutex for a new device and tries to connect to it
 * 
 * @return true 
 * @return false 
 */
bool vd_icp_producer::init_mutex() {
  // Setting up mutex attributes to work with shared memory
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);

  pthread_mutex_init(&shmPtr->producer, &attr);
  pthread_mutex_init(&shmPtr->consumer, &attr);
  return try_connecting(shmPtr);
}

// ----------------------------------------------------------

vd_icp_consumer::vd_icp_consumer() {}
vd_icp_consumer::~vd_icp_consumer() {}

/**
 * @brief Tries to connect to the mutex
 * 
 * @param vdObj 
 */
bool vd_icp_consumer::try_connecting(vd_header* vdObj) {
  int r = pthread_mutex_trylock(&vdObj->consumer);
  if (r == EOWNERDEAD) {            // Mutex owner died 
    pthread_mutex_consistent(&vdObj->consumer);
    r = 0;
  }
  if (!r) { return true; }
  return false;
}

/**
 * @brief Initiates mutex for a new device and tries to connect to it
 * 
 * @return true 
 * @return false 
 */
bool vd_icp_consumer::init_mutex() {
  // Setting up mutex attributes to work with shared memory
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
  pthread_mutexattr_setrobust(&attr, PTHREAD_MUTEX_ROBUST);

  pthread_mutex_init(&shmPtr->consumer, &attr);
  return try_connecting(shmPtr);
}


