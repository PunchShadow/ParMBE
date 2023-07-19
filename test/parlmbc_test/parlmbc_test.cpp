#include "parlmbc.hpp"
#include <iostream>
#include <string>


using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "Too few parameters!!!!" << endl;
        return 0;
    } 

    if (argc == 2) {
        cout << "Use default thread number: " << num_thread << endl;
    } else if (argc == 3) {
        cout << "Use thread number: " << argv[2] << endl;
        num_thread = atoi(argv[2]);
    }

    const string file_path = argv[1];


    parlmbc_main(file_path);

    return 0;

}