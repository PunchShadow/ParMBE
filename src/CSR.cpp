#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include "CSR.hpp"


CSR graphToCSR(const vector<pair<int, int>>& graph) {
    CSR csr;
    int curr_row = -1;

    for (const auto& edge : graph) {
        while (curr_row < edge.first) {
            csr.row_ptr.push_back(csr.col_ind.size());
            curr_row++;
        }

        csr.col_ind.push_back(edge.second);
    }

    // Don't forget the last row_ptr for the end of the matrix
    csr.row_ptr.push_back(csr.col_ind.size());

    return csr;
}


vector<pair<int, int>> readGraphFromFile(const string& filename) {
    ifstream file(filename);
    string line;
    vector<pair<int, int>> graph;

    while (getline(file, line)) {
        if (line[0] == '%') continue;  // skip comment lines

        stringstream ss(line);
        int source, destination;
        ss >> source >> destination;
        graph.push_back({source, destination});
    }

    return graph;
}