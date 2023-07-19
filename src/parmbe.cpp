#include <iostream>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_hash_map.h>
#include <sys/time.h>
#include <atomic>
#include <thread>

#include "parlmbc.hpp"
#include "parmbe.hpp"




void
parmbe_main(string file_name)
{
    /* Initialization */
    tbb::task_scheduler_init init(num_thread);
    cout << "Allocated thread number: " << num_thread << endl;

    /* Initilize CSR */
    edgeListToCSR(file_name, csr, csr_reverse);
    /* CSR: Vertex in L_set consoles R_set's neighbors */
    /* CSR_reverse: Vertex in R_set consoles L_set's neighbors */

    /* Initilize X_set, Gamma(x), tail(x), and ms */
    /* tail: smaller, Gamma: larger */
    X_set X;
    L_set Gamma;
    R_set tail;
    int ms = 1;


    int csr_size = csr.row_ptr.size();
    int csr_reverse_size = csr_reverse.row_ptr.size();


    cout << "Original |L| : " << csr_size - 2 << endl;
    cout << "Original |R| : " << csr_reverse_size - 2 << endl;

    /* Assume two set's vertices start from ID 1 */

    /* tail is in R set, which is the smaller partition */
    for (int i=1; i < (int)csr_reverse.row_ptr.size() - 1 ; i++) {
        tail.insert(i);
    }
    /* Gamma is in L set, which is the larger partition */
    for (int j=1; j < (int)csr.row_ptr.size() - 1; j++) {
        Gamma.insert(j);
    }

    struct timeval start, end;
    gettimeofday(&start, NULL);

    /* Use tbb::global control */



    
    tbb::task_group tg0;
    tg0.run([&] {
        /* Start MBE */
        tbb::parallel_for_each(tail.begin(), tail.end(), 
            [&](int v) {
                X_set X_local;
                L_set Gamma_local;
                R_set tail_local;

                X_local.insert(v);

                /* Find the neighbor of v */
                int v_start, v_end, deg_v;
                searchNeighbor(csr_reverse, v, v_start, v_end); /* v: R_set , v's neighbor: L_set */
                deg_v = v_end - v_start;

                tbb::task_group tg;
                tg.run([&] {
                tbb::parallel_for(v_start, v_end, 1,
                    [&](int i) {
                        int u = csr_reverse.col_idx[i]; /* u: L_set */
                        Gamma_local.insert(u);
                    });
                });
                tg.wait();

                tbb::task_group tg1;
                tg1.run([&] {
                    tbb::parallel_for_each(Gamma_local.begin(), Gamma_local.end(),
                        [&](int w) {
                            
                            /* Find w's neighbor */
                            int w_start, w_end;
                            searchNeighbor(csr, w, w_start, w_end); /* w: L_set, w's neighbor: R_set */

                            /* y \in  Gamma(w) */
                            // cout << "y: ";
                            tbb::parallel_for(w_start, w_end, 1,
                                [&](int j) {
                                    int y = csr.col_idx[j]; /* y: R_set */
                                    // cout << y << " ";
                                    if (y == v) {
                                        return;
                                    }
                                    /* If rank(y) > rank(v) */
                                    int y_start, y_end, deg_y;
                                    searchNeighbor(csr_reverse, y, y_start, y_end); /* y: R_set, y's neighbor: L_set */
                                    deg_y = y_end - y_start;
                                    // cout << "(" << deg_y << ") ";
                                    if ((deg_y > deg_v) || (deg_y == deg_v && y > v)) {
                                        
                                        tail_local.insert(y);
                                    }
                                    // tail_local.insert(y);
                                });
                            // cout << endl;
                        });
                });
                tg1.wait();

                /* Print X_local, Gamma_local and tail_local */
                // cout << "X_local (" << deg_v << "): " ;
                // print_set(X_local);
                // cout << "Gamma_local: ";
                // print_set(Gamma_local);
                // cout << "tail_local: ";
                // print_set(tail_local);

                /* Frist check if {X_local} + {Gamma(X_local)} is a maximal biclique: Maximality check */
                /* Maximality: If Gamma_local's common neighbor is equal to X_local */
                R_set common_neighbor = findCommonNeighbor(csr, Gamma_local);

                if (set_equal(common_neighbor, X_local)) {
                    insert_final_lock.lock();
                    mbe_num++;
                    /* Print MBE */
                    // cout << "************ << +MBE: " << mbe_num << " >>**********" << endl;
                    // print_set(X_local);
                    // print_set(Gamma_local);
                    
                    insert_final_lock.unlock();
                }
                {
                parlmbc(X_local, Gamma_local, tail_local, ms);
                }
            });
    });
    tg0.wait();

    gettimeofday(&end, NULL);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1.0E-6;
    cout << "Execution time: " << time << " sec" << endl;

    cout << "MBE number: " << mbe_num << endl;
}