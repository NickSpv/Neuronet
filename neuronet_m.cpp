#include "neuronet_m.h"

Neuronet_M::Neuronet_M()
{

}

Neuronet_M::Neuronet_M(QVector<int> neuronet_conf)
{
    norm = 0.7;
    momentum = 0.5;
    neuronet_conf[neuronet_conf.size() - 1]--;
    for (int i = 0; i < neuronet_conf.size() - 1; i++) {
        float low = -0.3;
        float high = 0.3;
        this->W_weight_matrix.push_back(nc::random::uniform(nc::Shape(neuronet_conf[i] + 1, neuronet_conf[i + 1] + 1), low, high));
        this->dW_weight_error.push_back(nc::NdArray<float>(neuronet_conf[i] + 1, neuronet_conf[i + 1] + 1) = 0);
    }
    this->B_neuron_error = QVector<nc::NdArray<float>>(neuronet_conf.size());
}

nc::NdArray<float> Neuronet_M::calculate(nc::NdArray<float> input_data)
{
    QTextStream out(stdout);
    input_data.print();
    input_data = nc::append(input_data, nc::NdArray<float>(1) = 1);
    input_data.print();
    for (nc::NdArray<float> i : this->W_weight_matrix) {
        input_data[-1] = 1.;
        input_data = input_data.dot(i);
        input_data.print();
        input_data = normalize(input_data, false);
        out << "Нормализованные значения: ";
        input_data.print();
    }
    return input_data;
}

nc::NdArray<float> Neuronet_M::traning(nc::NdArray<float> input_data, nc::NdArray<float> perfect_output)
{
    input_data = nc::append(input_data, nc::NdArray<float>(1) = 1);
    this->O_neuron_value.push_back(input_data);
    for (nc::NdArray<float> i : this->W_weight_matrix) {
        input_data[-1] = 1.;
        input_data = input_data.dot(i);
        input_data = normalize(input_data, false);
        this->O_neuron_value.push_back(input_data);
    }
    int O_size = this->O_neuron_value.size() - 1;
    int B_size = this->B_neuron_error.size() - 1;
    this->B_neuron_error[B_size] = (perfect_output - this->O_neuron_value[O_size]) * normalize(this->O_neuron_value[O_size], true);
    //nc::mean(nc::abs(this->B_neuron_error[B_size])).print();
    for (int i = this->B_neuron_error.size() - 2; i >= 0; i--) {
        this->B_neuron_error[i] = this->B_neuron_error[i + 1].dot(this->W_weight_matrix[i].transpose()) * normalize(this->O_neuron_value[i], true);
        this->dW_weight_error[i] = this->O_neuron_value[i].transpose().dot(this->B_neuron_error[i + 1]) * this->norm + this->dW_weight_error[i] * this->momentum;
        this->W_weight_matrix[i] += this->dW_weight_error[i];
        //this->B_neuron_error[i].print();
        this->B_neuron_error[i][-1] = 0;
    }
    this->O_neuron_value.clear();
    return input_data;
}

//**************************************************************************

void Neuronet_M::print_W_weight_matrix()
{
    for (int i = 0; i < W_weight_matrix.size(); i++) {
        W_weight_matrix[i].print();
    }
}

void Neuronet_M::save_weight_matrix()
{
    QFile file("weight_matrix");
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << "something" << endl;
    }
}

nc::NdArray<float> Neuronet_M::normalize(nc::NdArray<float> data, bool isDiff)
{
    for (auto &i : data) {
        i = func_norm(i, isDiff);
    }
    return data;
}

float Neuronet_M::func_norm(float data, bool isDiff)
{
    if (isDiff)
        return data * (1. - data);
    return 1. / (1. + nc::exp(-data));
}

void Neuronet_M::setNorm(float norm_)
{
    this->norm = norm_;
}

void Neuronet_M::setMomentum(float momentum_)
{
    this->momentum = momentum_;
}
