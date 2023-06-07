#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include "CSR.hpp"

using namespace std;


int main(int argc, char *argv[]) {

    const string filename = argv[1];
    vector<pair<int, int>> graph = readGraphFromFile(filename);
    CSR csr = graphToCSR(graph);

    // Print CSR format
    cout << "row_ptr: ";
    for (int val : csr.row_ptr) {
        cout << val << " ";
    }
    cout << "\n";

    cout << "col_ind: ";
    for (int val : csr.col_ind) {
        cout << val << " ";
    }
    cout << "\n";

    return 0;
}