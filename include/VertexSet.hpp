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
    tbb::concurrent_unordered_set<int> *L_set;
    tbb::concurrent_unordered_set<int> *R_set;

public:

    /* Constructors & deconstructor */
    VertexSet() {this->L_set = new tbb::concurrent_unordered_set<int>(); this->R_set = new tbb::concurrent_unordered_set<int>();};
    VertexSet(tbb::concurrent_unordered_set<int> *L_set, tbb::concurrent_unordered_set<int> *R_set);
    ~VertexSet();

    /* Operator =  overloading: copy by reference */
    void operator=(const VertexSet &src) {
        this->L_set = src.L_set;
        this->R_set = src.R_set;   
    }
    /* Operator * (intersection) overloading */
    VertexSet operator*(const VertexSet &src) const{
        VertexSet result;
        TBB_CUS_INTERSECT(result.L_set, this->L_set, src.L_set);
        TBB_CUS_INTERSECT(result.R_set, this->R_set, src.R_set);
        return result;
    }

    /* Operator + (union) */
    VertexSet operator+(const VertexSet &src) const{
        VertexSet result;
        TBB_CUS_UNION(result.L_set, src.L_set);
        TBB_CUS_UNION(result.R_set, src.R_set);
        TBB_CUS_UNION(result.L_set, this->L_set);
        TBB_CUS_UNION(result.R_set, this->R_set);
        return result;
    }

    void operator+=(const VertexSet &src) {
        TBB_CUS_UNION(this->L_set, src.L_set);
        TBB_CUS_UNION(this->R_set, src.R_set);
    }

    /* CARE */
    void operator*=(const VertexSet &src) {
        VertexSet result;
        
        TBB_CUS_INTERSECT(result.L_set, this->L_set, src.L_set);
        TBB_CUS_INTERSECT(result.R_set, this->R_set, src.R_set);
        // TBB_CUS_COPY(this->L_set, result.L_set);
        // TBB_CUS_COPY(this->R_set, result.R_set);
        this->L_set = result.L_set;
        this->R_set = result.R_set;
    }

    /* Operator - (substract) */
    VertexSet operator-(const VertexSet &src) const {
        VertexSet result;
        
        TBB_CUS_COPY(result.L_set, this->L_set);
        TBB_CUS_COPY(result.R_set, this->R_set);
        TBB_CUS_SUBTRACT(result.L_set, src.L_set);
        TBB_CUS_SUBTRACT(result.R_set, src.R_set);

        return result;
    }

    void operator-=(const VertexSet &src) {
        TBB_CUS_SUBTRACT(this->L_set, src.L_set);
        TBB_CUS_SUBTRACT(this->R_set, src.R_set);
    }

    /* Operator == (equal) */
    bool operator==(const VertexSet &src) const {
        for (auto it = this->L_set->begin(); it != this->L_set->end(); it++) {
            if (src.L_set->find(*it) == src.L_set->end()) return false;
        }
        for (auto it = this->R_set->begin(); it != this->R_set->end(); it++) {
            if (src.R_set->find(*it) == src.R_set->end()) return false;
        }
        return true;
    }

    /* Operater <= (contain) */
    bool operator<=(const VertexSet &src) const {
        if (TBB_CUS_SUBSET(this->L_set, src.L_set) && TBB_CUS_SUBSET(this->R_set, src.R_set)) return true;
        else return false;
    }

    void CUS2VertexSet(tbb::concurrent_unordered_set<int> *set, BiGraph *bigraph);

    /* Insert node to find the neighbor set of (L_set + R_set + node)*/
    // tbb::concurrent_unordered_set<int> * FindNeighborWNode(int node, bool insert); // Find the set when insert vertex node
    // tbb::concurrent_unordered_set<int> * FindNeighborWSet(tbb::concurrent_unordered_set<int> *set, bool insert);

    tbb::concurrent_unordered_set<int> * GetLset() {return L_set;} /* FIXME: Check the usage of this function for NULL pointer */
    tbb::concurrent_unordered_set<int> * GetRset() {return R_set;} /* FIXME: Check the usage of this function for NULL pointer*/

    /* Setters */
    void SetLset(tbb::concurrent_unordered_set<int> *L_set) {if (L_set == NULL) return; this->L_set = L_set;}
    void SetRset(tbb::concurrent_unordered_set<int> *R_set) {if (R_set == NULL) return; this->R_set = R_set;}


    /* Get status of the set */
    bool IsEmpty();
    bool IsLsetEmpty();
    bool IsRsetEmpty();
    int IsAllLeft();
    int GetLsetSize(); // Return -1 if the L_set is not allocated
    int GetRsetSize(); // Return -1 if the R_set is not allocated
    int GetSize(); // Return -1 if both L_set and R_set is not allocated
    tbb::concurrent_unordered_set<int> * GetX() { if (L_set->size() > R_set->size()) return L_set; else return R_set;}

    void deep_copy(VertexSet *src); /* Deep copy from src to this */
    void insert_R(int node); /* Insert node to R set and update the temp_neighbor. */
    void insert_L(int node); /* Insert node to L set and update the temp_neighbor. */
    void clear(); /* clear both L_set and R_set if the pointers of them are not NULL */
    void union_VertexSet(VertexSet *src); /* Union src to this */
    void union_LSet(tbb::concurrent_unordered_set<int> *set); /* Union set to L_set */
    void union_RSet(tbb::concurrent_unordered_set<int> *set); /* Union set to R_set */
    void unsafe_remove_L(int node);
    void unsafe_remove_R(int node);


    bool FindInLSet(int node); /* Find node in L_set */
    bool FindInRSet(int node); /* Find node in R_set */


    void PrintStatus(); /* Print the status. */

};



#endif // _VERTEXSET_H_