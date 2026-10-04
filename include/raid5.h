#ifndef RAID5_H
#define RAID5_H
#include "common.h"
#include "vdisk.h"
#include <vector>

class RAID5 {
public:
    std::vector<VDisk*> disks;
    RAID5(const std::vector<VDisk*>& d);
    void map_lba(uint64_t lba, int* stripe, int* idx, int* parity_disk);
    int data_disk_for_index(int idx, int p);
    static void xor_buf(const char* a, const char* b, char* out, int len);
    int read(uint64_t lba, char* out);
    int write(uint64_t lba, const char* in);
};
#endif
