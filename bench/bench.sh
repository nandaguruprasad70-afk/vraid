#!/bin/bash
echo "Benchmark: Rebuild 200 blocks"
time ./raidctl init > /dev/null 2>&1
time ./raidctl fail 2 > /dev/null 2>&1
time ./raidctl rebuild > /dev/null 2>&1
echo "Benchmark complete"
