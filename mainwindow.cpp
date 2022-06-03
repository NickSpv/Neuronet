#include "mainwindow.h"
#include "ui_mainwindow.h"

void clearDir(const QString);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(""); // It worked
    neuronet = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QTextStream out(stdout);

    if (neuronet != nullptr) delete neuronet;
    int n = 0, count_input_neuron = 0, count_output_neuron = 0;
    n = ui->lineEdit_7->text().toInt();
    count_input_neuron = ui->lineEdit_2->text().toInt();
    count_output_neuron = ui->lineEdit->text().toInt();
    std::string myString = ui->lineEdit_3->text().toStdString();
    std::istringstream is( myString );
    QVector<int> myNumbers( ( std::istream_iterator<int>( is ) ), ( std::istream_iterator<int>() ) );
    myNumbers.push_front(count_input_neuron);
    myNumbers.push_back(count_output_neuron);
    neuronet = new Neuronet_M(myNumbers);

    neuronet->setMomentum(ui->lineEdit_4->text().toFloat());
    neuronet->setNorm(ui->lineEdit_5->text().toFloat());

    //neuronet->print_W_weight_matrix();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Text files (*txt)"));

    out << fileName << '\n';

    QVector<QVector<float>> data;
    QString str;
    QFile in(fileName);
    in.open(QIODevice::ReadOnly | QIODevice::Text);
    while (!in.atEnd()) {
        str = in.readLine();
        QStringList strList = str.split(QRegExp("[,]"),QString::SkipEmptyParts);
        QVector<float> vector;
        for(int i=0; i < strList.length(); i++) {
            vector.push_back(strList[i].toFloat());
        }
        data.push_back(vector);
    }
    data.removeLast();

    this->borders = QVector<QPair<float, float>>(count_input_neuron + count_output_neuron);

    for (int i = 0; i < borders.size(); i++) {
        borders[i].first = data[0][i];
        borders[i].second = data[0][i];
        float &min = borders[i].first;
        float &max = borders[i].second;
        for (auto j : data) {
            if (min > j[i]) min = j[i];
            if (max < j[i]) max = j[i];
        }
    }

    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, data.size() - 1);

    QProgressDialog progress("Обучение...", nullptr, 0, n, this);
    progress.setWindowModality(Qt::WindowModal);
    int n_temp = n;
    while (n_temp--) {
        progress.setValue(n - n_temp);
        int i = dist(rd);
        std::vector<float> input_neuron(count_input_neuron);
        std::vector<float> output_neuron(count_output_neuron);
        for (int j = 0; j < count_input_neuron; j++) {
            if (borders[j].second - borders[j].first != 0)
                input_neuron[j] = (data[i][j] - borders[j].first) / (borders[j].second - borders[j].first);
            else
                input_neuron[j] = 0;
        }
        for (int j = 0; j < count_output_neuron; j++) {
            int index = j + count_input_neuron;
            if (borders[index].second - borders[index].first != 0)
                output_neuron[j] = (data[i][index] - borders[index].first) / (borders[index].second - borders[index].first);
            else
                output_neuron[j] = 0;
        }
        neuronet->traning(input_neuron, output_neuron);
    }

    //neuronet->print_W_weight_matrix();

    out  << "\ndone\n";
}


void MainWindow::on_pushButton_2_clicked()
{
    //neuronet->print_W_weight_matrix();
    QTextStream out(stdout);
    QString str = QInputDialog::getText(this, tr("Введите значения входных нейронов"), tr("Введите значения через запятую:"), QLineEdit::Normal);
    QStringList strList = str.split(QRegExp("[,]"),QString::SkipEmptyParts);
    std::vector<float> vector;
    for(int i = 0; i < strList.length(); i++) {
        float num = strList[i].toFloat();
        if (borders[i].second - borders[i].first != 0)
            num = (num - borders[i].first) / (borders[i].second - borders[i].first);
        else
            num = 0;
        //out << num << '\n';
        vector.push_back(num);
    }
    nc::NdArray<float> output_neuron = this->neuronet->calculate(vector);
    output_neuron.print();
    QString output;
    for (auto i : output_neuron) {
        output = output + QString::number(i * (this->borders[strList.length()].second - this->borders[strList.length()].first) + this->borders[strList.length()].first) + " ";
        out << '\n' << QString::number(i);
    }
    QMessageBox msgBox;
    msgBox.setText("Значения выходных нейронов: " + output);
    msgBox.exec();
}

