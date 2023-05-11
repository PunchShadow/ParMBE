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
    this->L_set = L_set;
    this->R_set = R_set;
}

VertexSet::
~VertexSet()
{

}



/* ############### Getter & Setter ################ */




void 
VertexSet::CUS2VertexSet(tbb::concurrent_unordered_set<int> *set, BiGraph *bigraph)
{
    for (auto it = set->begin(); it != set->end(); it++) {
        if (bigraph->VertexIsLeft(*it)) this->L_set->insert(*it);
        else this->R_set->insert(*it);
    }
}



/* *****************************  */

bool
VertexSet::IsEmpty()
{
    return (this->IsLsetEmpty() && this->IsRsetEmpty());
}

bool
VertexSet::IsLsetEmpty()
{
    if (this->L_set == NULL) return true;
    if (this->L_set->empty()) return true;
    return false;
}

bool
VertexSet::IsRsetEmpty()
{
    if (this->R_set == NULL) return true;
    if (this->R_set->empty()) return true;
    return false;
}

int
VertexSet::IsAllLeft()
{
    if (L_set->empty() && !R_set->empty()) return 0;
    else if (!L_set->empty() && R_set->empty()) return 1;
    else return -1;
}


int 
VertexSet::GetLsetSize()
{
    if (this->L_set == NULL) return -1;
    return this->L_set->size();
}

int
VertexSet::GetRsetSize()
{
    if (this->R_set == NULL) return -1;
    return this->R_set->size();
}

int
VertexSet::GetSize()
{
    if (this->L_set == NULL && this->R_set == NULL) return 0;
    else if (this->L_set == NULL) return this->R_set->size();
    else if (this->R_set == NULL) return this->L_set->size();
    else return this->L_set->size() + this->R_set->size();
}


void 
VertexSet::deep_copy(VertexSet *src)
{
    TBB_CUS_COPY(this->L_set, src->GetLset());
    TBB_CUS_COPY(this->R_set, src->GetRset());
}

void
VertexSet::insert_L(int node)
{
    if (this->L_set == NULL) {
        /* Allocate a new L_set */
        this->L_set = new tbb::concurrent_unordered_set<int>();
    }
    this->L_set->insert(node);
}

void
VertexSet::insert_R(int node)
{
    if (this->R_set == NULL) {
        /* Allocate a new R_set */
        this->R_set = new tbb::concurrent_unordered_set<int>();
    }
    this->R_set->insert(node);
}


void
VertexSet::clear()
{
    /* Avoid clearing the null ptrs */
    if (this->L_set != NULL) this->L_set->clear();
    if (this->R_set != NULL) this->R_set->clear();
}



void 
VertexSet::union_VertexSet(VertexSet *src)
{
    TBB_CUS_UNION(L_set, src->GetLset());
    TBB_CUS_UNION(R_set, src->GetRset());
}

void
VertexSet::union_LSet(tbb::concurrent_unordered_set<int> *set)
{
    TBB_CUS_UNION(L_set, set);
}

void
VertexSet::union_RSet(tbb::concurrent_unordered_set<int> *set)
{
    TBB_CUS_UNION(R_set, set);
}


void
VertexSet::unsafe_remove_L(int node)
{
    if (this->L_set == NULL) {
        cout << "VertexSet::unsafe_remove_L(): L_set is NULL" << endl;
        exit(1);
    }
    this->L_set->unsafe_erase(node);
}

void
VertexSet::unsafe_remove_R(int node)
{
    if (this->R_set == NULL) {
        cout << "VertexSet::unsafe_remove_R(): R_set is NULL" << endl;
        exit(1);
    }
    this->R_set->unsafe_erase(node);
}


bool
VertexSet::FindInLSet(int node)
{
    if (this->L_set == NULL) {
        cout << "VertexSet::FindInLSet(): L_set is NULL" << endl;
        exit(1);
    }
    if (this->L_set->find(node) == this->L_set->end()) return false;
    else return true;
}

bool
VertexSet::FindInRSet(int node)
{
    if (this->R_set == NULL) {
        cout << "VertexSet::FindInRSet(): R_set is NULL" << endl;
        exit(1);
    }
    if (this->R_set->find(node) == this->R_set->end()) return false;
    else return true;
}



void 
VertexSet::PrintStatus()
{
    if (this->L_set != NULL) {
        cout << "L set: " << this->L_set->size() << ": ";
        for (auto it = this->L_set->begin(); it != L_set->end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    } else { cout << "L set: NULL" << endl; }

    if (this->R_set != NULL) {
        cout << "R set: " << this->R_set->size() << ": ";
        for (auto it = this->R_set->begin(); it != R_set->end(); it++) {
            cout << *it << " ";
        }
        cout << endl;
    } else { cout << "R set: NULL" << endl; }

}