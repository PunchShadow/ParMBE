#ifndef _CSR_H_
#define _CSR_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <numeric>
#include "shared_var.hpp"
#include "utility.hpp"


using namespace std;

/* Forward declaration */
typedef struct CSR CSR_t;


CSR_t graphToCSR(const vector<pair<int, int>>& graph);

void searchNeighbor(const CSR_t& csr, int node, int& start, int& end);

int getKthNeighbor(const CSR_t& csr, int node, int k);

int getKthNeighbor(const CSR_t& csr, int node, int start, int end, int k);

void edgeListToCSR(const std::string &filename, CSR_t &csr, CSR_t &csr_reverse);

X_set findCommonNeighbor(const CSR_t& csr, const X_set& X);

void swapCSR(CSR_t& csr1, CSR_t& csr2);

void printCSR(const CSR_t& csr);



#endif /* _CSR_H_ */