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
    assert(X->GetLsetSize() <= 0 || X->GetRsetSize() <= 0); \
    assert(! (X->GetLsetSize() > 0 && X->GetRsetSize() > 0))


#define CHECK_VERTEXSET_IS_NON_EMPTY(X) \
    assert(X->GetLsetSize() > 0 || X->GetRsetSize() > 0)



#endif // DEBUG

/* If des is NULL allocate new set for des */
inline
void TBB_CUS_COPY(tbb::concurrent_unordered_set<int> *des, tbb::concurrent_unordered_set<int> *src)
{
    /* Consider if the src and des are NULL */
    if (des == NULL) {
        cout << "The des of TBB_CUS_COPY is NULL" << endl; // DEBUG
        exit(1);
    }
    des->clear();
    if (src == NULL) {
        return;
    } else {
        for (auto it = src->begin(); it != src->end(); it++) {
            des->insert(*it);
        }
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
    /* Consider the pointer might be NULL. */
    if (set1 == NULL) return true;
    if (set1->size() == 0) return true;
    if (set2 == NULL) return false;
    if (set2->size() == 0) return false;

    for (auto it = set1->begin(); it != set1->end(); it++) {
        if (set2->find(*it) == set2->end()) return false;
    }
    return true;
}

/* des stract the elements in src */
inline
void TBB_CUS_SUBTRACT(tbb::concurrent_unordered_set<int> *des, tbb::concurrent_unordered_set<int> *src)
{
    if (des == NULL) return;
    if (des->size() == 0) return;
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

    /* If des is NULl, allocate a new set for des */
    if (des == NULL) {
        cout << "The des of TBB_CUS_UNION is NULL" << endl; // DEBUG
        exit(1);
    }
    for (auto it = src->begin(); it != src->end(); it++) {
        des->insert((int)*it);
    }
}

inline
void TBB_CUS_INTERSECT(tbb::concurrent_unordered_set<int> *des, tbb::concurrent_unordered_set<int> *src1, tbb::concurrent_unordered_set<int> *src2)
{
    /* Get the intersection of src1 and scr2, then sort in des */
    if (des == NULL) {
        des = new tbb::concurrent_unordered_set<int>();
    }
    des->clear();
    if (src1 == NULL || src2 == NULL) return;
    else if (src1 != NULL && src2 != NULL) {
        if (src1->size() == 0 || src2->size() == 0) return;
        for (auto it = src1->begin(); it != src1->end(); it++) {
            if (src2->find(*it) != src2->end()) {
                des->insert(*it);
            }
        }
    }
    else {
        return;
    }
}



#endif /* _UTILITY_H_ */