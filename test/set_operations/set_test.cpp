#include <iostream>
#include "shared_var.hpp"
#include "utility.hpp"

using namespace std;


int main()
{
    /* Test substraction */
    X_set X;
    X.insert(1);
    X.insert(2);
    X.insert(3);
    X.insert(4);
    X.insert(5);

    L_set Gamma;
    Gamma.insert(1);
    Gamma.insert(2);
    Gamma.insert(3);

    R_set tail;
    tail.insert(4);
    tail.insert(5);

    X_set result = set_subtract(X, Gamma);

    cout << "result: ";
    print_set(result);
    cout << "X: ";
    print_set(X);
    cout << "Gamma: ";
    print_set(Gamma);

    X_set empty_set;

    /* Test if the empty set is the subset of X */
    if (set_subset(empty_set, X)) {
        cout << "empty_set is the subset of X" << endl;
    } else {
        cout << "empty_set is not the subset of X" << endl;
    }

    if (set_subset(tail, Gamma)) {
        cout << "tail is the subset of Gamma" << endl;
    } else {
        cout << "tail is not the subset of Gamma" << endl;
    }

    if (set_subset(X, empty_set)) {
        cout << "X is the subset of empty_set" << endl;
    } else {
        cout << "X is not the subset of empty_set" << endl;
    }

    
    
    return 0;

}