#include "utility.hpp"
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include <iostream>

using namespace std;



int main()
{
    /* Unit test of all functions in utility.hpp */
    cout << "Unit test of all functions in utility.hpp" << endl;

    /* Test TBB_CUS_INTERSECTION() */
    cout << "Test TBB_CUS_INTERSECT()" << endl;
    tbb::concurrent_unordered_set<int> *src1 = new tbb::concurrent_unordered_set<int>();
    tbb::concurrent_unordered_set<int> *src2 = new tbb::concurrent_unordered_set<int>();
    tbb::concurrent_unordered_set<int> *dst = new tbb::concurrent_unordered_set<int>();

    /* Insert {1,2,3} to src1 and {1,3,5} to src2 */
    src1->insert(1);
    src1->insert(2);
    src1->insert(3);

    src2->insert(1);
    src2->insert(3);
    src2->insert(5);

    TBB_CUS_INTERSECT(dst, src1, src2);

    /* Print src1 */
    cout << "Expect: dst = src * src2 => {1,2,3} * {1,3,5} = {1,3}" << endl;
    cout << "Real: dst = {";
    for (auto it = dst->begin(); it != dst->end(); it++) {
        cout << *it << ",";
    }
    cout << "}" << endl;

    /* Test TBB_CUS_UNION() */
    cout << "Test TBB_CUS_UNION()" << endl;
    tbb::concurrent_unordered_set<int> *src3 = new tbb::concurrent_unordered_set<int>();
    tbb::concurrent_unordered_set<int> *src4 = new tbb::concurrent_unordered_set<int>();
    //tbb::concurrent_unordered_set<int> *dst2 = new tbb::concurrent_unordered_set<int>();

    /* Insert {1,2,3} to src3 and {1,3,5} to src4 */
    src3->insert(1);
    src3->insert(2);
    src3->insert(3);

    src4->insert(1);
    src4->insert(3);
    src4->insert(5);

    TBB_CUS_UNION(src3, src4);

    /* Print src3 */
    cout << "Expect: src3 = src3 + src4 => {1,2,3} + {1,3,5} = {1,2,3,5}" << endl;
    cout << "Real: src3 = {";
    for (auto it = src3->begin(); it != src3->end(); it++) {
        cout << *it << ",";
    }
    cout << "}" << endl;


    /* Test TBB_CUS_SUBSET */
    cout << "Test TBB_CUS_SUBSET()" << endl;

    if (TBB_CUS_SUBSET(src3, src4)) {
        cout << "src3 is a subset of src4" << endl;
    } else {
        cout << "src3 is not a subset of src4" << endl;
    }

    /* Create a unit test that sr4 is subset of sr5 */
    tbb::concurrent_unordered_set<int> *src5 = new tbb::concurrent_unordered_set<int>();
    src5->insert(1);
    src5->insert(3);
    src5->insert(5);

    if (TBB_CUS_SUBSET(src4, src5)) {
        cout << "src4 is a subset of src5" << endl;
    } else {
        cout << "src4 is not a subset of src5" << endl;
    }

    /* Create a empty set */
    tbb::concurrent_unordered_set<int> *src6 = new tbb::concurrent_unordered_set<int>();
    if (TBB_CUS_SUBSET(src6, src5)) {
        cout << "src6 is a subset of src5" << endl;
    } else {
        cout << "src6 is not a subset of src5" << endl;
    }

    /* Test TBB_CUS_SUBTRACT() */
    cout << "------------ << Test TBB_CUS_SUBTRACT() >> --------------" << endl;
    /* Reuse src1 and src2 */
    src1->clear();
    src2->clear();

    /* Insert {1,2,3} to src1 and {1,3,5} to src2 */
    src1->insert(1);
    src1->insert(2);
    src1->insert(3);

    src2->insert(1);
    src2->insert(3);
    src2->insert(5);

    TBB_CUS_SUBTRACT(src1, src2);

    /* Print src1 */
    cout << "Expect: src1 = src1 - src2 => {1,2,3} - {1,3,5} = {2}" << endl;
    cout << "Real: src1 = {";
    for (auto it = src1->begin(); it != src1->end(); it++) {
        cout << *it << ",";
    }
    cout << "}" << endl;

    /* Test substract with empty set */
    cout << "------------ << Test TBB_CUS_SUBTRACT() with empty set >> --------------" << endl;
    /* Reuse src1 and src2 */
    src1->clear();
    src2->clear();

    /* Insert {1,2,3} to src1 */
    src1->insert(1);
    src1->insert(2);
    src1->insert(3);

    TBB_CUS_SUBTRACT(src1, src2);

    /* Print src1 */
    cout << "Expect: src1 = src1 - src2 => {1,2,3} - {} = {1,2,3}" << endl;
    cout << "Real: src1 = {";
    for (auto it = src1->begin(); it != src1->end(); it++) {
        cout << *it << ",";
    }
    cout << "}" << endl;

    
    /* Test TBB_CUS_COPY() */
    cout << "------------ << Test TBB_CUS_COPY() >> --------------" << endl;
    /* Reuse src1 and src2 */
    src1->clear();
    src2->clear();

    /* Insert {1,2,3} to src1 */
    src1->insert(1);
    src1->insert(2);
    src1->insert(3);

    TBB_CUS_COPY(src2, src1);

    /* Print src1 */
    cout << "Expect: src2 = src1 => {1,2,3} = {1,2,3}" << endl;
    cout << "Real: src2 = {";
    for (auto it = src2->begin(); it != src2->end(); it++) {
        cout << *it << ",";
    }
    cout << "}" << endl;

    /* Test if src2 is NULL */
    cout << "------------ << Test TBB_CUS_COPY() with NULL >> --------------" << endl;

    tbb::concurrent_unordered_set<int> *src7 = new tbb::concurrent_unordered_set<int>();
    TBB_CUS_COPY(src7, src1);
    cout << "sr7: " << src7 << endl;
    /* Print src1 */
    cout << "Expect: src7 = src1 => {1,2,3} = {1,2,3}" << endl;
    cout << "Real: src7 = {";
    for (auto it = src7->begin(); it != src7->end(); it++) {
        cout << *it << ",";
    }
    cout << "}" << endl;







    return 0;

}