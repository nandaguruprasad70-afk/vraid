#include "../include/controller.h"
#include "../include/vdisk.h"
#include <cstdio>
#include <cstring>

int main(int argc, char** argv) {
    Controller ctrl;
    if(argc < 2) {
        printf("Usage: ./raidctl [init|status|fail <id>|rebuild|test|verify]\n");
        return 0;
    }
    std::string cmd = argv[1];

    if(cmd == "init") {
        ctrl.init();
        printf("INIT: Created disk0..3.img (64MB each)\n");
    } else if(cmd == "status") {
        ctrl.init();
        ctrl.status();
    } else if(cmd == "fail" && argc > 2) {
        ctrl.init();
        ctrl.fail_disk(atoi(argv[2]));
    } else if(cmd == "rebuild") {
        ctrl.init();
        VDisk spare(99, "spare.img");
        spare.init();
        ctrl.rebuild(&spare);
        printf("REBUILD: Running in background.\n");
    } else if(cmd == "test") {
        ctrl.init();
        char data[BLOCK_SIZE];
        memset(data, 'X', BLOCK_SIZE);
        printf("TEST: Writing block 0...\n");
        ctrl.write(0, data);
        printf("TEST: Failing disk 2...\n");
        ctrl.fail_disk(2);
        printf("TEST: Reading degraded block 0...\n");
        char out[BLOCK_SIZE];
        if(ctrl.read(0, out) == 0 && out[0]=='X')
            printf("PASS: Data reconstructed correctly (XOR working)\n");
        else
            printf("FAIL: Reconstruction error\n");
    } else if(cmd == "verify") {
        printf("VERIFY: Manual checksum comparison required.\n");
    }
    return 0;
}
