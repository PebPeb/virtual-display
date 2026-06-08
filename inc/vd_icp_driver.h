#ifndef VD_ICP_DRIVER_H
#define VD_ICP_DRIVER_H

#include <string>
#include <pthread.h>

#include "vd_parameters.h"

class vd_icp_driver {
  protected:
    std::string sharedMemoryFile;
    
    bool discover_device();
    bool create_device();
    virtual bool try_connecting(vd_header* vdObj) {return false;};
    bool init_mutex();
    public:
    vd_header *shmPtr;
    uint32_t *imageData;

    vd_icp_driver();
    ~vd_icp_driver();

    virtual bool write(int cmd);
    virtual bool read(int cmd);
    virtual bool init();
};

class vd_icp_producer : public vd_icp_driver {
  private:
    uint16_t pWidth;
    uint16_t pHeight; 
    uint8_t vdColorFormat;

    bool try_connecting(vd_header* vdObj) override;
  public:
    vd_icp_producer(uint16_t pixelW, uint16_t pixelH, uint8_t vdCF);
    ~vd_icp_producer();

    bool init() override;
};

class vd_icp_consumer : public vd_icp_driver {
  private:
    uint16_t curpWidth;
    uint16_t curpHeight; 
    uint8_t  curvdColorFormat;

    bool try_connecting(vd_header* vdObj) override;
    bool checkIfChangesToDriver();
    bool updateShmPtr();
  public:
    vd_icp_consumer();
    ~vd_icp_consumer();

    bool init() override;
    bool write(int cmd) override;
    bool read(int cmd) override;
};

#endif // VD_ICP_DRIVER_H