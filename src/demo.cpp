#include "Data_fetch.h"
#include "MLP_Neural_Network.h"

#include <cstdlib>
#include <iomanip>
using namespace std;

const char *Data_train = "./mnist_train.csv";
const char *Data_save  = "./param_save.dat";


const double rate = 0.35;
const int hl_num = 1;
const int ip_nodes_num = input_layer_num;
const int op_nodes_num = 10;
const int hl_nodes_num = 300;

const int train_round = 20;
const int test_round  = 100;
const int total_ = 1000;

bool show_result(vector<double> &re, int cnt) {
    cout << "num: " << cnt << endl;
    cout << "Ed: " << re[0] << endl;
    cout << "Res: ";
    int tar = 1, res = 1 + op_nodes_num;
    for (int inc = 0; inc < op_nodes_num; inc++) {
        cout << setw(8) << re[inc + 1] << " \n"[inc == op_nodes_num - 1];
        if (re[inc + 1] > re[tar]) tar = inc + 1;
    }
    cout << "Sam: ";
    for (int inc = 0; inc < op_nodes_num; inc++) {
        cout << setw(8) << re[inc + 1 + op_nodes_num] << " \n"[inc == op_nodes_num - 1];
        if (re[inc + 1 + op_nodes_num] > re[res])
            res = inc + 1 + op_nodes_num;
    }
    cout << "tar: " << tar - 1 << "  vs  res: " << res - 1 - op_nodes_num << endl;
    cout << (tar - 1 == res - 1 - op_nodes_num ? "Correct" : "Wrong") << endl;
    cout << "------------------------------------" << endl;
    return tar - 1 == res - 1 - op_nodes_num;
}

int main() {
    srand(time(NULL));

    DataManager DM;
    DM.openFile(Data_train);
    vector<double> in_out;
    vector<double> result;


    Network net(rate, hl_num, hl_nodes_num, 
    ip_nodes_num, op_nodes_num);

    net.init();

    net.read(Data_save);
    for (int cnt = 0; cnt < total_; cnt++) {
        cout << "train round _______________________" << endl;
        for (int train_ = 0; train_ < train_round; train_++) {
            DM.fetchSample(in_out);
            net.get_sam(in_out);
            net.trainning();
            net.get_result(result);

            show_result(result, cnt * total_ + train_);
            //net.show_state_sita();
        }

        net.save(Data_save);
        cout << "test round _________________________" << endl;
        int cor = 0;
        for (int test_ = 0; test_ < test_round; test_++) {
            DM.fetchSample(in_out);
            net.get_sam(in_out);
            net.trainning();
            net.get_result(result);

            cor += show_result(result, cnt * total_ + test_ + train_round);
        }
        cout << "____________test result_______________" << endl;
        cout << "correct rate: " << setprecision(4) << 100.0 * cor / test_round << "%" << endl;
        cout << "______________________________________" << endl;
    }
    
    system("pause");
    return 0;
}