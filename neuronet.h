#ifndef NEURONET_H
#define NEURONET_H

#include <QVector>
#include <QPair>
#include <random>
#include <QTextStream>
#include <QtMath>
#include <math.h>
#include <QTime>
#include <QRandomGenerator>

template <class weight_, class err_>
class Weight : QPair<weight_, err_> {
public:
    weight_ &weight = this->first;
    err_ &err = this->second;
};

class Neuronet
{
public:
    Neuronet(int, const QVector<int>&);

    void calculate();
    bool training();

    void showWeightMap();
    void setInput(QVector<float>);
    void setPerfectOutput(QVector<float>);
    QVector<float> getOutput();

    float normalization(float);
    float calculateErrorOutputElement(int);
    float calculateErrorHiddenElement(int, int);

private:
    QVector<QVector<QVector<Weight<float, float>>>> W_weight_map;
    QVector<QVector<float>> O_norm_net;
    QVector<QVector<float>> NET;
    QVector<QVector<float>> b_error_values;

    float n;
    int num_hidden_layers;
    QVector<int> num_neurons;
    float inertion;
    float norm;
    QVector<float> input;
    QVector<float> perfect_output;
    QVector<float> output;

    float calculate_net(int, int);
};

#endif // NEURONET_H
