#include <string>
#include <pthread.h>

#include "vd_parameters.h"

class vd_icp_driver {
  protected:
    std::string sharedMemoryFile;
    vd_header *shmPtr;
    bool configured = false;

    bool discover_device();
    bool create_device();
    virtual bool try_connecting(vd_header* vdObj) {return false;};
    virtual bool init_mutex() {return false;};
  public:
    vd_icp_driver();
    ~vd_icp_driver();

    void init();
};

class vd_icp_producer : public vd_icp_driver {
  private:
    bool try_connecting(vd_header* vdObj) override;
    bool init_mutex() override;
  public:
    vd_icp_producer();
    ~vd_icp_producer();
};

class vd_icp_consumer : public vd_icp_driver {
  private:
    bool try_connecting(vd_header* vdObj) override;
    bool init_mutex() override;
  public:
    vd_icp_consumer();
    ~vd_icp_consumer();
};