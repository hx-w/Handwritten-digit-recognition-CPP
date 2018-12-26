#include "MLP_Neural_Network.h"

#include <algorithm>
#include <cstring>

inline double sigmoid(double rhs) {
    return 1 / (1 + std::exp(-rhs));
}

/************ Matrix ******************/

Matrix::Matrix(int Row = 0, int Col = 0, double rhs = 0.01) {
    row_Max = Row; col_Max = Col;
    for (int row = 0; row < row_Max; row++) {
        for (int col = 0; col < col_Max; col++) {
            weights[row].push_back(rhs);
        }
    }
}

Matrix::~Matrix() { }

void Matrix::set_curr(int row, int col, double val) {
    weights[row][col] = val;
}

double Matrix::get_curr(int row, int col) {
    return weights[row][col];
}

std::pair<int, int> Matrix::get_rc() {
    return std::pair<int, int>(row_Max, col_Max);
}

/**************************************/

/************* Neuron *****************/

Neuron::Neuron(double sta = 1, double sit = 0) {
    state = sta; sita = sit;
}

Neuron::~Neuron() { }

/**************************************/

/************** Layer *****************/

Layer::Layer(int sz = 0) {
    size = sz;
    nodes.clear();
    for (int inc = 0; inc < size; inc++)
        nodes.push_back(Neuron());
}

Layer::~Layer() {
    nodes.clear();
}

void Layer::reset_size(int sz) {
    nodes.clear();
    size = sz;
    for (int inc = 0; inc < size; inc++)
        nodes.push_back(Neuron());
}

void Layer::copy(std::vector<double> &sample) {
    nodes.clear();
    std::vector<double>::iterator iter = sample.begin();
    for ( ; iter != sample.end(); iter++) {
        nodes.push_back(Neuron((*iter)));
        if (iter + 1 == sample.end())
            nodes.push_back(1.0);
    }
    size = nodes.size();
}

void Layer::Mat_mul_Lay(Matrix &mat, Layer &lay) {
    std::pair<int, int> pac = mat.get_rc();
    if (pac.second != lay.size) return;
    nodes.clear();
    for (int row = 0; row < pac.first; row++) {
        double lay_rowNum = 0.0;
        for (int col = 0; col < pac.second; col++)
            lay_rowNum += mat.get_curr(row, col) * lay.nodes[col].state;
        nodes.push_back(Neuron(sigmoid(lay_rowNum)));
    }
    size = nodes.size();
}

/**************************************/

/************** Network ****************/

Network::Network(double ra = 0, int hln = 0, int hlo = 0, 
    int ipo = 0, int opo = 0) {
    rate = ra; hl_num = hln; hl_nodes_num = hlo;
    ip_nodes_num = ipo; op_nodes_num = opo;
    Ed = 0;
}

Network::~Network() {
    hidden_layer.clear();
}

void Network::init() {
    for (int inc = 0; inc < hl_num; inc ++) {
        hidden_layer.push_back(Layer(hl_nodes_num));
    }

    mats.push_back(Matrix(hl_nodes_num, ip_nodes_num + 1));
    
    for (int inc = 0; inc < hl_num - 1; inc++)
        mats.push_back(Matrix(hl_nodes_num, hl_nodes_num + 1));

    mats.push_back(Matrix(op_nodes_num, hl_nodes_num + 1));
}

void Network::get_sam(std::vector<double> &inp_oup) {
    double tar = (*inp_oup.begin()) * 255.0;
    std::vector<double> outp;
    for (int inc = 0; inc < op_nodes_num; inc++) {
        outp.push_back((int(tar) == inc ? 1 : 0));
    }
    inp_oup.erase(inp_oup.begin());

    input_layer.copy(inp_oup);
    sam_output.copy(outp);
}

void Network::_forward_pro() {
    hidden_layer[0].Mat_mul_Lay(mats[0], input_layer);
    hidden_layer[0].nodes.push_back(Neuron());
    hidden_layer[0].size++;
    for (int idx = 1; idx < hl_num; idx++) {
        hidden_layer[idx].Mat_mul_Lay(mats[idx], hidden_layer[idx - 1]);
        hidden_layer[idx].nodes.push_back(Neuron());
        hidden_layer[idx].size++;
    }

    output_layer.Mat_mul_Lay(mats[hl_num], hidden_layer[hl_num - 1]);
    Ed = 0;
    for (int inc = 0; inc < op_nodes_num; inc++) {
        Ed += std::pow(output_layer.nodes[inc].state - sam_output.nodes[inc].state, 2) / 2.0;
    }
}

