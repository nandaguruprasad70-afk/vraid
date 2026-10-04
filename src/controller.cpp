#include "../include/controller.h"
#include <cstdio>
#include <thread>
#include <chrono>

Controller::Controller() : degraded(false), failed_id(-1), rebuilding(false) {}

Controller::~Controller() {
    for(auto d: disks) delete d;
}

bool Controller::init() {
    for(int i=0;i<4;i++) {
        char name[32];
        snprintf(name,32,"disk%d.img",i);
        VDisk* d = new VDisk(i,name);
        d->init();
        disks.push_back(d);
    }
    raid = std::make_unique<RAID5>(disks);
    return true;
}

int Controller::write(uint64_t lba, const char* data) { return raid->write(lba,data); }
int Controller::read(uint64_t lba, char* out) { return raid->read(lba,out); }

void Controller::fail_disk(int id) {
    if(id<0||id>=4) return;
    disks[id]->inject_fail();
    failed_id = id;
    degraded = true;
    printf("FAIL: Disk %d marked FAILED. State -> DEGRADED\n", id);
}

void Controller::rebuild(VDisk* spare) {
    printf("REBUILD: Starting to spare...\n");
    rebuilding = true;
    std::thread([this,spare]() {
        for(int b=0; b<200; b++) {
            char buf[BLOCK_SIZE];
            if(this->read((uint64_t)b, buf)==0)
                spare->write_block((uint64_t)b, buf);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        this->rebuilding = false;
        printf("REBUILD: Complete.\n");
    }).detach();
}

void Controller::status() {
    printf("=== RAID5 ARRAY STATUS ===\n");
    printf("State: %s\n", degraded ? (rebuilding ? "REBUILDING" : "DEGRADED") : "OPTIMAL");
    for(auto d: disks) {
        printf("  disk%d.img -> %s\n", d->id, d->is_healthy() ? "HEALTHY" : "FAILED");
    }
    printf("==========================\n");
}
