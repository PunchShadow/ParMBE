#include "BiGraph.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include <tbb/tbb.h>
#include <tbb/concurrent_hash_map.h>
#include "utility.hpp"




void BiGraph::PrintSetSize()
{
    cout << "R set size: " << R_set.size() << endl;
    cout << "L set size: " << L_set.size() << endl;
    cout << "Edge set size: " << E_set.size() << endl;
    cout << "L_neighbor_list size: " << L_neighbor_list.size() << endl;
    cout << "R_neighbor_list size: " << R_neighbor_list.size() << endl;
}


/* Read from edge pair file and transform into BiGraph structure */
/* FIXME: Consider the repeated edge, namely two vertices have multiple edges. */
void BiGraph::read(string file_path, int num, int tail)
{
    ifstream edgefile;
    edgefile.open(file_path);

    string line;
    const string delimiter = " "; // The char to split the line

    string token;
    size_t pos = 0;
    int c_num = 0;
    int src, des = -1;
    int count = 10;

    /* Read file until the end */
    while (true) {
        if (edgefile.is_open()) {
            getline(edgefile, line);
        }
        if (edgefile.eof()) break;
        if (line.empty()) continue;

        pos = 0;
        c_num = 0;
        src = -1;
        des = -1;


        /* Split the line by delimiter, token is the left side substring. 
           The line after the while loop is the remaining substring. 
           format: src des */
        while ((pos = line.find(delimiter)) != std::string::npos) {
            if (c_num >= num) break; // Control the number of words in a line read.
            
            token = line.substr(0, pos);
            if (token.compare("%") == 0) break; // Skip the line with "%"
            
            if (c_num == 0) {
                src = stoi(token);
                L_set.insert(src);
                
            } else if (c_num == 1) {
                des = stoi(token);
                R_set.insert(des);
                if (src == -1) {
                    cout << "Error in generating bigraph!!!!" << endl;
                    break;
                }
                pair<int, int> new_pair = make_pair(src, des);



                E_set.insert(new_pair);

                /* Update the neighbor list */
                //std::map<int, tbb::concurrent_unordered_set<int>>::iterator it;
                auto it = this->L_neighbor_list.find(src);
                if (it != this->L_neighbor_list.end()) {
                    /* (last, {...}) exists, update the set of the neighbor */
                    tbb::concurrent_unordered_set<int> *prev_pair = it->second;
                    prev_pair->insert(des);
                } else {
                    /* There is no (Last, ...), insert a new entry */
                    tbb::concurrent_unordered_set<int> *new_set = new tbb::concurrent_unordered_set<int>();
                    new_set->insert(des);
                    this->L_neighbor_list.insert(std::pair<int, tbb::concurrent_unordered_set<int>*>(src, new_set));
                }
                it = this->R_neighbor_list.find(des);
                if (it != this->R_neighbor_list.end()) {
                    tbb::concurrent_unordered_set<int> *prev_pair = it->second;
                    prev_pair->insert(src);
                } else {
                    tbb::concurrent_unordered_set<int> *new_set = new tbb::concurrent_unordered_set<int>();
                    new_set->insert(src);
                    this->R_neighbor_list.insert(std::pair<int, tbb::concurrent_unordered_set<int>*>(des, new_set));
                }
            } else {
                continue;
            }
            c_num++;
            // cout << token << " ";
            line.erase(0, pos + delimiter.length());
            
        }
        /* The line has only scan L set */
        if (c_num == 1) {
            token = line.substr(pos+1, line.length());
            des = stoi(token);
            R_set.insert(des);

            pair<int, int> new_pair = make_pair(src, des);
            /* TODO: Avoid repeated edges insertion */
            E_set.insert(new_pair);

            /* Update the neighbor list */
            //std::map<int, tbb::concurrent_unordered_set<int>>::iterator it;
            auto it = this->L_neighbor_list.find(src);
            if (it != this->L_neighbor_list.end()) {
                /* (last, {...}) exists, update the set of the neighbor */
                tbb::concurrent_unordered_set<int> *prev_pair = it->second;
                prev_pair->insert(des);
            } else {
                /* There is no (Last, ...), insert a new entry */
                tbb::concurrent_unordered_set<int> *new_set = new tbb::concurrent_unordered_set<int>();
                new_set->insert(des);
                this->L_neighbor_list.insert(std::pair<int, tbb::concurrent_unordered_set<int>*>(src, new_set));
            }
            it = this->R_neighbor_list.find(des);
            if (it != this->R_neighbor_list.end()) {
                tbb::concurrent_unordered_set<int> *prev_pair = it->second;
                prev_pair->insert(src);
            } else {
                tbb::concurrent_unordered_set<int> *new_set = new tbb::concurrent_unordered_set<int>();
                new_set->insert(src);
                this->R_neighbor_list.insert(std::pair<int, tbb::concurrent_unordered_set<int>*>(des, new_set));
            }

        }
        count--;
    }
    
}

