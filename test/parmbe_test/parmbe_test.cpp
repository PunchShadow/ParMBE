#include "parmbe.hpp"
#include <iostream>
#include <string>


using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Too few parameters!!!!" << endl;
        return 0;
    } 

    const string file_path = argv[1];

    parmbe_main(file_path);

    return 0;

}