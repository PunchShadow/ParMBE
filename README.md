## Introduction

This the re-implementation of the paper, *Shared-Memory Parallel Maximal Biclique Enumeration*, published at *2019 IEEE International Conferenece on High Performance Computing, Data, and Analysis (HiPC)* written by *Apurba Das et.al*

## Architecture

- include:
    - All the header files
- src:
    - All the cpp files
- test
    - Implementations including ParLMBC and ParMBE


## Usage

The ParLMBC and ParMBE (the proposed method in this paper) can be found in the `test` folder.
We take ParMBE for example. 

```shell
cd test/parmbe_test
make clean
make
./parmbe_main <edge_file_path> <thread_num>
```

The default `thread_num` is the maximum thread number of the underlying computer. 

**The format of the input graph file follows the format of [KORNET Network](http://konect.cc/networks/) shown below.**
Notably, since the graph is bipartite, the source_ndoe and destination_node should be in two disjoint set. 
The IDs of these two set start from **1**. Our graph reader in `src/CSR.cpp` will remove the replicated edges.
The two disjoint set's IDs can be overlaped.

```
% Some description of the file
source_node destination_node <other edge information>
... ... ...
... ... ...
```

