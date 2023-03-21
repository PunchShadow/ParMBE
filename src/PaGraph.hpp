#include <tbb/tbb.h>
#include <tbb/concurrent_unordered_set.h>

using namespace tbb;


class PaGraph 
{
private:
    concurrent_unordered_set<int> X; // vertex set
    concurrent_unordered_set<int> t_X; // adjacency list of X - one-hop neighbor
    concurrent_unordered_set<int> tail; // tail vertices of X
    int ms; // minimum size of threshold

public:
    
    


};