#include <iostream>
#include <vector>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>


typedef tbb::concurrent_unordered_set<int> X_set;

void
printSet(tbb::concurrent_unordered_set<int> set)
{
    for (const auto ele : set) {
        std::cout << ele << " ";
    }
    std::cout << std::endl;
    set.insert(100);
}

X_set
returnSet()
{
    X_set set1;
    set1.insert(5);
    set1.insert(6);
    set1.insert(7);

    return set1;
}



int main()
{
    /* Create a set */
    tbb::concurrent_unordered_set<int> set1;
    set1.insert(1);
    set1.insert(2);
    set1.insert(3);

    printSet(set1);
    printSet(set1);

    X_set res = returnSet();

    printSet(res);

    return 0;

}