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
    ~DataManager() { inFile.close(); }

    bool openFile(const char *);

    void fetchSample(std::vector<double> &, const int);
private:
    std::ifstream inFile;
    inline double _stod(std::string);
};

DataManager::DataManager(const char *filename) {
    inFile.open(filename, std::ios::in);
    ERROR(!inFile.good());
}

bool DataManager::openFile(const char *filename) {
    inFile.open(filename, std::ios::in);
    return inFile.good();
}

void DataManager::fetchSample(std::vector<double> &spt, const int spt_num  = input_layer_num) {
    spt.clear();
    std::string line;
    inFile >> line;
    int front = 0, last = 0, cnt = 0, len = line.length();
    while ((last = line.find(character, front)) && cnt <= spt_num) {
        spt.push_back(_stod(line.substr(front,  (last == SNP ? len : last) - front)));
        front = last + 1, cnt ++;
    }
}

inline double DataManager::_stod(std::string spl) {
    return double(stoi(spl, nullptr, 10));
}

#endif // !DATA_FETCH

