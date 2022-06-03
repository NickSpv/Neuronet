#ifndef NEURONET_M_H
#define NEURONET_M_H

#include <NumCpp.hpp>
#include <QVector>
#include <math.h>
#include <QTextStream>
#include <QFile>

class Neuronet_M
{
public:
    Neuronet_M();
    Neuronet_M(QVector<int> neuronet_conf);
    nc::NdArray<float> calculate(nc::NdArray<float> input_data);
    nc::NdArray<float> traning(nc::NdArray<float> input_data,  nc::NdArray<float> perfect_output);

public:
    void print_W_weight_matrix();
    void save_weight_matrix();
    void setNorm(float);
    void setMomentum(float);

private:
    float func_norm(float data, bool isDiff);
    nc::NdArray<float> normalize(nc::NdArray<float> data, bool isDiff);

private:
    QVector<nc::NdArray<float>> O_neuron_value;
    QVector<nc::NdArray<float>> B_neuron_error;
    QVector<nc::NdArray<float>> W_weight_matrix;
    QVector<nc::NdArray<float>> dW_weight_error;
    float momentum;
    float norm;
};

#endif // NEURONET_M_H
