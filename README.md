# Virtual RAID Array Controller & Fault-Tolerance Simulator

## Project Overview

This project is a software-based RAID 5 simulator developed using C++17 on Linux.

The system uses virtual disk image files to simulate physical disks and demonstrates RAID 5 data striping, rotating parity, disk failure injection, degraded-mode reading, XOR-based data reconstruction, and rebuild functionality.

## Objectives

- Simulate multiple virtual disks using Linux image files.
- Implement RAID 5 striping and rotating parity.
- Perform block-level I/O using 4096-byte blocks.
- Simulate disk failures.
- Reconstruct missing data using XOR parity.
- Demonstrate RAID controller states such as OPTIMAL and DEGRADED.
- Provide a rebuild mechanism using a spare virtual disk.

## Technologies Used

- C++17
- Linux / Ubuntu
- GNU g++
- POSIX `pread()` / `pwrite()`
- pthread / C++ threads
- Make

## System Architecture

```text
raidctl (CLI)
      |
      v
Controller
      |
      v
RAID5 Engine
      |
      v
VDisk Layer
      |
      v
Linux Disk Image Files
