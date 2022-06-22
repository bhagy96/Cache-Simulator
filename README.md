# Cache-Simulator

## Overview 

Designed and implemented a simple cache simulator, with support for 
1. Directmapped, set-associative and fully-associative mapping, and 
2. Least Recently Used (LRU) cache replacement policy. The purpose of load value prediction is to be able to predict the value of a memory load instruction.



```
./cachesim -i <tracefile> -c <cache size> -b <block size> -w <number of ways>
```
Example of simulator run:

```
$ ./cachesim -i traces/gcc-10K.memtrace -c 1MB -b 1KB -w 8
------------------------------
Cache size: 1048576 Bytes
Block Size: 1024 Bytes
Sets: 128
Ways: 8
Tag bits: 15
Index bits: 7
Offset bits: 10
------------------------------
Hit rate: 99.15%
Miss rate: 0.85%
Hits: 9915
total: 10000
------------------------------
```

Note : Each line in the trace file contains:
the memory instruction type (L = load, S = store)
the offset in decimal
the memory address in hexadecimal

### Credits:

- The structural code was given by Prof. Nael Abu-Ghazaleh and Jason Zellmer. I have added the implementation and extension part of the simulator.
