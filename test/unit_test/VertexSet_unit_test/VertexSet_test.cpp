#include "VertexSet.hpp"
#include "BiGraph.hpp"
#include "tbb/tbb.h"
#include "tbb/concurrent_unordered_set.h"
#include <iostream>


using namespace std;


int main(int argc, char *argv[])
{
        /* Initialize BPG */
    static BiGraph *BPG = new BiGraph();
    const string file_path = argv[1];
    
    /* Use read() function with argv */
    BPG->read(file_path, 2, 0);
    
    /* Test operater overloading function */
    VertexSet *VS1 = new VertexSet();
    VertexSet *VS2 = new VertexSet();
    VertexSet *VS3 = new VertexSet();

    VS1->insert_L(1);
    VS1->insert_L(2);
    VS1->insert_L(3);
    VS1->insert_L(4);
    VS1->insert_L(5);

    VS2->insert_L(1);
    VS2->insert_L(2);
    VS2->insert_L(3);

    /* Test operator = */
    cout << "=========<< Test operator = >>==========" << endl;
    cout << "VS1: " << endl;
    VS1->PrintStatus();
    cout << "VS2: " << endl;
    VS2->PrintStatus();
    cout << "VS3: " << endl;
    VS3->PrintStatus();
    cout << "After VS3 = VS1: " << endl;
    *VS3 = *VS1;
    cout << "VS3: " << VS3 <<  endl;
    VS3->PrintStatus();


    /* Test operator * */
    cout << "=========<< Test operator * >>==========" << endl;
    cout << "VS1: " << endl;
    VS1->PrintStatus();
    cout << "VS2: " << endl;
    VS2->PrintStatus();
    cout << "VS3: " << endl;
    VS3->PrintStatus();
    cout << "After VS3 = VS1 * VS2: " << endl;
    *VS3 = *VS1 * *VS2;
    cout << "VS3: " << VS3 << endl;
    VS3->PrintStatus();

    /* Test operator + */
    cout << "=========<< Test operator + >>==========" << endl;
    cout << "VS1: " << endl;
    VS1->PrintStatus();
    cout << "VS2: " << endl;
    VS2->PrintStatus();
    cout << "VS3: " << endl;
    VS3->PrintStatus();
    cout << "After VS3 = VS1 + VS2: " << endl;
    *VS3 = *VS1 + *VS2;
    cout << "VS3: " << VS3 << endl;
    VS3->PrintStatus();

    /* Test operator - */
    cout << "=========<< Test operator - >>==========" << endl;
    cout << "VS1: " << endl;
    VS1->PrintStatus();
    cout << "VS2: " << endl;
    VS2->PrintStatus();
    cout << "VS3: " << endl;
    VS3->PrintStatus();
    cout << "After VS3 = VS1 - VS2: " << endl;
    *VS3 = *VS1 - *VS2;
    cout << "VS3: " << VS3 << endl;
    VS3->PrintStatus();

    /* Test operator += */
    cout << "=========<< Test operator += >>==========" << endl;
    cout << "VS1: " << endl;
    VS1->PrintStatus();
    cout << "VS2: " << endl;
    VS2->PrintStatus();
    cout << "VS3: " << endl;
    VS3->PrintStatus();
    cout << "After VS3 += VS1: " << endl;
    *VS3 += *VS1;
    cout << "VS3: " << VS3 << endl;
    VS3->PrintStatus();

    /* Test operator *= */
    cout << "=========<< Test operator *= >>==========" << endl;
    cout << "VS1: " << endl;
    VS1->PrintStatus();
    cout << "VS2: " << endl;
    VS2->PrintStatus();
    cout << "VS3: " << endl;
    VS3->PrintStatus();
    cout << "After VS3 *= VS2: " << endl;
    *VS3 *= *VS2;
    cout << "VS3: " << VS3 << endl;
    VS3->PrintStatus();




    return 0;


}