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

using namespace std;

// Assume |L_set| > |R_set|
void parlmbc(X_set X, L_set Gamma, R_set tail, int ms)
{
    /* Print X, tail */
    // cout << "X: ";
    // print_set(X);
    // cout << "tail: ";
    // print_set(tail);

    tbb::concurrent_vector<int> new_tail;

    tbb::task_group g1;

    g1.run([&] {
        tbb::parallel_for_each(tail.begin(), tail.end(), 
            [&](int node) {
                int node_start, node_end;
                int count = 0;
                searchNeighbor(csr_reverse, node, node_start, node_end);
                for (int i=node_start; i < node_end; i++) {
                    int neighbor = csr_reverse.col_idx[i];
                    if (Gamma.find(neighbor) != Gamma.end()) {
                        count++;
                        if (count >= ms) {
                            new_tail.push_back(node);
                            break;
                        }
                    }

                }
            });
    });
    g1.wait();

    /* Check if |X| + |tail(X)| < ms: terminal condition */
    if ((int)(X.size() + new_tail.size()) < ms) {
        return;
    }

    tbb::task_group g2;

    g2.run([&] {
        /* Sort vertices of new_tail into ascending order of |Gamma(X u {v})| */
        tbb::parallel_sort(new_tail.begin(), new_tail.end(), 
            [&](int node1, int node2) {

                int node1_start, node1_end, node2_start, node2_end;
                int node1_count = 0, node2_count = 0;
                searchNeighbor(csr_reverse, node1, node1_start, node1_end);
                searchNeighbor(csr_reverse, node2, node2_start, node2_end);

                for (int i=node1_start; i < node1_end; i++) {
                    int neighbor = csr_reverse.col_idx[i];
                    if (Gamma.find(neighbor) != Gamma.end()) {
                        node1_count++;
                    }
                }
                for (int i=node2_start; i < node2_end; i++) {
                    int neighbor = csr_reverse.col_idx[i];
                    if (Gamma.find(neighbor) != Gamma.end()) {
                        node2_count++;
                    }
                }

                return node1_count < node2_count;
            });
    });
    g2.wait();
    // cout << "new_tail: ";
    // for (int ele : new_tail) {
    //     cout << ele << " ";
    // }
    // cout << endl;

    int tail_size = new_tail.size();

    tbb::task_group g3;
    
    g3.run([&] {
        /* Parallel do the vertex in the new_tail */
        tbb::parallel_for(0, tail_size, 1, 
            [&](int i) {
                
                R_set XVSet;
                L_set XVNeighbor;
                hash_map freqHash;
                /* Create the local ntail_set */
                R_set ntail_set;
            
                for (int j=i+1; j < tail_size; j++) {
                    ntail_set.insert(new_tail[j]);
                }

                int vertex = new_tail[i];
                // cout << "------------- <<vertex: " << vertex << "------------------" << endl;

                set_copy(XVSet, X);
                XVSet.insert(vertex);

                if ((int)(XVSet.size() + ntail_set.size()) >= ms) {

                    // XVNeighbor = findCommonNeighbor(csr_reverse, XVSet);
                    /* XVNeighbor = Gamma(X) and Gamma(vertex) */
                    /* vertex is in R_set, so search csr_reverse */
                    int vertex_start, vertex_end;
                    searchNeighbor(csr_reverse, vertex, vertex_start, vertex_end);
                    for (int i=vertex_start; i < vertex_end; i++) {
                        int neighbor = csr_reverse.col_idx[i];
                        if (Gamma.find(neighbor) != Gamma.end()) {
                            XVNeighbor.insert(neighbor);
                        }
                    }
                    
                    tbb::task_group g4;
                    g4.run([&] {
                        /* Parallel find 2-hop common neighbor */
                        tbb::parallel_for_each(XVSet.begin(), XVSet.end(), 
                            [&](int node) {
                                hash_map::accessor a;
                                std::pair<int, int> node_pair(node, 0);
                                freqHash.insert(a, node_pair);
                            });
                    });
                    g4.wait();

                    tbb::task_group g5;
                    g5.run([&] {
                        tbb::parallel_for_each(XVNeighbor.begin(), XVNeighbor.end(), 
                            [&](int neighbor) {
                                hash_map::accessor a;
                                /* neighbor is in L_set, search neighbor's neighbor from csr */
                                int neighbor_start, neighbor_end;
                                searchNeighbor(csr, neighbor, neighbor_start, neighbor_end); /* Search from R to L */
                                
                                for (int i=neighbor_start; i < neighbor_end; i++) {
                                    /* neighbor_neighbor is in R_set */
                                    int neighbor_neighbor = csr.col_idx[i];
                                    if (freqHash.insert(a, neighbor_neighbor)) {
                                        a->second = 1;
                                    } else {
                                        a->second++;
                                    }

                                    /* NOTE: find() does not lock the bucket it found !!!! */
                                    // if (freqHash.find(a, neighbor_neighbor)) {
                                    //     a->second++;
                                    // } 
                                    // else { /* If the neighbor is not previous in XVSet */
                                    //     std::pair<int, int> node_pair(neighbor_neighbor, 1);
                                    //     freqHash.insert(a, node_pair);
                                    // }
                                }
                            });
                    });
                    g5.wait();

                    /* Generating Y by find the key(vertex ID) whose value is equal to XVNeighborSize */
                    R_set Y;
                    int XVNeighborSize = XVNeighbor.size();

                    // cout << "(" << XVNeighborSize <<") freqHash: ";
                    if (XVNeighborSize > 0) {

                        tbb::task_group g6;
                        g6.run([&] {
                            tbb::parallel_for(freqHash.range(),
                                [&](const hash_map::range_type &r) {
                                    for (hash_map::iterator i = r.begin(); i != r.end(); i++) {
                                        // cout << "(" << i->first << ", " << i->second << "), ";
                                        if (i->second == XVNeighborSize) {
                                            Y.insert((int)i->first);
                                        }
                                    }
                                });
                        });
                        g6.wait();
                        // cout << endl;
                    }
                    // cout << "iteration[" <<  i << "]: " << vertex  << " ";
                    // cout << "XVSet: ";
                    // print_set(XVSet);
                    // cout << "XVNeighbor: ";
                    // print_set(XVNeighbor);
                    /* Print Y */
                    // cout << "Y: " << "Neighbor size: " << XVNeighborSize << endl;
                    // print_set(Y);
                    // cout << "XVNeighbor: " << endl;
                    // print_set(XVNeighbor);

                    /* if Y \ (X U {v}) contains(<=) ntail(X) */
                    R_set temp_Y = set_subtract(Y, XVSet);
                    /* Print temp_Y */
                    // cout << "temp_Y: ";
                    // print_set(temp_Y);
                    // cout << "ntail_set: ";
                    // print_set(ntail_set);
                    // cout << "temp_Y is subset of ntail_set: " << set_subset(temp_Y, ntail_set) << endl;
                    /* If temp_Y is subset of ntail_set */


                    if (set_subset(temp_Y, ntail_set)) {
                        /* If |Y| > ms */
                        if ((int)Y.size() >= ms) {
                            
                            /* MBE is found !!!! */
                            // cout << "MBE is found!!!! " << endl;
                            insert_final_lock.lock();
                            mbe_num++;
                            // /* Print MBE */
                            // cout << "************ << MBE: " << mbe_num << " >>**********" << endl;
                            // print_set(Y);
                            // print_set(XVNeighbor);
                            insert_final_lock.unlock();

                        }
                        
                        R_set next_tail = set_subtract(ntail_set, Y);
                        
                        /* Recursive call parlmbc */
                        // cout << "Recursive call parlmbc " << endl;
                        // cout << "next_tail: ";
                        // print_set(next_tail);
                        parlmbc(Y, XVNeighbor, next_tail, ms);
                        // cout << "Recursive call parlmbc finished" << endl;
                    }
                
                }
            });
    });      
    g3.wait();
}


/* Assume |L| >　|R| */
void
parlmbc_main(string file_name)
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

    parlmbc(X, Gamma, tail, ms);

    gettimeofday(&end, NULL);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1.0E-6;
    cout << "Execution time: " << time << " sec" << endl;

    cout << "MBE number: " << mbe_num << endl;
}
