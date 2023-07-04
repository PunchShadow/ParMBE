#ifndef _THREAD_CONFIG_
#define _THREAD_CONFIG_

#include <iostream>
#include <string>
#include <tbb/task_scheduler_init.h>
#include <tbb/spin_mutex.h>


/* Mutex locks */
static inline tbb::spin_mutex insert_final_lock;
static inline tbb::spin_mutex insert_tail_lock;
static inline tbb::spin_mutex insert_XV_lock1;
static inline tbb::spin_mutex insert_XV_lock2;
static inline tbb::spin_mutex insert_XV_lock3;

#ifdef DEBUG
inline tbb::spin_mutex debug_print_lock;
#endif /* DEBUG */




#endif /* _THREAD_CONFIG_ */