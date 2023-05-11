#ifndef _LOCALSTATE_H_
#define _LOCALSTATE_H_

#ifdef DEBUG
#include <assert.h>
#endif // DEBUG

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include "BiGraph.hpp"
#include "VertexSet.hpp"
#include "utility.hpp"


using namespace std;


/* TODO: Pointer of the VertexSet in LocalState */
class LocalState 
{
private:
    VertexSet X; // vertex set
    VertexSet tau; // adjacency list of X - one-hop neighbor
    VertexSet tail; // tail vertices of X
    VertexSet XV_neighbor; // neighbor of X U {V}
    VertexSet XV_neighbor_neighbor; // neighbor of neighbor of X U {V}
    VertexSet Y;
    tbb::concurrent_vector<int> tail_vector;
    tbb::concurrent_unordered_set<tbb::concurrent_unordered_set<int>> B; // Set of all maximal bicliques containing X
    BiGraph *bigraph; // Pointer to the global graph
    int ms; // minimum size of threshold

public:

    LocalState(){ms = 1;};
    LocalState(VertexSet *vs, VertexSet *t_x, VertexSet *tail, int ms);
    ~LocalState();

    /* Setter */
    void setVertexSet(VertexSet *vs) {this->X.deep_copy(vs);}
    void setTail(VertexSet *in_tail) { this->tail.deep_copy(in_tail);}
    void setTau(VertexSet *in_tau) { this->tau.deep_copy(in_tau); }
    void setMs(int ms) {this->ms = ms;}
    void setBiGraph(BiGraph *g) { this->bigraph = g;}
    void setY(VertexSet *y) {this->Y.deep_copy(y);}
    

    /* Getter */
    BiGraph *getBiGraph() {return this->bigraph;}
    VertexSet *getX() {return &this->X;}
    VertexSet *getTail() {return &this->tail;}
    tbb::concurrent_vector<int> *getTailVector() {return &this->tail_vector;}
    VertexSet *getTau() {return &this->tau;}
    int getMs() {return this->ms;}
    VertexSet *getVertexSet() {return &X;}
    VertexSet *getXVNeighbor() {return &this->XV_neighbor;}
    VertexSet *getXVNeighborNeighbor() {return &this->XV_neighbor_neighbor;}
    
    tbb::concurrent_unordered_set<tbb::concurrent_unordered_set<int>> *getB() {return &this->B;}
    VertexSet *getY() {return &this->Y;}

    // void pruneTail(int v);

    /* Calcluate neighbor and store in XV_neighbor or XV_neighbor_neighbor */
    void CalculateNeighbor(int v, bool nodeIsLeft);
    void CalculateNeighborNeighbor(int v, bool nodeIsLeft);



    // VertexSet *findNeighborWNode(int v);
    // VertexSet *findNeighborWSet(VertexSet *v_set);

    void PrintStatus();

};

#endif // _LOCALSTATE_H_

