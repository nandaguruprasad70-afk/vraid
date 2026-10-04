#include "../include/raid5.h"
#include <cstring>

RAID5::RAID5(const std::vector<VDisk*>& d) : disks(d) {}

void RAID5::map_lba(uint64_t lba, int* stripe, int* idx, int* parity_disk) {
    int n = (int)disks.size();
    int d = n - 1;
    *stripe = (int)(lba / d);
    *idx = (int)(lba % d);
    *parity_disk = (n - 1) - (*stripe % n);
    if(*parity_disk < 0) *parity_disk += n;
}

int RAID5::data_disk_for_index(int idx, int p) {
    return (idx < p) ? idx : idx + 1;
}

void RAID5::xor_buf(const char* a, const char* b, char* out, int len) {
    for(int i=0;i<len;i++) out[i] = a[i] ^ b[i];
}

int RAID5::read(uint64_t lba, char* out) {
    int s, i, p;
    map_lba(lba, &s, &i, &p);
    int d = data_disk_for_index(i, p);
    char buf[BLOCK_SIZE];
    memset(out, 0, BLOCK_SIZE);

    if(disks[d]->is_healthy() && disks[d]->read_block((uint64_t)s, out) == 0)
        return 0;

    if(!disks[p]->is_healthy()) return -2;
    char par[BLOCK_SIZE];
    if(disks[p]->read_block((uint64_t)s, par) != 0) return -2;
    memcpy(out, par, BLOCK_SIZE);

    for(int k=0; k<((int)disks.size()-1); k++) {
        int other_d = data_disk_for_index(k, p);
        if(other_d == d) continue;
        char other[BLOCK_SIZE];
        if(disks[other_d]->read_block((uint64_t)s, other) == 0) {
            xor_buf(out, other, out, BLOCK_SIZE);
        }
    }
    return 0;
}

int RAID5::write(uint64_t lba, const char* in) {
    int s, i, p;
    map_lba(lba, &s, &i, &p);
    int d = data_disk_for_index(i, p);

    char old_data[BLOCK_SIZE] = {0};
    char old_parity[BLOCK_SIZE] = {0};
    char new_parity[BLOCK_SIZE];

    if(disks[d]->is_healthy()) disks[d]->read_block((uint64_t)s, old_data);
    if(disks[p]->is_healthy()) disks[p]->read_block((uint64_t)s, old_parity);

    char temp[BLOCK_SIZE];
    xor_buf(old_parity, old_data, temp, BLOCK_SIZE);
    xor_buf(temp, in, new_parity, BLOCK_SIZE);

    if(!disks[d]->is_healthy() || disks[d]->write_block((uint64_t)s, in)!=0) return -1;
    if(!disks[p]->is_healthy() || disks[p]->write_block((uint64_t)s, new_parity)!=0) return -1;
    return 0;
}
