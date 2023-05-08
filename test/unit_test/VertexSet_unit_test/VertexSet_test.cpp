#include "VertexSet.hpp"
#include "BiGraph.hpp"
#include "tbb/tbb.h"
#include "tbb/concurrent_unordered_set.h"
#include <iostream>


using namespace std;


int main()
{
    /* BiGraph */
    BiGraph *BPG = new BiGraph();
    

    /* Test operator overloading in VertexSet.hpp */
    VertexSet *VS = new VertexSet();

    /* Make L_set and R_set */
    tbb::concurrent_unordered_set<int> *L_set = new tbb::concurrent_unordered_set<int>();
    tbb::concurrent_unordered_set<int> *R_set = new tbb::concurrent_unordered_set<int>();

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

    BPG->AddEdge(2, 4);
    BPG->AddEdge(2, 5);


    /* Add vertices to L_set and R_set in BPG */
    L_set->insert(1);
    L_set->insert(2);
    L_set->insert(3);

    R_set->insert(4);
    R_set->insert(5);
    R_set->insert(6);
    R_set->insert(7);
    R_set->insert(8);

    /* Set L_set and R_set to VS */
    VS->SetLset(L_set);
    VS->SetRset(R_set);

    /* Test operator overloading in VertexSet.hpp */
    std::cout << "Test operator overloading in VertexSet.hpp" << std::endl;

    /* Print VS */
    cout << "VS: " << endl;
    VS->PrintStatus();

    /* Test = operator */
    std::cout << "***** << Test = operator >> *******" << std::endl;
    VertexSet *VS2 = new VertexSet();
    VS2 = VS;
    VS2->PrintStatus();

    /* Test += operator */
    std::cout << "***** << Test += operator >> *******" << std::endl;
    VertexSet *VS3 = new VertexSet();
    /* Create new set */
    tbb::concurrent_unordered_set<int> *L_set3 = new tbb::concurrent_unordered_set<int>();
    tbb::concurrent_unordered_set<int> *R_set3 = new tbb::concurrent_unordered_set<int>();
    /* Add vertices to L_set and R_set differing from L_set and R_set */
    L_set3->insert(10);
    L_set3->insert(11);
    L_set3->insert(12);

    R_set3->insert(13);
    R_set3->insert(14);
    R_set3->insert(15);
    R_set3->insert(16);

    /* Set L_set and R_set to VS */
    VS3->SetLset(L_set3);
    VS3->SetRset(R_set3);

    *VS2 += *VS3;
    VS2->PrintStatus();

    




    return 0;


}