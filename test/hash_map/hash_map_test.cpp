#include <iostream>
#include <atomic>
#include <thread>
#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>
#include <tbb/concurrent_hash_map.h>


using namespace std;

typedef tbb::concurrent_hash_map<int, std::atomic<int>> hash_map;
hash_map freqHash;




int main()
{
    /* Generate a vector from range 0 ~ 100 */
    std::vector<int> vec;
    for (int i = 0; i < 100; i++) {
        vec.push_back(i);
    }

    hash_map::accessor a;
    freqHash.insert(a, 0);
    a->second = 0;
    freqHash.insert(a, 1);
    a->second = 0;
    
    tbb::parallel_for_each(vec.begin(), vec.end(), 
        [&](int node) {
            cout << "node: " << node << endl;
            hash_map::accessor a;
            if (node % 2 == 0) {
                freqHash.find(a, 0);
                a->second++;
            } else {
                freqHash.find(a, 1);
                a->second++;
            }
        }
    );

    cout << "Finished inserting" << endl;    

    /* Print the hash_table */
    hash_map::iterator it;
    for (it = freqHash.begin(); it != freqHash.end(); it++) {
        cout << it->first << " " << it->second << endl;
    }

    return 0;


}