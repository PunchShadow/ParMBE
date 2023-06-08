#include <iostream>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_vector.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_hash_map.h>
#include <stdio.h>
#include <string>
#include <set>
#include "BiGraph.hpp"
#include "LocalState.hpp"
#include "VertexSet.hpp"
#include "parlmbc.hpp"
#include <sys/time.h>

#ifdef DEBUG
#include <assert.h>
#endif /* DEBUG */


using namespace std;

static bool tailIsLeft = false;
static int num_thread = tbb::task_scheduler_init::default_num_threads();

/* Mutex locks */
tbb::spin_mutex sort_tail_lock;
tbb::spin_mutex insert_mbe_lock;
tbb::spin_mutex insert_final_lock;


void
remove_tail(BiGraph *BPG, VertexSet *X, VertexSet *new_tail, int node, int ms)
{
    VertexSet XVSet;
    XVSet.deep_copy(X);
    if (tailIsLeft) XVSet.insert_L(node);
    else XVSet.insert_R(node);

    VertexSet *v_neighbor;
    /* These only get the node's neighbor */
    v_neighbor = BPG->GetNeighborSet(&XVSet, tailIsLeft);
    if (v_neighbor->GetSize() >= ms) {
        tbb::spin_mutex::scoped_lock lock(sort_tail_lock);
        if (tailIsLeft) {
            new_tail->insert_L(node);
        }
        else {
            new_tail->insert_R(node);
        }
    }
}

