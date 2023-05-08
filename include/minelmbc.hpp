#ifndef _MINELMBC_H_
#define _MINELMBC_H_

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include <string>
#include "BiGraph.hpp"
#include "VertexSet.hpp"

extern void minelmbc_thread(BiGraph *BPG, VertexSet *X, VertexSet *tau, VertexSet *tail, int ms);


extern void minelmbc_main(std::string file_path);




#endif // _MINELMBC_H_