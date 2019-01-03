#pragma once
#ifndef DATA_FETCH
#define DATA_FETCH

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

#define ERROR(T) if(T) std::cout << __FUNCTION__ << " @"\
<< __TIMESTAMP__ << " for " << #T " error\n", exit(-1)
#define SNP std::string::npos

const int  input_layer_num = 28 * 28;
const char character = ',';

class DataManager;

class DataManager {
public:
    DataManager() { }
    DataManager(const char *);
    ~DataManager();

    bool openFile(const char *);

    void fetchSample(std::vector<double> &, const int = input_layer_num);
private:
    std::ifstream inFile;
    inline double _stod(std::string);
};

#endif // !DATA_FETCH

