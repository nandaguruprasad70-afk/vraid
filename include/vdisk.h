#ifndef VDISK_H
#define VDISK_H
#include "common.h"
#include <string>

class VDisk {
public:
    int id;
    std::string path;
    int fd;
    bool failed;
    VDisk(int id, const std::string& path);
    ~VDisk();
    bool init();
    int read_block(uint64_t blk, void* buf);
    int write_block(uint64_t blk, const void* buf);
    void inject_fail();
    bool is_healthy() const;
};
#endif
