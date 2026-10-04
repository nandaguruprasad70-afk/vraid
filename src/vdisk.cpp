#include "../include/vdisk.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
#include <cstdio>

VDisk::VDisk(int id, const std::string& p) : id(id), path(p), fd(-1), failed(false) {}

VDisk::~VDisk() { if(fd>=0) close(fd); }

bool VDisk::init() {
    fd = open(path.c_str(), O_RDWR | O_CREAT, 0644);
    if(fd < 0) return false;
    off_t sz = (off_t)BLOCK_SIZE * DISK_SIZE_BLOCKS;
    ftruncate(fd, sz);
    return true;
}

int VDisk::read_block(uint64_t blk, void* buf) {
    if(failed) return -1;
    if(fd < 0) fd = open(path.c_str(), O_RDWR);
    if(fd < 0) return -1;
    off_t off = (off_t)blk * BLOCK_SIZE;
    return (pread(fd, buf, BLOCK_SIZE, off) == BLOCK_SIZE) ? 0 : -1;
}

int VDisk::write_block(uint64_t blk, const void* buf) {
    if(failed) return -1;
    if(fd < 0) fd = open(path.c_str(), O_RDWR);
    if(fd < 0) return -1;
    off_t off = (off_t)blk * BLOCK_SIZE;
    return (pwrite(fd, buf, BLOCK_SIZE, off) == BLOCK_SIZE) ? 0 : -1;
}

void VDisk::inject_fail() {
    failed = true; close(fd); fd = -1;
}

bool VDisk::is_healthy() const { return !failed; }
