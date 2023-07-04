#ifndef _PARL_MBC_H_
#define _PARL_MBC_H_

#include <iostream>
#include <string>
#include "utility.hpp"
#include "shared_var.hpp"
#include "thread_config.hpp"
#include "CSR.hpp"

using namespace std;

/* Function Declaration */
void parlmbc(X_set X, L_set Gamma, R_set tail, int ms);

void parlmbc_main(string file_path);


#endif // _PARL_MBC_H_