void Network::_back_pro() {
    // calc sita
    calc_sita();

    // update weights
    updata_weight();
}

void Network::calc_sita() {
    for (int idx = 0; idx < op_nodes_num; idx++) {
        double gey = output_layer.nodes[idx].state;
        double tar = sam_output.nodes[idx].state;
        output_layer.nodes[idx].sita = \
            gey * (1 - gey) * (tar - gey);
    }

    for (int lay_idx = hl_num - 1; lay_idx >= 0; lay_idx--) {
        int tar_mat_num = lay_idx + 1;
        std::pair<int, int> pack = mats[tar_mat_num].get_rc();
        for (int node_idx = 0; node_idx < hl_nodes_num; node_idx++) {
            double prev = hidden_layer[lay_idx].nodes[node_idx].state * \
                (1 - hidden_layer[lay_idx].nodes[node_idx].state);
            double post = 0;
            int rowNum = pack.first;
            for (int row = 0; row < rowNum; row++) {
                post += mats[tar_mat_num].get_curr(row, 0) *
                    (lay_idx == hl_num - 1 ? 
                        output_layer.nodes[row].state :
                        hidden_layer[lay_idx + 1].nodes[row].state);
            }
            hidden_layer[lay_idx].nodes[node_idx].sita = prev * post;
        }
    }
}

void Network::updata_weight() {
    std::pair<int, int> pack = mats[0].get_rc();

    for (int mat_idx = 0; mat_idx < hl_num + 1; mat_idx ++) {
        for (int row = 0; row < pack.first; row++) {
            for (int col = 0; col < pack.second; col++) {
                double _sita = mat_idx == hl_num ?
                    output_layer.nodes[row].sita :
                    hidden_layer[mat_idx].nodes[row].sita;
                double _xput = mat_idx == 0 ?
                    input_layer.nodes[col].state :
                    hidden_layer[mat_idx - 1].nodes[col].state;
                double _grap = rate * _sita * _xput;
                double _orig = mats[mat_idx].get_curr(row, col);
                mats[mat_idx].set_curr(row, col, _orig + _grap);
            }
        }
        if (mat_idx < hl_num)
            pack = mats[mat_idx + 1].get_rc();
    }

}

void Network::trainning() {
    _forward_pro();
    _back_pro();
}

void Network::testing() {
    _forward_pro();
}

void Network::get_result(std::vector<double> &rhs) {
    rhs.clear();
    rhs.push_back(Ed);
    for (int inc = 0; inc < op_nodes_num; inc++)
        rhs.push_back(output_layer.nodes[inc].state);
    for (int inc = 0; inc < op_nodes_num; inc++)
        rhs.push_back(sam_output.nodes[inc].state);
}

void Network::save(const char *filename) {
    std::ofstream putFile(filename, std::ios::out);

    for (int mat_idx = 0; mat_idx < hl_num + 1; mat_idx++) {
        std::pair<int, int> pack = mats[mat_idx].get_rc();
        putFile << pack.first << " " << pack.second << std::endl;
        for (int row = 0; row < pack.first; row++) {
            for (int col = 0; col < pack.second; col++) {
                putFile << mats[mat_idx].get_curr(row, col) << " \n"[col == pack.second - 1];
            }
        }
    }

    putFile.close();
}

void Network::read(const char *filename) {
    std::ifstream readFile(filename, std::ios::in);

    for (int mat_idx = 0; mat_idx < hl_num + 1; mat_idx++) {
        double Row = 0, Col = 0;
        readFile >> Row >> Col;
        for (int row = 0; row < Row; row++) {
            for (int col = 0; col < Col; col++) {
                double val = 0;
                readFile >> val;
                mats[mat_idx].set_curr(row, col, val);
            }
        }
    }

    readFile.close();
}