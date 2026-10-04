# Design — Virtual RAID Controller

## Architecture
- Monolithic C++ executable (`raidctl`) + `VDisk` layer.
- RAID5 uses rotating parity mapped via LBA->Stripe->DataDisk.

## Component Interfaces
- `VDisk::read_block(uint64_t, void*)`
- `RAID5::read/write(uint64_t, char*)`
- `Controller::fail_disk(int) -> state DEGRADED`

## UML (Text)
See architecture.txt (Class + Sequence + State Machine).

## State Machine
[OPTIMAL] -> [DEGRADED] -> [REBUILDING] -> [OPTIMAL]
