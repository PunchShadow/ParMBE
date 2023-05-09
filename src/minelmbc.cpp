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
#include <sys/time.h>

#ifdef DEBUG
#include <assert.h>
#endif /* DEBUG */


using namespace std;

static bool tailIsLeft = false;
static double num_mbe = 0;

void 
minelmbc_thread(BiGraph *BPG, VertexSet *X, VertexSet *tau, VertexSet *tail, int ms) {
    
    LocalState ls;
    ls.setVertexSet(X);
    ls.setBiGraph(BPG);
    ls.setMs(ms);
    ls.setTau(tau);
    ls.setTail(tail);

    VertexSet *new_tail = new VertexSet();

    VertexSet *temp_tail = ls.getTail();

#ifdef DEBUG
    /* Check if the tail is one-side VertexSet. */
    CHECK_VERTEXSET_IS_ONE_SIDE(temp_tail);
    cout << "temp_tail: " << endl;
    temp_tail->PrintStatus();
#endif /* DEBUG */

    /* Iteratively access the elements in tail using for loop. */ 
    // tbb::concurrent_unordered_set<int>::iterator it;


    /* FIXME: Removing from tail has something wrong, the remove will access the element which is not in the L_set or R_set */
    // cout << "Remove vertices from tail" << endl;
    if (tailIsLeft) {
        for (auto it = temp_tail->GetLset()->begin(); it != temp_tail->GetLset()->end(); it++) {
#ifdef DEBUG
            cout << *it << " ";
#endif /* DEBUG */
            ls.CalculateNeighbor(*it, true);
            if (ls.getXVNeighbor()->GetSize() >= ms) {
#ifdef DEBUG
                cout << "Insert (L) " << *it << " to new tail" << endl;
                // ls.getTail()->unsafe_remove_L(*it);
#endif /* DEBUG */
                new_tail->insert_L(*it);        
            }
        }
    } else {
        for (auto it = temp_tail->GetRset()->begin(); it != temp_tail->GetRset()->end(); it++) {
            ls.CalculateNeighbor(*it, false);
            if (ls.getXVNeighbor()->GetSize() >= ms) {
#ifdef DEBUG
                cout << "Insert (R) " << *it << " to tail" << endl;
#endif  /* DEBUG */
                new_tail->insert_R(*it);
                //ls.getTail()->unsafe_remove_R(*it);
            
            }
        }
    }

    ls.setTail(new_tail);
    


    /* Check if |X| + |tail(X)| < ms: terminal condition */
    if ((int)(ls.getTail()->GetSize() + ls.getX()->GetSize()) < ms) {
        return;
    }   

    /* Sort vertices of tail(X) into ascending order of |tau(X U {v})| */
    /* Insert all elements in tail to tail_vector and sort the tail_vector */
    temp_tail = ls.getTail();
    std::vector<int> *tail_vector = new std::vector<int>();
    // tbb::concurrent_vector<int> *tail_vector = ls.getTailVector();

#ifdef DEBUG
    CHECK_VERTEXSET_IS_ONE_SIDE(temp_tail);
    cout << "Tail (after remove): " << endl;
    temp_tail->PrintStatus();
#endif /* DEBUG */

    if (temp_tail->GetLsetSize() > 0) {
        for (auto it = temp_tail->GetLset()->begin(); it != temp_tail->GetLset()->end(); it++) {
            tail_vector->push_back(*it);
        }
    } else {
        for (auto it = temp_tail->GetRset()->begin(); it != temp_tail->GetRset()->end(); it++) {
            tail_vector->push_back(*it);
        }
    }

    /* Sort the tail_vector */
    std::sort(tail_vector->begin(), tail_vector->end(), [&](int a, int b) {
        return ls.getBiGraph()->GetNeighbor(a, true)->GetSize() < ls.getBiGraph()->GetNeighbor(b, true)->GetSize();
    });

    /* Check unique */
    auto itt = std::unique(tail_vector->begin(), tail_vector->end());
    bool wasUnique = (itt == tail_vector->end());
    if (!wasUnique) exit(0);

    /* TBB parallel sort version */
    // tbb::parallel_sort(tail_vector->begin(), tail_vector->end(), [&](int a, int b) {
    //     return ls.getBiGraph()->GetNeighbor(a, true)->GetSize() > ls.getBiGraph()->GetNeighbor(b, true)->GetSize();
    // });

    /* for x in tail_vector */
    for (int i = 0; i < (int)tail_vector->size(); i++) {
        /* FIXME: there are two same vertex appear at the same time */
        int vertex = (int)(*tail_vector)[i];
        
        
        /* tail(X) <- tail(X) \ {v} */
        if (tailIsLeft) ls.getTail()->unsafe_remove_L(vertex);
        else ls.getTail()->unsafe_remove_R(vertex);


#ifdef DEBUG

#endif /* DEBUG */
        /* XWVertex = X U {v} */
        VertexSet XWVertex;
        XWVertex = *(ls.getX());
        if (tailIsLeft) XWVertex.insert_L(vertex);
        else XWVertex.insert_R(vertex);

        /* if |(X U {v})| + |tail(X)| >= ms */
        if ((int)(XWVertex.GetSize() + ls.getTail()->GetSize()) > ms) {

            ls.CalculateNeighborNeighbor(vertex, tailIsLeft);
#ifdef DEBUG
            VertexSet *Y_ptr = ls.getXVNeighborNeighbor();
            assert(Y_ptr->GetLsetSize() == 0 || Y_ptr->GetRsetSize() == 0);
            CHECK_VERTEXSET_IS_ONE_SIDE(ls.getXVNeighborNeighbor());
            CHECK_VERTEXSET_IS_ONE_SIDE(ls.getXVNeighbor());
            CHECK_VERTEXSET_IS_ONE_SIDE(ls.getTail());
            cout << "*******************" << "vertex:" << vertex << "****************" << endl;
            cout << "ls.geTail: " << endl;
            ls.getTail()->PrintStatus();
            cout << "X: " << endl;
            ls.getX()->PrintStatus();
            cout << "ls.getXVNeighbor: " << endl;
            ls.getXVNeighbor()->PrintStatus();
            cout << "ls.getXVNeighborNeighbor: " << endl;
            ls.getXVNeighborNeighbor()->PrintStatus();
#endif /* DEBUG */

            ls.setY(ls.getXVNeighborNeighbor());

#ifdef DEBUG
            CHECK_VERTEXSET_IS_ONE_SIDE(ls.getY());
#endif /* DEBUG */

            /* XWVertex = X U {v} */            
            /* ls.getY <- ls.getY \ (X U {v}) */
            VertexSet temp_Y = *(ls.getY()) - (XWVertex);
#ifdef DEBUG
            VertexSet *temp_y_ptr = &temp_Y;
            CHECK_VERTEXSET_IS_ONE_SIDE(temp_y_ptr);
            /* Print temp_Y */
            cout << "Y: " << endl;
            ls.getY()->PrintStatus();
            cout << "temp_Y:" << endl;
            temp_Y.PrintStatus();
#endif /* DEBUG */

            /* if Y \ (X U {v}) contains(<=) tail(X) */
            if (temp_Y <= *(ls.getTail())) {

                if ((int)(ls.getY()->GetSize()) >= ms) {
                    // cout << "MBE Found!" << endl;
                    /* Generating new biclique set and inserting to Bigraph. */
                    VertexSet *newMBE = new VertexSet();
                    if (ls.getY()->GetSize() > 0) {
                        #ifdef DEBUG
                        CHECK_VERTEXSET_IS_ONE_SIDE(ls.getY());
                        CHECK_VERTEXSET_IS_ONE_SIDE(ls.getXVNeighbor());
                        #endif /* DEBUG */
                        *newMBE = *(ls.getY());
                        *newMBE += *(ls.getXVNeighbor());
                        ls.getBiGraph()->InsertToFinal(newMBE);
                        num_mbe++;
                    }
#ifdef STATUS   
                    cout << "newMBE: " << num_mbe << endl;
                    newMBE->PrintStatus();
#endif /* STATUS */
                    
                }

                VertexSet newTail = *(ls.getTail()) - *(ls.getY()); /* tail(X) \ Y */

                VertexSet *newX = new VertexSet(); /* Y */
                *newX = *(ls.getY());


#ifdef DEBUG
                CHECK_VERTEXSET_IS_ONE_SIDE(newX);
                CHECK_VERTEXSET_IS_ONE_SIDE(ls.getY());
                CHECK_VERTEXSET_IS_ONE_SIDE((&newTail));
                cout << "newX: " << endl;
                newX->PrintStatus();
#endif /* DEBUG */

                /* Recursive call */
                // cout << "Recursive call" << endl;
                minelmbc_thread(BPG, 
                                newX, 
                                ls.getXVNeighbor(), /* tau(X U {v}) */
                                &newTail, 
                                ls.getMs());
                /* Garbage collection */
                delete newX;

            }
        
        }
    }
    
    /* Garbage collection */
    //delete tail_vector;
    


    // cout << "RETURN from an thread function !!!!!!!!!!!" << endl;
}


