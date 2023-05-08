#include <stdio.h>
#include <iostream>


#include "BiGraph.hpp"

using namespace std;



int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Too few parameters!!!!" << endl;
        return 0;
    } 

    string file_path = argv[1];
    
    cout << "file path is: " << file_path << endl;

    BiGraph BPG;
    
    BPG.read(file_path, 2, 0);

    BPG.PrintSetSize();

    return 0;
}