#ifndef _VERTEXSET_H_
#define _VERTEXSET_H_

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include "utility.hpp"

class BiGraph; // Forward declaration

class VertexSet
{
private:
    tbb::concurrent_unordered_set<int> L_set;
    tbb::concurrent_unordered_set<int> R_set;

public:

    /* Constructors & deconstructor */
    VertexSet(){};
    VertexSet(tbb::concurrent_unordered_set<int> *L_set, tbb::concurrent_unordered_set<int> *R_set);
    ~VertexSet();

    /* Operator =  overloading */
    VertexSet operator=(VertexSet &src) {
        this->GetLset()->clear();
        this->GetRset()->clear();
        TBB_CUS_COPY(this->GetLset(), src.GetLset());
        TBB_CUS_COPY(this->GetRset(), src.GetRset());
        return *this;
    }
    /* Operator * (intersection) overloading */
    VertexSet operator*(VertexSet &src) {
        VertexSet result;
        TBB_CUS_INTERSECT(&result.L_set, &L_set, src.GetLset());
        TBB_CUS_INTERSECT(&result.R_set, &R_set, src.GetRset());
        return result;
    }

    /* Operator + (union) */
    VertexSet operator+(VertexSet &src) {
        VertexSet result;
        TBB_CUS_UNION(result.GetLset(), src.GetLset());
        TBB_CUS_UNION(result.GetRset(), src.GetRset());
        TBB_CUS_UNION(result.GetLset(), &L_set);
        TBB_CUS_UNION(result.GetRset(), &R_set);
        return result;
    }

    VertexSet operator+=(VertexSet &src) {
        TBB_CUS_UNION(this->GetLset(), src.GetLset());
        TBB_CUS_UNION(this->GetRset(), src.GetRset());
        return *this;
    }

    /* CARE */
    VertexSet operator*=(VertexSet &src) {
        //cout << "VertexSet operator*=" << endl;
        VertexSet result;
        TBB_CUS_INTERSECT(result.GetLset(), this->GetLset(), src.GetLset());
        TBB_CUS_INTERSECT(result.GetRset(), this->GetRset(), src.GetRset());
        TBB_CUS_COPY(this->GetLset(), result.GetLset());
        TBB_CUS_COPY(this->GetRset(), result.GetRset());
        return result;
    }

    /* Operator - (substract) */
    VertexSet operator-(VertexSet &src) {
        VertexSet result;
        TBB_CUS_COPY(result.GetLset(), &L_set);
        TBB_CUS_COPY(result.GetRset(), &R_set);
        TBB_CUS_SUBTRACT(result.GetLset(), src.GetLset());
        TBB_CUS_SUBTRACT(result.GetRset(), src.GetRset());
        return result;
    }

    /* Operator == (equal) */
    bool operator==(VertexSet &src) {
        for (auto it = L_set.begin(); it != L_set.end(); it++) {
            if (src.L_set.find(*it) == src.L_set.end()) return false;
        }
        for (auto it = R_set.begin(); it != R_set.end(); it++) {
            if (src.R_set.find(*it) == src.R_set.end()) return false;
        }
        return true;
    }

    /* Operater <= (contain) */
    bool operator<=(VertexSet &src) {
        if (TBB_CUS_SUBSET(&L_set, src.GetLset()) && TBB_CUS_SUBSET(&R_set, src.GetRset())) return true;
        else return false;
    }

    void CUS2VertexSet(tbb::concurrent_unordered_set<int> *set, BiGraph *bigraph);

    /* Insert node to find the neighbor set of (L_set + R_set + node)*/
    // tbb::concurrent_unordered_set<int> * FindNeighborWNode(int node, bool insert); // Find the set when insert vertex node
    // tbb::concurrent_unordered_set<int> * FindNeighborWSet(tbb::concurrent_unordered_set<int> *set, bool insert);

    tbb::concurrent_unordered_set<int> * GetLset() {return &L_set;}
    tbb::concurrent_unordered_set<int> * GetRset() {return &R_set;}

    /* Setters */
    void SetLset(tbb::concurrent_unordered_set<int> *L_set) {if (L_set == NULL) return; TBB_CUS_COPY(&this->L_set, L_set);}
    void SetRset(tbb::concurrent_unordered_set<int> *R_set) {if (R_set == NULL) return; TBB_CUS_COPY(&this->R_set, R_set);}


    /* Get status of the set */
    bool IsEmpty() {return (L_set.empty() && R_set.empty());}
    bool IsLsetEmpty() {return L_set.empty();}
    bool IsRsetEmpty() {return R_set.empty();}
    int IsAllLeft();
    int GetLsetSize() {return L_set.size();}
    int GetRsetSize() {return R_set.size();}
    int GetSize() {return L_set.size() + R_set.size();}
    tbb::concurrent_unordered_set<int> * GetX() { if (L_set.size() > R_set.size()) return &L_set; else return &R_set;}

    void copy(VertexSet *src); /* Deep copy from src to this */
    void insert_R(int node) {R_set.insert(node);}; /* Insert node to R set and update the temp_neighbor. */
    void insert_L(int node) {L_set.insert(node);}; /* Insert node to L set and update the temp_neighbor. */
    void clear() {L_set.clear(); R_set.clear();}
    void union_VertexSet(VertexSet *src); /* Union src to this */
    void union_LSet(tbb::concurrent_unordered_set<int> *set); /* Union set to L_set */
    void union_RSet(tbb::concurrent_unordered_set<int> *set); /* Union set to R_set */
    void unsafe_remove_L(int node) {L_set.unsafe_erase(node);};
    void unsafe_remove_R(int node) {R_set.unsafe_erase(node);};


    bool FindInLSet(int node) {return (L_set.find(node) != L_set.end());}
    bool FindInRSet(int node) {return (R_set.find(node) != R_set.end());}


    void PrintStatus(); /* Print the status. */

};



#endif // _VERTEXSET_H_