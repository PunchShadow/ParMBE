#include "LocalState.hpp"
#include <iostream>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include "VertexSet.hpp"
#include "utility.hpp"

LocalState::
LocalState(VertexSet *vs, VertexSet *t_x, VertexSet *tail, int ms)
{
    this->X.copy(vs);
    this->tau.copy(t_x);
    this->tail.copy(tail);
    this->ms = ms;
}

LocalState::~LocalState()
{

}


/* FIXME: The Neighbor set is defined as the common neighbors of all vertices in a given set, namely, 
we should use intersection not union. */
void
LocalState::CalculateNeighbor(int v, bool nodeIsLeft)
{
    this->XV_neighbor.clear();
    VertexSet v_neighbor = *(this->bigraph->GetNeighbor(v, nodeIsLeft));

    /* If the X set is empty, v_neighbor is the neighbor list */
    if (this->X.IsEmpty()) {
        this->XV_neighbor = v_neighbor;
        return;
    }


    VertexSet* x_neighbor = this->bigraph->GetNeighborSet(&(this->X), nodeIsLeft);
    if (x_neighbor == NULL) {
        /* X's common neighbor is null */
        return;
    }
    this->XV_neighbor = *x_neighbor;
    this->XV_neighbor *= v_neighbor;

    // if (x_neighbor == NULL) return;
    // else {
    //     if (x_neighbor->IsEmpty()) {
    //         this->XV_neighbor = v_neighbor;
    //     } else {
    //         this->XV_neighbor = v_neighbor;
    //         this->XV_neighbor *= (*x_neighbor);
    //     }
    // }
}

void
LocalState::CalculateNeighborNeighbor(int v, bool nodeIsLeft)
{
    this->CalculateNeighbor(v, nodeIsLeft);
    /* CARE: neighbor neighbor is not union, should use intersection. */
    VertexSet* x_neighbor = this->bigraph->GetNeighborSet(&(this->XV_neighbor), !nodeIsLeft);
    if (x_neighbor == NULL) {
        /* X's common neighbor is null */
        this->XV_neighbor_neighbor.clear();
    } else {
        this->XV_neighbor_neighbor = *x_neighbor;
    }
    
    //this->XV_neighbor_neighbor *= *(this->bigraph->GetNeighborSet(&(this->XV_neighbor)));
#ifdef DEBUG
    //assert(this->XV_neighbor.IsAllLeft() >= 0);
    //assert(this->XV_neighbor_neighbor.IsAllLeft() >= 0);
#endif /* DEBUG */
}




void
LocalState::PrintStatus()
{
    X.PrintStatus();
    cout << "tau size: " << tau.GetSize() << endl;
    cout << "tail size: " << tail.GetSize() << endl;
    cout << "B size: " << B.size() << endl;
    cout << "ms: " << ms << endl;
}