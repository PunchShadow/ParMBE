#include <iostream>
#include <string>
#include "fmbe.hpp"

using namespace std;


int main(int argc, char *argv[])
{
    if (argc < 2) {
        std::cout << "Too few parameters!!!!" << std::endl;
        return 0;
    } 

    const std::string file_path = argv[1];

    fmbe_main(file_path);

    return 0;

}