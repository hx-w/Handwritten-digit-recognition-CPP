#pragma once
#ifndef ASSISTANT
#define ASSISTANT

#include "Data_fetch.h"
#include "MLP_Neural_Network.h"

class Assistant;

class Assistant {
public:
    Assistant();
    ~Assistant();

    void set_param();

    void working();

    void pause();
private:
    bool __show_res() const;

    bool __show_detail;

    int test_num;
    int train_num;
    int num_count;

    double show_gap;
    int accu_num;

    DataManager DM;
    Network NET;

    std::vector<double> in_out;
    std::vector<double> result;

    std::string Data_train;
    std::string Data_test;
    std::string Data_read;
    std::string Data_save;
    std::string Accu_save;
};

#endif // !ASSISTANT
