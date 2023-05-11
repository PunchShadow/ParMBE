/* Unit test for BiGraph.hpp */
#include "BiGraph.hpp"
#include "VertexSet.hpp"
#include <iostream>
#include <string>

using namespace std;

// static bool tailIsLeft = false;


int main(int argc, char *argv[])
{
    /* Test GetNeighbor() function. */
    /* Initialize BPG */
    static BiGraph *BPG = new BiGraph();
    const string file_path = argv[1];
    
    /* Use read() function with argv */
    BPG->read(file_path, 2, 0);

    /* Test GetNeighbor() function */
    cout << "Test GetNeighbor() function" << endl;
    
    VertexSet *result;
    result = BPG->GetNeighbor(1, true);
    cout << "Find 1's neighbor: " << endl;
    result->PrintStatus();

    cout << "Find 2's neighbor: " << endl;
    result = BPG->GetNeighbor(2, true);
    result->PrintStatus();

    cout << "Find 3's neighbor: " << endl;
    result = BPG->GetNeighbor(3, true);
    result->PrintStatus();

    cout << "Find 5's neighbor: " << endl;
    result = BPG->GetNeighbor(5, false);
    result->PrintStatus();

    cout << "Find 6's neighbor: " << endl;
    result = BPG->GetNeighbor(6, false);
    result->PrintStatus();

    cout << "Find 7's neighbor: " << endl;
    result = BPG->GetNeighbor(7, false);
    result->PrintStatus();


    /* Test GetNeighborSet() */
    cout << "===============<< Test GetNeighborSet() function >> ================" << endl;
    VertexSet *VS1 = new VertexSet();

    /* Find {1,2}'s common neighbor */
    VS1->clear();
    VS1->insert_L(1);
    VS1->insert_L(2);
    result = BPG->GetNeighborSet(VS1, true);
    cout << "Find {1,2}'s common neighbor: " << endl;
    result->PrintStatus();

    /* Find {1,2,3}'s common neighbor */
    VS1->insert_L(3);
    result = BPG->GetNeighborSet(VS1, true);
    cout << "Find {1,2,3}'s common neighbor: " << endl;
    result->PrintStatus();

    VS1->clear();
    VS1->insert_L(2);
    VS1->insert_L(3);

    result = BPG->GetNeighborSet(VS1, true);
    cout << "Find {2,3}'s common neighbor: " << endl;
    result->PrintStatus();

    VS1->clear();
    /* VS1 insert 3, 4 to L */
    VS1->insert_L(3);
    VS1->insert_L(4);

    result = BPG->GetNeighborSet(VS1, true);
    cout << "Find {3,4}'s common neighbor: " << endl;
    result->PrintStatus();

    /* Insert R set and test GetNeighborSet() function */
    VS1->clear();
    VS1->insert_R(5);
    VS1->insert_R(6);

    result = BPG->GetNeighborSet(VS1, false);
    cout << "Find {5,6}'s common neighbor: " << endl;
    result->PrintStatus();

    VS1->clear();
    VS1->insert_R(6);
    VS1->insert_R(7);

    result = BPG->GetNeighborSet(VS1, false);
    cout << "Find {6,7}'s common neighbor: " << endl;
    result->PrintStatus();

    VS1->clear();
    VS1->insert_R(7);
    VS1->insert_R(8);

    result = BPG->GetNeighborSet(VS1, false);
    cout << "Find {7,8}'s common neighbor: " << endl;
    result->PrintStatus();

    VS1->clear();
    VS1->insert_R(8);
    VS1->insert_R(9);

    result = BPG->GetNeighborSet(VS1, false);
    cout << "Find {8,9}'s common neighbor: " << endl;
    result->PrintStatus();


    

    return 0;
}