void
minelmbc_main(string file_path)
{
    static BiGraph *BPG = new BiGraph();
    BPG->read(file_path, 2, 0);

    BPG->PrintSetSize();
    // BPG->PrintLNeighborList();
    // BPG->PrintRNeighborList();

    /* Initilization */
    tbb::concurrent_unordered_set<int> X_L_set;
    tbb::concurrent_unordered_set<int> X_R_set;
    VertexSet X = VertexSet(&X_L_set, &X_R_set);
    int ms = 1;

    // cout << "setPtr size: " << setPtr->size() << endl;
    // LocalState ls(&X, BPG.getRSet(), BPG.getLSet(), 1);
    // int i = 0;
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

    // cout << "RSet: " << endl;
    // for (auto it = BPG->getRSet()->begin(); it != BPG->getRSet()->end(); it++) {
    //     cout << *it << " ";
    // }
    // cout << endl;
    // cout << "LSet: " << endl;
    // for (auto it = BPG->getLSet()->begin(); it != BPG->getLSet()->end(); it++) {
    //     cout << *it << " ";
    // }
    // cout << endl;

    /* Calculate execution time of minelmbc_thread() */
    struct timeval start, end;
    gettimeofday(&start, NULL);

    minelmbc_thread(BPG, &X, ls.getTau(), ls.getTail(), ms);

    gettimeofday(&end, NULL);
    double time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1.0E-6;
    cout << "Execution time: " << time << " sec" << endl;


    BPG->PrintFinalB();
    
}