/* Add vertex functions */

void BiGraph::AddVertexL(int node)
{
    this->L_set.insert(node);
}


void BiGraph::AddVertexR(int node)
{
    this->R_set.insert(node);
}

/* Add edge functions */

void BiGraph::AddEdge(int src, int des)
{
    /* First check if src and des are in L_set and R_set */
    if (this->L_set.find(src) == this->L_set.end()) {
        cout << "Error: src is not in L_set" << endl;
        return;
    }
    if (this->R_set.find(des) == this->R_set.end()) {
        cout << "Error: des is not in R_set" << endl;
        return;
    }

    /* Add the edge to the edge set */
    pair<int, int> new_pair = make_pair(src, des);
    E_set.insert(new_pair);

    /* Update the neighbor list */
    //std::map<int, tbb::concurrent_unordered_set<int>>::iterator it;
    auto it = this->L_neighbor_list.find(src);
    if (it != this->L_neighbor_list.end()) {
        /* (last, {...}) exists, update the set of the neighbor */
        tbb::concurrent_unordered_set<int> *prev_pair = it->second;
        prev_pair->insert(des);
    } else {
        /* There is no (Last, ...), insert a new entry */
        tbb::concurrent_unordered_set<int> *new_set = new tbb::concurrent_unordered_set<int>();
        new_set->insert(des);
        this->L_neighbor_list.insert(std::pair<int, tbb::concurrent_unordered_set<int>*>(src, new_set));
    }
    it = this->R_neighbor_list.find(des);
    if (it != this->R_neighbor_list.end()) {
        tbb::concurrent_unordered_set<int> *prev_pair = it->second;
        prev_pair->insert(src);
    } else {
        tbb::concurrent_unordered_set<int> *new_set = new tbb::concurrent_unordered_set<int>();
        new_set->insert(src);
        this->R_neighbor_list.insert(std::pair<int, tbb::concurrent_unordered_set<int>*>(des, new_set));
    }
}



int BiGraph::VertexIsLeft(int node) 
{
    auto it = this->L_set.find(node);
    if (it != this->L_set.end()) {
        return 1;
    } else if (this->R_set.find(node) != this->R_set.end()) {
        return 0;
    } else {
        return -1;
    }
}

int BiGraph::SetIsLeft(VertexSet *set)
{
    if (set->GetLsetSize() > 0 && set->IsRsetEmpty()) {
        return 1;
    } else if (set->GetRsetSize() > 0 && set->IsLsetEmpty()) {
        return 0;
    } else {
        return -1;
    }
}

/* FIXME: Reduce the set copy on SetRset */
/* Use link the read-only data to L_set and R_set of neighbor_set */
/* If other functions want to use the return pointer, please use deep_copy() of VertexSet */
VertexSet *BiGraph::GetNeighbor(int node, bool nodeIsLeft)
{
    VertexSet *neighbor_set = new VertexSet();

    if (nodeIsLeft) {
        neighbor_set->SetRset(this->FindLNeighbor(node));
    } else {
        neighbor_set->SetLset(this->FindRNeighbor(node));
    }
    return neighbor_set;
}

/* FIXME: Unable to find the correct set and return */
/* Find the "common neighbor" of the set 
   Return: the pointer of allocated Vertextset if there is common neighbor
           NULL if there is no common neighbor
 */
