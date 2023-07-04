#include <iostream>
#include <string>
#include "CSR.hpp"
#include "shared_var.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Too few parameters!!!!" << endl;
        return 0;
    } 

    const string file_path = argv[1];

    /* CSR */
    edgeListToCSR(file_path, csr, csr_reverse);


    /* Test searchNeighbr function */
    int start, end;
    searchNeighbor(csr, 2, start, end);

    int start_reverse, end_reverse;
    searchNeighbor(csr_reverse, 6, start_reverse, end_reverse);
    /* Print the neighbor */
    cout << "Neighbor of 2: ";
    for (int i = start; i < end; i++) {
        cout << csr.col_idx[i] << " ";
    }
    cout << endl;

    cout << "Neighbor of 6: ";
    for (int i = start_reverse; i < end_reverse; i++) {
        cout << csr_reverse.col_idx[i] << " ";
    }
    cout << endl;

    X_set Gamma, tail;
    
    for (int i=1; i < (int)csr.row_ptr.size() - 1 ; i++) {
        tail.insert(i);
    }
    for (int j=csr.row_ptr.size()-1; j < (int)csr_reverse.row_ptr.size() - 1; j++) {
        Gamma.insert(j);
    }

    /* Print tail, Gamma */
    cout << "tail: ";
    print_set(tail);
    cout << "Gamma: ";
    print_set(Gamma);


    /* Test swapCSR */
    /* Print original CSRs */
    cout << "Before swapCSR" << endl;
    cout << "csr: " << endl;
    printCSR(csr);
    cout << "csr_reverse: " << endl;
    printCSR(csr_reverse);
    int csr_size = csr.row_ptr.size();

    swapCSR(csr, csr_reverse);


    int zero_num = csr_size;
    csr.row_ptr.erase(csr.row_ptr.begin(), csr.row_ptr.begin() + zero_num - 2);
    
    for (int i = 0; i < zero_num - 1; i++) {
        csr_reverse.row_ptr.insert(csr_reverse.row_ptr.begin(), 0);
    }



    cout << "After swapCSR" << endl;
    cout << "csr: " << endl;
    printCSR(csr);
    cout << "csr_reverse: " << endl;
    printCSR(csr_reverse);    




    return 0;

}

