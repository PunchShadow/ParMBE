#include "VertexSet.hpp"
#include <iostream>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include "utility.hpp"
#include "BiGraph.hpp"


VertexSet::
VertexSet(tbb::concurrent_unordered_set<int> *L_set, tbb::concurrent_unordered_set<int> *R_set)
{
    if (L_set == NULL || R_set == NULL) {
        cout << "VertexSet::VertexSet(): L_set or R_set is NULL" << endl;
        exit(1);
    }
    TBB_CUS_COPY(&(this->L_set), L_set);
    TBB_CUS_COPY(&(this->R_set), R_set);
}

VertexSet::
~VertexSet()
{

}

void 
VertexSet::CUS2VertexSet(tbb::concurrent_unordered_set<int> *set, BiGraph *bigraph)
{
    for (auto it = set->begin(); it != set->end(); it++) {
        if (bigraph->VertexIsLeft(*it)) L_set.insert(*it);
        else R_set.insert(*it);
    }
}



void 
VertexSet::copy(VertexSet *src)
{
    TBB_CUS_COPY(&L_set, src->GetLset());
    TBB_CUS_COPY(&R_set, src->GetRset());
}



int
VertexSet::IsAllLeft()
{
    if (L_set.empty() && !R_set.empty()) return 0;
    else if (!L_set.empty() && R_set.empty()) return 1;
    else return -1;
}



void 
VertexSet::PrintStatus()
{
    cout << "L set: " << this->L_set.size() << ": ";
    for (auto it = this->L_set.begin(); it != L_set.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
    cout << "R set: " << R_set.size() << ": ";
    for (auto it = R_set.begin(); it != R_set.end(); it++) {
        cout << *it << " ";
    }
    cout << endl;
}


void 
VertexSet::union_VertexSet(VertexSet *src)
{
    TBB_CUS_UNION(&L_set, src->GetLset());
    TBB_CUS_UNION(&R_set, src->GetRset());
}

void
VertexSet::union_LSet(tbb::concurrent_unordered_set<int> *set)
{
    TBB_CUS_UNION(&L_set, set);
}

void
VertexSet::union_RSet(tbb::concurrent_unordered_set<int> *set)
{
    TBB_CUS_UNION(&R_set, set);
}