#include "Data_fetch.h"


DataManager::DataManager(const char *filename) {
    inFile.open(filename, std::ios::in);
    ERROR(!inFile.good());
}

DataManager::~DataManager() {
    if (inFile.is_open())
        inFile.close();
}

bool DataManager::openFile(const char *filename) {
    if (inFile.is_open())
        inFile.close();
    inFile.open(filename, std::ios::in);
    if (!inFile.good())
        std::exit(-1);
    return inFile.good();
}

void DataManager::fetchSample(std::vector<double> &spt, const int spt_num) {
    spt.clear();
    std::string line;
    inFile >> line;
    int front = 0, last = 0, cnt = 0, len = line.length();
    while ((last = line.find(character, front)) && cnt <= spt_num) {
        spt.push_back(_stod(line.substr(front, (last == SNP ? len : last) - front)));
        front = last + 1, cnt++;
    }
}

inline double DataManager::_stod(std::string spl) {
    return double(stoi(spl, nullptr, 10));
}