void
unroll_iterative_recursive(tbb::concurrent_vector<int> *vec, int num_iter, LocalState* ls)
{
    /* ntail(X) = tail(i+1,...,k); ntail = vec->at[start: start + stride] */
    VertexSet *ntail_set = new  VertexSet();
    for (int i = num_iter+1; i < (int)vec->size(); i++) {
        if (tailIsLeft) ntail_set->insert_L(vec->at(i));
        else ntail_set->insert_R(vec->at(i));
    }

    /* Copy the ls to new_ls */
    LocalState new_ls;
    new_ls.setVertexSet(ls->getX());
    new_ls.setTail(ls->getTail());
    new_ls.setBiGraph(ls->getBiGraph());
    int ms = ls->getMs();
    tbb::concurrent_hash_map<int, int> *XVNeighborNeighbor = new tbb::concurrent_hash_map<int, int>();
    VertexSet *XVNeighbor;
    VertexSet *XWVertex = new VertexSet();
    XWVertex->deep_copy(new_ls.getX());
    int XVNeighborSize;
        
    int vertex = vec->at(num_iter);

    if (tailIsLeft) XWVertex->insert_L(vertex);
    else XWVertex->insert_R(vertex);

    /* if |(X U {v})| + |ntail(X)| >= ms */
    if ((int)(XWVertex->GetSize() + ntail_set->GetSize()) >= ms) {
        
        /* Parallel find the 2-hop common neighbor set */
        /* Still need to get the common neighbor of {X U {v}} */
        new_ls.CalculateNeighbor(vertex, tailIsLeft);
        XVNeighbor = new_ls.getXVNeighbor();
        XVNeighborSize = XVNeighbor->GetSize();

        /* Check if the XWVertex belongs to L set or R set */
        tbb::concurrent_unordered_set<int> *set_ptr;
        if (tailIsLeft) set_ptr = XWVertex->GetLset();
        else set_ptr = XWVertex->GetRset();

        /* Create concurrent_hash_map by parallely inserting the node from XWVertex */
        tbb::parallel_for_each(set_ptr->begin(), set_ptr->end(), [&](int node){
            tbb::concurrent_hash_map<int, int>::accessor a;
            std::pair<int, int> node_pair(node, 0);
            XVNeighborNeighbor->insert(a, node_pair);
        });        
        


        /* Parallely iterate the key in XVNeighborNeighbor 
        and update frequency of the value with the number of neighors. */
        if (tailIsLeft) set_ptr = XVNeighbor->GetRset();
        else set_ptr = XVNeighbor->GetLset();
        tbb::parallel_for_each(set_ptr->begin(), set_ptr->end(), [&](int node){
            tbb::concurrent_hash_map<int, int>::accessor a;
            VertexSet *temp_neighbor = new_ls.getBiGraph()->GetNeighbor(node, !tailIsLeft);

            if (tailIsLeft) {
                for (auto it = temp_neighbor->GetLset()->begin(); it != temp_neighbor->GetLset()->end(); it++) {
                    if (XVNeighborNeighbor->find(a, (int)*it)) {
                        a->second++;
                    } else {
                        std::pair<int, int> node_pair(*it, 1);
                        XVNeighborNeighbor->insert(a, node_pair);
                    }
                }
            } else {
                for (auto it = temp_neighbor->GetRset()->begin(); it != temp_neighbor->GetRset()->end(); it++) {
                    if (XVNeighborNeighbor->find(a, (int)*it)) {
                        a->second++;
                    } else {
                        std::pair<int, int> node_pair(*it, 1);
                        XVNeighborNeighbor->insert(a, node_pair);
                    }
                }
            }
        });

        /* Generating Y by find the key(vertex ID) whose value is equal to XVNeighborSize */
        VertexSet *Y = new VertexSet();
        if (XVNeighborSize > 0) {
            tbb::parallel_for_each(XVNeighborNeighbor->begin(), XVNeighborNeighbor->end(),
                [&](const std::pair<int, int>& p) {
                    if (p.second == XVNeighborSize) {
                        /* Use spin lock to ensure the consistency */
                        tbb::spin_mutex::scoped_lock lock(insert_mbe_lock);
                        if (tailIsLeft) {
                            Y->insert_L(p.first);
                        } else {
                            Y->insert_R(p.first);
                        }
                    }
                });
        }


        /* if Y \ (X U {v}) contains(<=) ntail(X) */
        VertexSet temp_Y; /* Y \ (X U {v}) */
        temp_Y.deep_copy(Y);
        temp_Y -= *XWVertex;


        #ifdef DEBUG
        /* Print the XWVertex */
        cout << "XWVertex: ------------------------------------------" << endl;
        XWVertex->PrintStatus();
        /* Print the XVNeighborNeighbor size and all the elements */
        cout << vertex << ": XVNeighborNeighbor: " << "XVNeighborSize = " << XVNeighborSize << endl;
        for (auto it = XVNeighborNeighbor->begin(); it != XVNeighborNeighbor->end(); it++) {
            cout << it->first << ": " << it->second << endl;
        }
        /* Print XVNeighbor */
        cout << "XVNeighbor: " << endl;
        XVNeighbor->PrintStatus();
        /* Print the status of Y */
        cout << "Y: " << endl;
        Y->PrintStatus();
        cout << "temp_Y: " << endl;
        temp_Y.PrintStatus();
        /* Print ntail_set */
        cout << "ntail_set: " << endl;
        ntail_set->PrintStatus();
        #endif /* DEBUG */



        if (temp_Y <= *(ntail_set)) {
            if ((int)(Y->GetSize()) >= ms) {
                /* Generating new biclique set and inserting to BiGraph with concurrent_hash_map. */
                // VertexSet *newMBE = new VertexSet();
                if (Y->GetSize() > 0) {
                    VertexSet *newMBE = new VertexSet();
                    newMBE->deep_copy(Y);
                    *newMBE += *(new_ls.getXVNeighbor());
                    /* Use spin lock for ensure mbe insertion and number count */
                    #ifdef DEBUG
                    cout << "MBE Found!!!: " << endl;
                    newMBE->PrintStatus();
                    #endif /* DEBUG */
                    tbb::spin_mutex::scoped_lock lock(insert_final_lock);
                    new_ls.getBiGraph()->InsertToFinal(newMBE);
                }
            }

            VertexSet newTail = *(ntail_set) - *(Y);
            VertexSet *newX = new VertexSet();
            *newX = *(Y);

            parlmbc(new_ls.getBiGraph(), newX, new_ls.getXVNeighbor(), &newTail, ms);
        } /* if Y \ (X U {v}) contains(<=) tail(X) */
    } /* if |(X U {v})| + |tail(X)| >= ms */

    /* Reset the hash_map for next iteration. */
    XVNeighborNeighbor->clear();

}


