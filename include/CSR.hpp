#ifndef _CSR_H_
#define _CSR_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;


/* CSR format */
struct CSR
{
    vector<int> row_ptr;
    vector<int> col_ind;
};

CSR graphToCSR(const vector<pair<int, int>>& graph);

vector<pair<int, int>> readGraphFromFile(const string& filename);





#endif /* _CSR_H_ */