#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_hash_map.h>

using namespace std;

#ifdef DEBUG
#include <assert.h>

#define CHECK_VERTEXSET_IS_ONE_SIDE(X) \
    assert(X->GetLsetSize() == 0 || X->GetRsetSize() == 0); \
    assert(! (X->GetLsetSize() > 0 && X->GetRsetSize() > 0))


#define CHECK_VERTEXSET_IS_NON_EMPTY(X) \
    assert(X->GetLsetSize() > 0 || X->GetRsetSize() > 0)



#endif // DEBUG

//#define TBB_CUS_COPY(des, src)      for(auto it = src->begin(); it != src->end(); it++) des->insert(*it)

inline
void TBB_CUS_COPY(tbb::concurrent_unordered_set<int> *des, tbb::concurrent_unordered_set<int> *src)
{
    if (src == NULL) {
#ifdef DEBUG
        assert(0);
#endif // DEBUG
    }
    des->clear(); /* Fisrt clear the des set*/
    for (auto it = src->begin(); it != src->end(); it++) {
        des->insert(*it);
    }
}

inline 
void TBB_CHM_COPY(tbb::concurrent_hash_map<int, int> *des, tbb::concurrent_hash_map<int, int> *src)
{
    if (src == NULL) return;
    if (src->size() == 0) return;
    for (auto it = src->begin(); it != src->end(); it++) {
        des->insert(*it);
    }
}

/* Determine if set1 is set2's subset*/
inline
bool TBB_CUS_SUBSET(tbb::concurrent_unordered_set<int> *set1, tbb::concurrent_unordered_set<int> *set2)
{
    if (set1 == NULL || set1->size() == 0) return true;
    for (auto it = set1->begin(); it != set1->end(); it++) {
        if (set2->find(*it) == set2->end()) return false;
    }
    return true;
}

inline
void TBB_CUS_SUBTRACT(tbb::concurrent_unordered_set<int> *des, tbb::concurrent_unordered_set<int> *src)
{
    if (src == NULL) return;
    if (src->size() == 0) return;
    for (auto it = src->begin(); it != src->end(); it++) {
        if (des->find(*it) != des->end()) {
            des->unsafe_erase(*it);
        }
    }
}

inline
void TBB_CUS_UNION(tbb::concurrent_unordered_set<int> *des, tbb::concurrent_unordered_set<int> *src)
{
    if (src == NULL) return;
    if (src->size() == 0) return;
    for (auto it = src->begin(); it != src->end(); it++) {
        des->insert((int)*it);
    }
}

inline
void TBB_CUS_INTERSECT(tbb::concurrent_unordered_set<int> *des, tbb::concurrent_unordered_set<int> *src1, tbb::concurrent_unordered_set<int> *src2)
{
    /* Get the intersection of src1 and scr2, then sort in des */
    if (des == NULL) {
#ifdef DEBUG
        assert(0);
#endif // DEBUG
        return;
    }
    des->clear();
    if (src1 == NULL || src2 == NULL) {
#ifdef DEBUG
        assert(0);
#endif // DEBUG
        return;
    }
    if (src1->empty() || src2->empty()) {
        return;
    }
    for (auto it = src1->begin(); it != src1->end(); it++) {
        if (src2->find(*it) != src2->end()) {
            des->insert(*it);
        }
    }
}



#endif /* _UTILITY_H_ */