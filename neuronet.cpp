#include "neuronet.h"

double my_rand(int accuracy) {
    double a = 0;
    a = (qrand() % int (qPow(10, accuracy) + 1))/qPow(10, accuracy);
    return a;
}

Neuronet::Neuronet(int num_hidden_layers_, const QVector<int> &num_neurons_)
{
    norm = 0.002;
    inertion = 0.5;

    input = QVector<float>(num_neurons_[0] + 1, 0);
    output = QVector<float>(num_neurons_[num_neurons_.size() - 1], 0);

    perfect_output = QVector<float>(num_neurons_[num_neurons_.size() - 1], 0);

    num_hidden_layers = num_hidden_layers_;
    num_neurons = num_neurons_;

    for (auto &i : num_neurons) i++;
    num_neurons.push_front(num_neurons_[0] + 1);
    num_neurons.push_back(num_neurons_[num_neurons_.size() - 1]);

    W_weight_map = QVector<QVector<QVector<Weight<float, float>>>>(num_hidden_layers + 1);

    static std::default_random_engine e;
    static std::uniform_real_distribution<> dis(-1, 1);

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    for (int i = 1; i <= num_hidden_layers + 1; i++) {
        QVector<QVector<Weight<float, float>>> main_temp(num_neurons[i]);

        for (int j = 0; j < num_neurons[i]; j++) {
            QVector<Weight<float, float>> temp(num_neurons[i + 1]);
            for (int k = 0; k < num_neurons[i + 1]; k++) {
                temp[k].weight = dis(e);
                temp[k].err = 0;
            }
            main_temp[j] = temp;
        }
        W_weight_map[i] = main_temp;

        NET.push_back(QVector<float>(num_neurons[i], 0));
        O_norm_net.push_back(QVector<float>(num_neurons[i], 0));
        b_error_values.push_back(QVector<float>(num_neurons[i], 0));
    }

    O_norm_net.push_back(QVector<float>(num_neurons_[num_neurons_.size() - 1], 0));
    NET.push_back(QVector<float>(num_neurons_[num_neurons_.size() - 1], 0));
    b_error_values.push_back(QVector<float>(num_neurons_[num_neurons_.size() - 1], 0));
}

void Neuronet::calculate() {
    QTextStream out(stdout);
    O_norm_net[0] = input;
    for (int i = 1; i < NET.size(); i++) {
        for (int j = 1; j < NET[i].size(); j++) {
            NET[i][j] = calculate_net(i, j);
            //out << NET[i][j] << '\n';
            O_norm_net[i][j] = normalization(NET[i][j]);
        }
    }
    output = O_norm_net[O_norm_net.size() - 1];
    //out << output[0] << " " << output[1] << " " << output[2] << '\n';
}

bool Neuronet::training() {
    QTextStream out(stdout);
    O_norm_net[0] = input;
    for (int i = 1; i < NET.size(); i++) {
        for (int j = 0; j < NET[i].size(); j++) {
            NET[i][j] = calculate_net(i, j);
            O_norm_net[i][j] = normalization(NET[i][j]);
        }
    }
    output = O_norm_net[O_norm_net.size() - 1];
    bool converge = true;
    for (int i = 0; i < output.size(); i++) {
        if (abs(perfect_output[i] - output[i]) > 0.001)
            converge = false;
    }
    for (int i = 0; i < output.size(); i++) {
        b_error_values[NET.size() - 1][i] = calculateErrorOutputElement(i);
    }
    for (int i = NET.size() - 2; i >= 0; i--) {
        for (int j = 0; j < NET[i].size(); j++) {
            b_error_values[i][j] = calculateErrorHiddenElement(i, j);
        }
    }
    for (int i = 0; i < W_weight_map.size(); i++) {
        for (int j = 0; j < W_weight_map[i].size(); j++) {
            for (int k = 0; k < W_weight_map[i][j].size(); k++) {
                this->n = n - n * norm;
                this->W_weight_map[i][j][k].err = norm * b_error_values[i + 1][k] * O_norm_net[i][j] + inertion * W_weight_map[i][j][k].err;
                this->W_weight_map[i][j][k].weight = W_weight_map[i][j][k].weight + W_weight_map[i][j][k].err;
            }
        }
    }
    return converge;
}

float Neuronet::calculateErrorOutputElement(int wj) {
    float out = O_norm_net[O_norm_net.size() - 1][wj];
    return (perfect_output[wj] - out) * out * (1 - out);
}

float Neuronet::calculateErrorHiddenElement(int wi, int wj) {
    float sum = 0;
    float out = O_norm_net[wi][wj];
    for (int i = 1; i < O_norm_net[wi + 1].size(); i++) {
        sum += b_error_values[wi + 1][i] * W_weight_map[wi][wj][i].weight;
    }
    return out * (1 - out) * sum;
}

float Neuronet::calculate_net(int wi, int wj) {
    QTextStream out(stdout);
    float sum = W_weight_map[wi - 1][0][wj].weight;
    for (int i = 1; i < O_norm_net[wi - 1].size(); i++) {
        //out << O_norm_net[wi - 1][i] << " : " << W_weight_map[wi - 1][i][wj].weight << "   ";
        sum += O_norm_net[wi - 1][i] * W_weight_map[wi - 1][i][wj].weight;
    }
    //out << "\n";
    //out << sum << '\n';
    return sum;
}

float Neuronet::normalization(float sum) {
    return 1. / (1. + exp(-sum));
    //return (0 > sum ? 0 : sum);
    //return std::tanh(sum);
}

void Neuronet::showWeightMap() {
    QTextStream out(stdout);
    for (const auto &i : qAsConst(W_weight_map)) {
        for (const auto &j : i) {
            for (auto k : j) {
                out << k.weight << " : " << k.err << "  ";
            }
            out << "\n";
        }
        out << "\n********************\n";
    }
}

void Neuronet::setInput(QVector<float> input_) {
    input_.push_front(1);
    input = input_;
}

void Neuronet::setPerfectOutput(QVector<float> perfect_output_) {
    perfect_output = perfect_output_;
}

QVector<float> Neuronet::getOutput() {
    return output;
}
