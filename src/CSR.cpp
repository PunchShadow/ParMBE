#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include "CSR.hpp"


CSR_t graphToCSR(const vector<pair<int, int>>& graph) {
    CSR_t csr;
    int curr_row = -1;

    for (const auto& edge : graph) {
        while (curr_row < edge.first) {
            csr.row_ptr.push_back(csr.col_idx.size());
            curr_row++;
        }

        csr.col_idx.push_back(edge.second);
    }

    // Don't forget the last row_ptr for the end of the matrix
    csr.row_ptr.push_back(csr.col_idx.size());

    return csr;
}

void
searchNeighbor(const CSR_t& csr, int node, int& start, int& end) {
    start = csr.row_ptr[node];
    end = csr.row_ptr[node + 1];
}

int
getKthNeighbor(const CSR_t& csr, int node, int k) {
    int start, end;
    searchNeighbor(csr, node, start, end);

    if (k < 0 || k >= end - start) {
        return -1;
    }

    return csr.col_idx[start + k];
}

int
getKthNeighbor(const CSR_t& csr, int node, int start, int end, int k) {
    if (k < 0 || k >= end - start) {
        return -1;
    }

    return csr.col_idx[start + k];
}

X_set
findCommonNeighbor(const CSR_t& csr, const X_set X)
{
    X_set temp_x, temp_x2, result;
    bool first_ele = true;
    for (int ele : X) {
        int start, end;
        searchNeighbor(csr, ele, start, end);
        
        if (first_ele) {
            for (int i = start; i < end; i++) {
                int neighbor = csr.col_idx[i];
                result.insert(neighbor);
            }
        } else {
            for (int i = start; i < end; i++) {
                temp_x.insert(csr.col_idx[i]);
            }
            result = set_intersection(result, temp_x);
        }

    }
    return result;
}



void edgeListToCSR(const std::string &filename, CSR_t &csr, CSR_t &csr_reverse) {
    std::ifstream file(filename);

    // Mapping each node to its neighbours
    std::map<int, std::vector<int>> adjacencyList;
    // Reverse map
    std::map<int, std::vector<int>> reverseAdjacencyList;
    int maxNodeidxex = 0;
    int maxSource = 0;
    int maxDes = 0;

    std::string line;
    while (std::getline(file, line)) {
        // Ignore lines that start with '%'
        if (line[0] == '%') continue;

        std::istringstream iss(line);
        int source, target;
        iss >> source >> target;
        
        /* Avoid repeated edges */
        /* First find if source is in map */
        if (adjacencyList.find(source) != adjacencyList.end()) {
            /* Find if target is in the vector */
            if (std::find(adjacencyList[source].begin(), adjacencyList[source].end(), target) != adjacencyList[source].end()) {
                continue;
            }
        }

        if (reverseAdjacencyList.find(target) != reverseAdjacencyList.end()) {
            if (std::find(reverseAdjacencyList[target].begin(), reverseAdjacencyList[target].end(), source) != reverseAdjacencyList[target].end()) {
                continue;
            }
        }
        
        
        adjacencyList[source].push_back(target);
        reverseAdjacencyList[target].push_back(source);
        
        maxSource = std::max(maxSource, source);
        maxDes = std::max(maxDes, target);
        // Fidxing maximum node idxex for sizing our CSR
        maxNodeidxex = std::max({maxNodeidxex, source, target});
    }

    csr.row_ptr.push_back(0);  // First element of row_ptr is always 0
    csr.row_ptr.push_back(0);
    csr_reverse.row_ptr.push_back(0);
    csr_reverse.row_ptr.push_back(0);


    // We assume that nodes are 1-idxexed and go up to maxNodeidxex inclusively
    for (int node = 1; node <= maxSource; ++node) {
        csr.col_idx.insert(csr.col_idx.end(), adjacencyList[node].begin(), adjacencyList[node].end());
        csr.row_ptr.push_back(csr.col_idx.size());

    }

    for (int node = 1; node <= maxDes; node++) {
        csr_reverse.col_idx.insert(csr_reverse.col_idx.end(), reverseAdjacencyList[node].begin(), reverseAdjacencyList[node].end());
        csr_reverse.row_ptr.push_back(csr_reverse.col_idx.size());
    }

    /* L set is always the larger one */
    if (maxSource < maxDes) {
        swapCSR(csr, csr_reverse);
    }

}

void swapCSR(CSR_t& csr1, CSR_t& csr2) {
    csr1.row_ptr.swap(csr2.row_ptr);
    csr1.col_idx.swap(csr2.col_idx);
}


void printCSR(const CSR_t& csr) {
    cout << "CSR: " << endl;
    cout << "row_ptr: ";
    for (auto ele : csr.row_ptr) {
        cout << ele << " ";
    }
    cout << endl;
    cout << "col_idx: ";
    for (auto ele : csr.col_idx) {
        cout << ele << " ";
    }
    cout << endl;
}