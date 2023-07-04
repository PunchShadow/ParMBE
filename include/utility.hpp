#ifndef _UTILITY_H_
#define _UTILITY_H_

#include <iostream>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_hash_map.h>

using namespace std;


/* Forward declaration */
typedef struct CSR CSR_t;
typedef tbb::concurrent_unordered_set<int> X_set;



#ifdef DEBUG
#include <assert.h>

#define CHECK_VERTEXSET_IS_ONE_SIDE(X) \
    assert(X->GetLsetSize() <= 0 || X->GetRsetSize() <= 0); \
    assert(! (X->GetLsetSize() > 0 && X->GetRsetSize() > 0))


#define CHECK_VERTEXSET_IS_NON_EMPTY(X) \
    assert(X->GetLsetSize() > 0 || X->GetRsetSize() > 0)



#endif // DEBUG

inline void
print_set(const X_set& set)
{
    for (const auto& ele : set) {
        std::cout << ele << " ";
    }
    std::cout << std::endl;
}

inline void
set_copy(X_set& dst, const X_set& src)
{
    dst.clear();
    for (const auto& ele : src) {
        dst.insert(ele);
    }
}


inline X_set
set_union(const X_set& set1, const X_set& set2)
{
    X_set result = set1;

    for (const auto& ele : set2) {
        result.insert(ele);
    }

    return result;
}

inline X_set
set_subtract(const X_set& set1, const X_set& set2)
{
    X_set result;

    for (const auto& element : set1) {
        if (set2.find(element) == set2.end()) {
            result.insert(element);
        }
    }

    return result;
}


inline bool
set_subset(X_set& subset, X_set& superset)
{
    for (const auto& ele : subset) {
        if (superset.find(ele) == superset.end()) {
            return false;
        }
    }

    return true;
}


inline X_set
set_intersection(const X_set& set1, const X_set& set2)
{
    const auto& small_set = set1.size() < set2.size() ? set1 : set2;
    const auto& large_set = set1.size() < set2.size() ? set2 : set1;

    X_set result;

    for (const auto& ele : small_set) {
        if (large_set.find(ele) != large_set.end()) {
            result.insert(ele);
        }
    }

    return result;
}



#endif /* _UTILITY_H_ */