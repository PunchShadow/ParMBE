#include <iostream>
#include <tbb/tbb.h>
#include <tbb/parallel_for.h>
#include <tbb/concurrent_unordered_set.h>
#include <stdio.h>
#include <string>
#include <set>
#include "BiGraph.hpp"
#include "LocalState.hpp"
#include "VertexSet.hpp"
#include "minelmbc.hpp"
#include <sys/time.h>

static bool tailIsLeft = false;


void fmbe_main(string file_path)
{
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

    VertexSet *new_tail = new VertexSet();

    /* Use rank to prune the tail set */
    /* Use the size of E_set*/
    /* For all elements in tail */
    VertexSet* tail_ptr = ls.getTail();
    tbb::concurrent_unordered_set<int>::iterator v;
    VertexSet *tau_w;
    int v_degree, y_degree;

    struct timeval start, end;
    gettimeofday(&start, NULL);

    if (tailIsLeft) {
        for (v = tail_ptr->GetLset()->begin(); v != tail_ptr->GetLset()->end(); v++) {
            X.insert_L(*v);
            ls.setVertexSet(&X);
            ls.CalculateNeighborNeighbor(*v, true);
            ls.setTau(ls.getXVNeighbor());
            v_degree = BPG->getDegree(*v, true);

            for (auto w = ls.getTau()->GetRset()->begin(); w != ls.getTau()->GetRset()->end(); w++) {
                tau_w = BPG->GetNeighbor(*w, false);
                
                for (auto y = tau_w->GetLset()->begin(); y != tau_w->GetLset()->end(); y++) {
                    y_degree = BPG->getDegree(*y, true);
                    if (y_degree > v_degree) {
                        new_tail->insert_L(*y);
                    } else if (y_degree == v_degree) {
                        if (*y > *v) {
                            new_tail->insert_L(*y);
                        }
                    }
                }
            }

            minelmbc_thread(BPG, &X, ls.getTau(), new_tail, ms);

        }
    } else {
        for (v = tail_ptr->GetRset()->begin(); v != tail_ptr->GetRset()->end(); v++) {
            X.insert_R(*v);
            ls.setVertexSet(&X);
            ls.CalculateNeighborNeighbor(*v, false);
            ls.setTau(ls.getXVNeighbor());
            v_degree = BPG->getDegree(*v, false);

            for (auto w = ls.getTau()->GetLset()->begin(); w != ls.getTau()->GetLset()->end(); w++) {
                tau_w = BPG->GetNeighbor(*w, true);
                
                for (auto y = tau_w->GetRset()->begin(); y != tau_w->GetRset()->end(); y++) {
                    y_degree = BPG->getDegree(*y, false);
                    if (y_degree > v_degree) {
                        new_tail->insert_R(*y);
                    } else if (y_degree == v_degree) {
                        if (*y > *v) {
                            new_tail->insert_R(*y);
                        }
                    }
                }
            }

            minelmbc_thread(BPG, &X, ls.getTau(), new_tail, ms);
        }
    }








    gettimeofday(&end, NULL);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1.0E-6;
    cout << "Execution time: " << time << " sec" << endl;


    BPG->PrintFinalB();
}