VertexSet *BiGraph::GetNeighborSet(VertexSet *set, bool setIsLeft)
{
    if (set == NULL) {
        cout << "Error: GetNeighborSet() set is NULL" << endl;
        exit(1);
    }
    if (set->IsEmpty()) {
        cout << "Error: GetNeighborSet() set is empty" << endl;
        exit(1);
    }

#ifdef DEBUG
    CHECK_VERTEXSET_IS_ONE_SIDE(set);
#endif /* DEBUG */

    VertexSet *neighbor_set = new VertexSet();
    VertexSet *temp_set;
    int first_time = 1;

    /* For each vertex in the set */
    if (setIsLeft) {
        for (auto it = set->GetLset()->begin(); it != set->GetLset()->end(); it++) {
            temp_set = this->GetNeighbor(*it, true);
            if (temp_set->GetSize() == 0) {
                continue;
            }
            if (first_time) {
                /* Deep copy all the temp_set to neighbor set */
                neighbor_set->deep_copy(temp_set);
                first_time = 0;
                continue;
            }

            /* Intersection the neighbor */
            *neighbor_set *= *temp_set;

        }
    } else {

        for (auto it = set->GetRset()->begin(); it != set->GetRset()->end(); it++) {
            temp_set = this->GetNeighbor(*it, false);
            if (temp_set->GetSize() == 0) {
                continue;
            }
            if (first_time) {
                neighbor_set->deep_copy(temp_set);
                first_time = 0;
                continue;
            }

            *neighbor_set *= *temp_set;

        }
    }
    return neighbor_set;
}

    


tbb::concurrent_unordered_set<int> *BiGraph::FindLNeighbor(int node)
{
    auto it = this->L_neighbor_list.find(node);
    if (it != this->L_neighbor_list.end()) {
        return it->second;
    } else {
        return NULL;
    }
}


tbb::concurrent_unordered_set<int> *BiGraph::FindRNeighbor(int node)
{
    auto it = this->R_neighbor_list.find(node);
    if (it != this->R_neighbor_list.end()) {
        return it->second;
    } else {
        return NULL;
    }
}

/* Find the degree of given node and the position of this node (L_set or R_set). */
int BiGraph::getDegree(int node, bool nodeIsLeft)
{
    int degree;
    tbb::concurrent_unordered_set<int> *n_list;
    if (nodeIsLeft) {
        /* Find the node in L_neighbor_list */
        n_list = this->L_neighbor_list.find(node)->second;
        degree = n_list->size();
    } else {
        n_list = this->R_neighbor_list.find(node)->second;
        degree = n_list->size();
    }
    return degree;
}




void BiGraph::InsertToFinal(VertexSet *B)
{
    this->B_final.insert(B);
}

/* Output results */
void BiGraph::PrintFinalB()
{
    cout << "Number of Maximal Biclique: " << this->B_final.size() << endl;
    // for (auto it = this->B_final.begin(); it != this->B_final.end(); it++) {
    //     tbb::concurrent_unordered_set<int> *B = it;
    //     for (auto it2 = B->begin(); it2 != B->end(); it2++) {
    //         cout << *it2 << " ";
    //     }
    //     cout << endl;
    // }
}


void
BiGraph::PrintLNeighborList()
{
    cout << "L_neighbor_list: " << endl;
    for (auto it = this->L_neighbor_list.begin(); it != this->L_neighbor_list.end(); it++) {
        cout << it->first << ": ";
        tbb::concurrent_unordered_set<int> *temp = it->second;
        for (auto it2 = temp->begin(); it2 != temp->end(); it2++) {
            cout << *it2 << " ";
        }
        cout << endl;
    }
}

void
BiGraph::PrintRNeighborList()
{
    cout << "R_neighbor_list: " << endl;
    for (auto it = this->R_neighbor_list.begin(); it != this->R_neighbor_list.end(); it++) {
        cout << it->first << ": ";
        tbb::concurrent_unordered_set<int> *temp = it->second;
        for (auto it2 = temp->begin(); it2 != temp->end(); it2++) {
            cout << *it2 << " ";
        }
        cout << endl;
    }
}