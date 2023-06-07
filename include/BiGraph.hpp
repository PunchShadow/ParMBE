#ifndef _BIGRAPH_H_
#define _BIGRAPH_H_
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <map>
#include <tbb/tbb.h>
#include <tbb/concurrent_hash_map.h>
#include "VertexSet.hpp"
#include "utility.hpp"
#include "CSR.hpp"

#ifdef DEBUG
#include <assert.h>
#endif /* DEBUG */


using namespace std;

/* Storing the global information for read-only operations
*/


class BiGraph
{
private:
    tbb::concurrent_unordered_set<int> L_set; // L vertex set
    tbb::concurrent_unordered_set<int> R_set; // R vertex set
    std::multimap<int, int> E_set;
    
    /* Read-only data structure */
    std::map<int, tbb::concurrent_unordered_set<int>*> L_neighbor_list; // L_set's neighbor list, which is the subset of R_set
    std::map<int, tbb::concurrent_unordered_set<int>*> R_neighbor_list; // R_set's neighbor list, which is the subset of L_set
    // tbb::concurrent_unordered_set<std::pair<int, int>> E_set; // Edge set, pair of L_set and R_set
    
    /* CSR format */
    CSR csr;

    /* Results */
    tbb::concurrent_unordered_set<VertexSet*> B_final;
    
    /* Find the neighbor of the insert node */
    tbb::concurrent_unordered_set<int> *FindLNeighbor(int node);
    tbb::concurrent_unordered_set<int> *FindRNeighbor(int node);
public:
    BiGraph() {};
    ~BiGraph() {};
    
    /* @param:
       - file_path: the path to the file 
       - num: how many number of substring do we want to read 
       - tail: need to remain the tail of the line?
    */

    void read(string file_path, int num, int tail); // Read from edge pair file
    
    /* Add vertex to BiGraph's L_set and R_set, separately */
    void AddVertexL(int node);
    void AddVertexR(int node);

    /* Add edge to BiGraph's E_set */
    void AddEdge(int src, int des);


    tbb::concurrent_unordered_set<int> * getRSet() {return &R_set;};
    tbb::concurrent_unordered_set<int> * getLSet() {return &L_set;};
    int getLSetSize() {return L_set.size();}
    int getRSetSize() {return R_set.size();}
    
    /* Get the neighbor from L_neighbor_list or R_neighbor_list */
    VertexSet *GetNeighbor(int node, bool nodeIsLeft);
    VertexSet *GetNeighborSet(VertexSet *set, bool setIsLeft);

    /* Calculate degree */
    int getDegree(int node, bool nodeIsLeft);


    /* Return 1 if the node is in L_set; 0 in R_set */
    int VertexIsLeft(int node);
    int SetIsLeft(VertexSet *set);

    /* Insert to final B */
    void InsertToFinal(VertexSet *B);

    /* State query */
    void PrintSetSize();
    void PrintFinalB();
    void PrintLNeighborList();
    void PrintRNeighborList();
};

#endif // _BIGRAPH_H_