void
parlmbc(BiGraph *BPG, VertexSet *X, VertexSet *tau, VertexSet *tail, int ms)
{
    LocalState ls;
    ls.setVertexSet(X);
    ls.setBiGraph(BPG);
    ls.setMs(ms);
    ls.setTau(tau);
    ls.setTail(tail);

    VertexSet *new_tail = new VertexSet();
    VertexSet *temp_tau = new VertexSet();
    temp_tau->deep_copy(ls.getTau()); /* temp_vertexset = Tau(X) */

    VertexSet *temp_tail = ls.getTail();


    /* Parallel removing the element which has 0 common neighbor with X in the tail set */
    if (tailIsLeft == true && temp_tail->GetLset() > 0) {
        tbb::parallel_for_each(temp_tail->GetLset()->begin(), temp_tail->GetLset()->end(), [&](int node){
            remove_tail(BPG, ls.getVertexSet(), new_tail, node, ms);
        });
    } else {
        tbb::parallel_for_each(temp_tail->GetRset()->begin(), temp_tail->GetRset()->end(), [&](int node){
            remove_tail(BPG, ls.getVertexSet(), new_tail, node, ms);
        });
    }
    #ifdef DEBUG
    /* Print the old tail and new tail */
    cout << "Old tail: " << endl;
    temp_tail->PrintStatus();
    cout << "New tail (after remove): " << endl;
    new_tail->PrintStatus();
    #endif /* DEBUG */
    ls.setTail(new_tail);
    /* Check if |X| + |tail(X)| < ms: terminal condition */
    int X_size = ls.getX()->GetSize();
    int tail_size = ls.getTail()->GetSize();
    if ((int)(X_size + tail_size) < ms) {
        return;
    }   

    temp_tail = ls.getTail();
    tbb::concurrent_vector<int> *tail_vector = new tbb::concurrent_vector<int>();

    if (temp_tail->GetLsetSize() > 0) {
        for (auto it = temp_tail->GetLset()->begin(); it != temp_tail->GetLset()->end(); it++) {
            tail_vector->push_back(*it);
        }
    } else if (temp_tail->GetRsetSize() > 0) {
        for (auto it = temp_tail->GetRset()->begin(); it != temp_tail->GetRset()->end(); it++) {
            tail_vector->push_back(*it);
        }
    }

    /* Parallel sorting the vertices of tail(X) (tail_vector) into ascending order of |tau(X u {v})| */
    if (temp_tail->GetSize() > 0) {
        tbb::parallel_sort(tail_vector->begin(), tail_vector->end(), [&](int a, int b) {
            return ls.getBiGraph()->GetNeighbor(a, true)->GetSize() < ls.getBiGraph()->GetNeighbor(b, true)->GetSize();
        });
    }

    tbb::parallel_for(0, tail_size, 1, [&](int i){
        unroll_iterative_recursive(tail_vector, i, &ls);
    });

}

void
parlmbc_main(string file_path)
{
    /* Initialization */
    tbb::task_scheduler_init init(num_thread);
    static BiGraph *BPG = new BiGraph();
    BPG->read(file_path, 2, 0);

    BPG->PrintSetSize();

    /* Initilization */
    tbb::concurrent_unordered_set<int> X_L_set;
    tbb::concurrent_unordered_set<int> X_R_set;
    VertexSet X = VertexSet(&X_L_set, &X_R_set);
    int ms = 1;

    LocalState ls;
    VertexSet *tau = new VertexSet();
    VertexSet *tail = new VertexSet();

    ls.setVertexSet(&X);
    ls.setBiGraph(BPG);
    if (BPG->getLSetSize() > BPG->getRSetSize()) {
        cout << "--------<< Tau is in L_set >>--------" << endl;
        cout << "--------<< Tail is in R_set >>--------" << endl;
        tau->SetLset(BPG->getLSet());
        tail->SetRset(BPG->getRSet());
    } else {
        cout << "--------<< Tau is in R_set >>--------" << endl;
        cout << "--------<< Tail is in L_set >>--------" << endl;
        tau->SetRset(BPG->getRSet());
        tail->SetLset(BPG->getLSet());
        tailIsLeft = true;
    }
    ls.setTau(tau);
    ls.setTail(tail);


    /* Calculate execution time of minelmbc_thread() */
    struct timeval start, end;
    gettimeofday(&start, NULL);

    parlmbc(BPG, &X, ls.getTau(), ls.getTail(), ms);

    gettimeofday(&end, NULL);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1.0E-6;
    cout << "Execution time: " << time << " sec" << endl;


    BPG->PrintFinalB();
}