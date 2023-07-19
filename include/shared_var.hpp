#ifndef _SHARE_VAR_
#define _SHARE_VAR_

#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_hash_map.h>
#include <atomic>
#include <thread>

/* CSR format */
typedef struct CSR
{
    std::vector<int> row_ptr;
    std::vector<int> col_idx;
} CSR_t;


inline int num_thread = tbb::task_scheduler_init::default_num_threads();

inline CSR_t csr;
inline CSR_t csr_reverse;


inline bool tailIsLeft = false;

inline int mbe_num = 0;


/* Typedef */
typedef tbb::concurrent_unordered_set<int> L_set;
typedef tbb::concurrent_unordered_set<int> R_set;
typedef tbb::concurrent_unordered_set<int> X_set;
typedef tbb::concurrent_hash_map<int, int> hash_map;







#endif /* _SHARE_VAR_ */