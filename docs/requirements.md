# Requirements — Virtual RAID Controller

## Functional

- FR1: Create 4 virtual disks (disk0..3.img) + 1 spare.
- FR2: RAID 5 (striping + rotating parity).
- FR3: Block I/O at 4096 bytes.
- FR4: Fault injection (disk failure, bit-rot).
- FR5: Degraded read via XOR reconstruction.
- FR6: Hot-spare rebuild thread.

## Non-Functional

- C++17, Linux, pthread, block-aligned pread/pwrite.
- No external dependencies (only std lib).

## Milestones

M1: Disk layer (Day 1 AM)  
M2: RAID 5 engine (Day 1 PM)  
M3: CLI + rebuild (Day 2 AM)  
M4: Tests + report (Day 2 PM)
