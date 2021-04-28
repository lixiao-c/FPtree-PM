## FPtree
### 介绍
原始论文：FPTree: A Hybrid SCM-DRAM Persistent and Concurrent B-Tree for Storage Class Memory  
基于 https://github.com/liumx10/ICPP-RNTree 复现  
使用PMDK的libvmem管理PM空间  
使用jemalloc作为内存分配器  
### 使用
```
mkdir build
cd build
cmake ..
make
./benchmark
```