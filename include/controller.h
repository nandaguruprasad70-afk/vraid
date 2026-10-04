#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "common.h"
#include "vdisk.h"
#include "raid5.h"
#include <memory>
#include <vector>

class Controller {
public:
    std::vector<VDisk*> disks;
    std::unique_ptr<RAID5> raid;
    bool degraded;
    int failed_id;
    bool rebuilding;

    Controller();
    ~Controller();
    bool init();
    int write(uint64_t lba, const char* data);
    int read(uint64_t lba, char* out);
    void fail_disk(int id);
    void rebuild(VDisk* spare);
    void status();
};
#endif
