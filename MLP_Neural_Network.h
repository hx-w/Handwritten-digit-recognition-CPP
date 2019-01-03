#pragma once
#ifndef MLP_NEURAL_NETWORKS
#define MLP_NEURAL_NETWORKS

#include <cmath>
#include <vector>
#include <fstream>
#include <iostream>

const int MATRIX_MAX = 800;
const double response = 1;

class Matrix;
class Neuron;
class Layer;
class Network;

inline double sigmoid(double);

/*******************************************/
class Matrix {
public:
    Matrix(int, int, double);
    ~Matrix();

    double get_curr(int, int);
    void set_curr(int, int, double);
    std::pair<int, int> get_rc();

private:
    int row_Max;
    int col_Max;
    std::vector<double> weights[MATRIX_MAX];
};

/*******************************************/

class Neuron {
public:
    Neuron(double, double);
    ~Neuron();

    double state;
    double sita;
};

/*******************************************/

class Layer {
public:
    Layer(int);
    ~Layer();

    void reset_size(int);
    void copy(std::vector<double> &);
    void Mat_mul_Lay(Matrix &, Layer &);

    int size;
    std::vector<Neuron> nodes;
};

/*******************************************/

class Network {
public:
    Network(double, int, int, int, int);
    ~Network();

    void init();

    void get_sam(std::vector<double> &);

    void get_result(std::vector<double> &);

    void trainning();
    void testing();

    void save(const char *);
    void read(const char *);
    
    void show_state_sita();
private:
    void _forward_pro();
    void _back_pro();
    
    void calc_sita();
    void updata_weight();


    double Ed;
    double rate;
    int hl_num;
    int hl_nodes_num;
    int ip_nodes_num;
    int op_nodes_num;
    Layer input_layer;
    Layer output_layer;
    Layer sam_output;
    std::vector<Layer> hidden_layer;
    std::vector<Matrix> mats;
};
#endif // !MLP_NEURAL_NETWORKS

