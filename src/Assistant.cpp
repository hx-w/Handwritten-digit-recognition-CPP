#include "Assistant.h"

/************** Public *************/

Assistant::Assistant() {
    __show_detail = false;
    test_num  = 0;
    train_num = 0;
    num_count = 0;
    accu_num  = 0;
    show_gap = 0.01;

    std::srand(std::time(NULL) + std::rand());
    NET.init();
}

Assistant::~Assistant() { }

void Assistant::set_param() {
    printf("Set parameters before working: \n\n");
    std::string choice;
    printf("*.Want show every record detail?\ninput yes or no\n>> ");
    std::cin >> choice;
    __show_detail = (choice[0] == 'y' || choice[0] == 'Y');

    printf("\n*.Want to train before testing or test directly with existing parameters?\n");
    printf("input yes or no\n>> ");
    std::cin >> choice;
    if (choice[0] == 'y' || choice[0] == 'Y') {
        printf("\n*.Input the path of train_data\n>> ");
        std::cin >> Data_train;
        printf("\n*.Input the records num want to be trained(max 60000)\n>> ");
        std::cin >> train_num;
        printf("\n*.Input the file name of trained parameters\n>> ");
        std::cin >> Data_save;
    }
    else {
        printf("\n*.Input the path of parameters_data\n>> ");
        std::cin >> Data_read;
    }

    printf("\n*.Input the path of test_data\n>> ");
    std::cin >> Data_test;
    printf("\n*.Input the records num want to be tested(max 10000)\n>> ");
    std::cin >> test_num;

    printf("\n*.Input the file name of accuracy rate result\n>> ");
    std::cin >> Accu_save;
}

void Assistant::working() {
    std::ofstream outFiles(Accu_save.c_str(), std::ios::app);
    outFiles << "________________ * ________________\n";

    std::time_t rawtime;
    tm *ptminfo;
    std::time(&rawtime);
    ptminfo = std::localtime(&rawtime);
    
    outFiles << "Start time: " << \
        ptminfo->tm_mon + 1 << "-" << ptminfo->tm_mday << " " <<\
        ptminfo->tm_hour << ":" << ptminfo->tm_min << \
        ":" << ptminfo->tm_sec << std::endl;

    double proce = 0;
    if (train_num) {
        printf("______________train part_____________\n");
        DM.openFile(Data_train.c_str());
        for (num_count = 0; num_count < train_num; num_count++) {
            DM.fetchSample(in_out);
            NET.get_sam(in_out);
            NET.trainning();
            NET.get_result(result);

            __show_res();
            
            if (1.0 * num_count / train_num - proce > show_gap) {
                printf("Processing: %.2lf%c\n", 100.0 * num_count / train_num, '%');
                proce = 1.0 * num_count / train_num;
            }
        }

        NET.save(Data_save.c_str());
    }
    else {
        NET.read(Data_read.c_str());
    }

    printf("______________test part_____________\n");
    proce = 0;
    DM.openFile(Data_test.c_str());
    for (num_count = 0; num_count < test_num; num_count++) {
        DM.fetchSample(in_out);
        NET.get_sam(in_out);
        NET.testing();
        NET.get_result(result);

        accu_num += __show_res();

        if (1.0 * num_count / test_num - proce > show_gap) {
            printf("Processing: %.2lf%c\n", 100.0 * num_count / test_num, '%');
            proce = 1.0 * num_count / test_num;
        }
    }

    if (Data_save.empty()) {
        outFiles << "Param: " << Data_read << std::endl;
    }
    else {
        outFiles << "Train_num: " << train_num << std::endl;
        outFiles << "Param    : " << Data_save << std::endl;
    }
    outFiles << "Test_num : " << test_num << std::endl;
    outFiles << "-*- Accuracy -*-: " << 1.0 * accu_num / test_num << std::endl;

    std::time(&rawtime);
    ptminfo = std::localtime(&rawtime);
    outFiles << "Finish time: " << \
        ptminfo->tm_mon + 1 << "-" << ptminfo->tm_mday << " " <<\
        ptminfo->tm_hour << ":" << ptminfo->tm_min << \
        ":" << ptminfo->tm_sec << std::endl;
    outFiles << "__________*_____ * ______*_________\n";
    outFiles.close();

    printf("______________test done_____________\n");
}

void Assistant::pause() {
    printf("Press Enter to exit.");
    std::getchar();
    std::getchar();
}

/************** Public *************/
bool Assistant::__show_res() const {
    if (__show_detail) {
        printf("num: %d\nEd: %lf\nRes: ", num_count, result[0]);
    }
    
    int tar = 1, res = 1 + op_nodes_num;
    for (int inc = 0; inc < op_nodes_num; inc++) {
        if (__show_detail)
            printf("%.6lf%c", result[inc + 1], " \n"[inc == op_nodes_num - 1]);
        if (result[inc + 1] > result[tar]) tar = inc + 1;
    }

    if (__show_detail)
        printf("Sam: ");
    for (int inc = 0; inc < op_nodes_num; inc++) {
        if (__show_detail)
            printf("%.6lf%c", result[inc + 1 + op_nodes_num], " \n"[inc == op_nodes_num - 1]);
        if (result[inc + 1 + op_nodes_num] > result[res])
            res = inc + 1 + op_nodes_num;
    }
    if (__show_detail) {
        printf("res: %d  vs  tar: %d\n", tar - 1, res - 1 - op_nodes_num);
        if (tar - 1 == res - 1 - op_nodes_num)
            printf("Correct\n");
        else
            printf("Wrong\n");
        printf("------------------------------------\n");
    }
    return tar - 1 == res - 1 - op_nodes_num;
}