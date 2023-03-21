#include <string>
#include <iostream>
#include <fstream>
#include <set>
#include <tbb/tbb.h>

using namespace std;


template <typename L_type, typename R_type, typename E_type>
class BiGraph
{
private:
    tbb::concurrent_unordered_set<L_type> L_set; // L vertex set
    tbb::concurrent_unordered_set<R_type> R_set; // R vertex set
    tbb::concurrent_unordered_set<E_type> E_set; // Edge set
    // set<L_type> L_set;
    // set<R_type> R_set;
    // set<E_type> E_set;

public:
    /* @param:
       - file_path: the path to the file 
       - num: how many number of substring do we want to read 
       - tail: need to remain the tail of the line?
    */
    void read(string file_path, int num, int tail); // Read from edge pair file
    void PrintSetSize();
    tbb::concurrent_unordered_set<L_type> getRSet() {return R_set;};
    tbb::concurrent_unordered_set<L_type> getLSet() {return L_set;};


};

template<typename L_type, typename R_type, typename E_type>
void BiGraph<L_type, R_type, E_type>::PrintSetSize()
{
    cout << "R set size: " << R_set.size() << endl;
    cout << "L set size: " << L_set.size() << endl;
}


/* Read from edge pair file and transform into BiGraph structure */
template<typename L_type, typename R_type, typename E_type>
void BiGraph<L_type, R_type, E_type>::read(string file_path, int num, int tail)
{
    ifstream edgefile;
    edgefile.open(file_path);

    string line;
    const string delimiter = " "; // The char to split the line

    int count = 10;
    /* Read file until the end */
    while (true) {
        if (edgefile.is_open()) {
            getline(edgefile, line);
        }
        if (edgefile.eof()) break;
        
        string token;
        size_t pos = 0;
        int c_num = 0;
    

        /* Split the line by delimiter, token is the left side substring. 
           The line after the while loop is the remaining substring. */
        while ((pos = line.find(delimiter)) != std::string::npos) {
            if (c_num >= num) break; // Control the number of words in a line read.
            
            token = line.substr(0, pos);
            if (token.compare("%") == 0) break; // Skip the line with "%"
            
            if (c_num == 0) {
                L_set.insert(stoi(token));
            } else if (c_num == 1) {
                R_set.insert(stoi(token));
            } else {
                continue;
            }
            c_num++;
            // cout << token << " ";
            line.erase(0, pos + delimiter.length());
            
        }
        count--;
    }
    

}




