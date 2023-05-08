#include "LocalState.hpp"
#include "VertexSet.hpp"
#include "BiGraph.hpp"
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>

#include <iostream>



/* Unit test for LocalState.hpp */

int main()
{
    /* Test findNeighborWNode() function */
    std::cout << "Test findNeighborWNode() function" << std::endl;
    LocalState *LS = new LocalState();
    BiGraph *BPG = new BiGraph();
    
    /* Add vertices to L_set and R_set in BPG */
    BPG->AddVertexL(1);
    BPG->AddVertexL(2);
    BPG->AddVertexL(3);

    BPG->AddVertexR(4);
    BPG->AddVertexR(5);
    BPG->AddVertexR(6);
    BPG->AddVertexR(7);
    BPG->AddVertexR(8);

    /* Add edges to BPG */
    BPG->AddEdge(1, 4);
    BPG->AddEdge(1, 5);
    BPG->AddEdge(1, 6);
    BPG->AddEdge(1, 7);
    BPG->AddEdge(1, 8);

    BPG->AddEdge(2, 4);
    BPG->AddEdge(2, 5);
    BPG->AddEdge(2, 6);

    BPG->AddEdge(3, 4);

    /* Set BPG to LS */
    LS->setBiGraph(BPG);

    /* Set tail of LS */
    VertexSet *tail = new VertexSet();
    tbb::concurrent_unordered_set<int> *L_set = new tbb::concurrent_unordered_set<int>();
    tbb::concurrent_unordered_set<int> *R_set = new tbb::concurrent_unordered_set<int>();

    L_set->insert(1);
    L_set->insert(2);
    L_set->insert(3);


    tail->SetLset(L_set);
    tail->SetRset(R_set);

    LS->setTail(tail);

    /* Test the unsafe_remove_R() and unsafe_remove_L() of the LocalState */
    std::cout << "Test the unsafe_remove_R() and unsafe_remove_L() of the LocalState" << std::endl;
    // LS->PrintStatus();
    /* Use for to print the tail in LS */
    for (auto it = LS->getTail()->GetLset()->begin(); it != LS->getTail()->GetLset()->end(); it++)
    {
        std::cout << *it << " ";
    }

    std::cout << std::endl;

    for (auto it = LS->getTail()->GetRset()->begin(); it != LS->getTail()->GetRset()->end(); it++)
    {
        std::cout << *it << " ";
    }

    std::cout << std::endl;

    /* Remove on element in tail of LS */
    LS->getTail()->unsafe_remove_R(4);
    LS->getTail()->unsafe_remove_L(1);
    LS->getTail()->unsafe_remove_L(2);
    LS->getTail()->unsafe_remove_L(3);

    /* Use for to print the tail in LS */
    for (auto it = LS->getTail()->GetLset()->begin(); it != LS->getTail()->GetLset()->end(); it++)
    {
        std::cout << *it << " ";
    }

    std::cout << std::endl;

    for (auto it = LS->getTail()->GetRset()->begin(); it != LS->getTail()->GetRset()->end(); it++)
    {
        std::cout << *it << " ";
    }

    std::cout << std::endl;

    return 0;


}







/* Test findNeighborWSet() function */