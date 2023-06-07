#ifndef _PARL_MBC_H_
#define _PARL_MBC_H_

#include <iostream>
#include <string>
#include "BiGraph.hpp"
#include "VertexSet.hpp"
#include "LocalState.hpp"





/* Functions */
void remove_tail(BiGraph *BPG, VertexSet* X, VertexSet* new_tail, int node, int ms);

void unroll_iterative_recursive(tbb::concurrent_vector<int> *vec, int start, int stride, LocalState* ls);

/* Function Declaration */
void parlmbc(BiGraph *BPG, VertexSet *X, VertexSet *tau, VertexSet *tail, int ms);

void parlmbc_main(string file_path);


#endif // _PARL_MBC_H_