#include <string>
#include <pthread.h>

#include "vd_parameters.h"

class vd_icp_driver {
  protected:
    std::string sharedMemoryFile;
    vd_header *shmPtr;
    
    bool discover_device();
    bool create_device();
    virtual bool try_connecting(vd_header* vdObj) {return false;};
    virtual bool init_mutex() {return false;};
  public:
    uint32_t *imageData;
    
    vd_icp_driver();
    ~vd_icp_driver();

    bool write(int cmd);
    bool read(int cmd);
    virtual bool init();
};

class vd_icp_producer : public vd_icp_driver {
  private:
    uint16_t pWidth;
    uint16_t pHeight; 
    uint8_t vdColorFormat;

    bool try_connecting(vd_header* vdObj) override;
    bool init_mutex() override;
  public:
    vd_icp_producer(uint16_t pixelW, uint16_t pixelH, uint8_t vdCF);
    ~vd_icp_producer();

    bool init() override;
};

class vd_icp_consumer : public vd_icp_driver {
  private:
    bool try_connecting(vd_header* vdObj) override;
    bool init_mutex() override;
  public:
    vd_icp_consumer();
    ~vd_icp_consumer();
};