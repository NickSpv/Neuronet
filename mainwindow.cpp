#include "mainwindow.h"
#include "ui_mainwindow.h"

void clearDir(const QString);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(""); // It worked

    QPixmap image("/home/Pavel/MathematicalAnalysis/images/none.png");
    QPixmap arrow_img("/home/Pavel/MathematicalAnalysis/images/arrow.png");

    ui->input_image_label->resize(image.size());
    ui->input_image_label->setPixmap(image);
    ui->output_image_label->resize(image.size());
    ui->output_image_label->setPixmap(image);
    ui->arrow_image_label->resize(arrow_img.size());
    ui->arrow_image_label->setPixmap(arrow_img);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_choose_img_in_clicked()
{
    QString patch_to_image_in = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));
    QImage image_in(patch_to_image_in);
    this->input_image = image_in;

    ui->input_image_label->resize(image_in.scaled(200, 200).size());
    ui->input_image_label->setPixmap(QPixmap::fromImage(image_in).scaled(200, 200));
}


void MainWindow::on_choose_img_out_clicked()
{
    QString patch_to_image_out = QFileDialog::getOpenFileName(this, tr("Open File"), "/home", tr("Images (*.png *.xpm *.jpg)"));
    QImage image_out(patch_to_image_out);
    this->output_image = image_out;

    ui->output_image_label->resize(image_out.scaled(200, 200).size());
    ui->output_image_label->setPixmap(QPixmap::fromImage(image_out).scaled(200, 200));
}

void MainWindow::generateImage(Neuronet& neuronet, int x_size, int y_size, int& img_count) {
    QTextStream out(stdout);
    QImage result_image(x_size, y_size, QImage::Format_ARGB32);
    for (int x = 0; x < x_size; x++) {
        for (int y = 0; y < y_size; y++) {
            //float K = 0.5;
            float x_ = (float)x/x_size;
            float y_ = (float)y/y_size;
            x_ = x_ * 2 - 1;
            y_ = y_ * 2 - 1;
            neuronet.setInput({x_, y_});
            //out << (float)(x/x_size) << " " << (float)(y/y_size) << '\n';
            neuronet.calculate();
            QVector<float> colors = neuronet.getOutput();
            result_image.setPixel(x, y, QColor(abs(colors[0]*255), abs(colors[1]*255), abs(colors[2]*255)).rgb());
        }
    }

    QString filename = "/home/Pavel/MathematicalAnalysis/images_output/%1.jpg";
    result_image.save(filename.arg(img_count), "JPG");
    img_count++;
}

void MainWindow::generateImage(Neuronet_M& neuronet, int x_size, int y_size, int& img_count) {
    QTextStream out(stdout);
    QImage result_image(x_size, y_size, QImage::Format_ARGB32);
    for (int x = 0; x < x_size; x++) {
        for (int y = 0; y < y_size; y++) {
            //float K = 0.5;
            float x_ = (float)x/x_size;
            float y_ = (float)y/y_size;
            x_ = x_ * 2 - 1;
            y_ = y_ * 2 - 1;
            nc::NdArray<float> colors = neuronet.calculate({x_, y_});;
            result_image.setPixel(x, y, QColor(abs(colors[0]*255), abs(colors[1]*255), abs(colors[2]*255)).rgb());
            //out << x_ << " " << y_ << " : " << abs(colors[0]*255) << " " << abs(colors[1]*255) << " " << abs(colors[2]*255) << '\n';
        }
    }

    QString filename = "/home/Pavel/MathematicalAnalysis/images_output/%1.jpg";
    result_image.save(filename.arg(img_count), "JPG");
    img_count++;
}

double my_random(int accuracy) {
    double a = 0;
    a = (qrand() % int (qPow(10, accuracy) + 1))/qPow(10, accuracy);
    return a;
}

void MainWindow::on_start_clicked()
{
    /*Neuronet_M neuronet({1, 2, 1});
    //neuronet.print_W_weight_matrix();
    for (float i = 0; i < 100; i++) {
        neuronet.traning({1. / 10}, {2. / 10}).print();
        //neuronet.print_W_weight_matrix();
    }
*/

    Neuronet_M neuronet({2, 60, 40, 20, 3});
//    neuronet.print_W_weight_matrix();
//    nc::NdArray<float> test = {0.3, 0.4};
//    test.print();
//    test = neuronet.calculate(test);
//    test.print();

    clearDir("/home/Pavel/MathematicalAnalysis/video_output");

    int x_size = std::min(input_image.width(), output_image.width());
    int y_size = std::min(input_image.height(), output_image.height());
//    x_size = 100;
//    y_size = 100;
    int img_count = 0;

    QImage result_image(x_size, y_size, QImage::Format_ARGB32);

//    neuronet.print_W_weight_matrix();

    QTextStream out(stdout);
    out << x_size << " " << y_size << '\n';
    generateImage(neuronet, x_size, y_size, img_count);

    int n = 100000;
    out << n << '\n';

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    while (n--) {
        for (int x = 0; x < x_size; x++) {
            for (int y = 0; y < y_size; y++) {
                float x_ = (float)x/x_size;
                float y_ = (float)y/y_size;
                x_ = x_ * 2 - 1;
                y_ = y_ * 2 - 1;
                QColor rgb = input_image.pixel(x, y);
                float r = rgb.red();
                float g = rgb.green();
                float b = rgb.blue();
                float K = 255;
                neuronet.traning({x_, y_}, {r / K, g / K, b / K});

                //out << neuronet.getOutput()[0] << " " << neuronet.getOutput()[1] << " " << neuronet.getOutput()[2] << '\n';
                //    neuronet.print_W_weight_matrix();
            }
        }
        if(n % 10 == 0)generateImage(neuronet, x_size, y_size, img_count);
        //out << x << " " << y << " : " << int(x_ * 255) << " " << int(y_ * 255) << '\n';
    }

/*    clearDir("/home/Pavel/MathematicalAnalysis/video_output");

    int x_size = std::min(input_image.width(), output_image.width());
    int y_size = std::min(input_image.height(), output_image.height());
    //x_size = 1000;
    //y_size = 1000;
    int img_count = 0;

    QImage result_image(x_size, y_size, QImage::Format_ARGB32);

    Neuronet neuronet(3, {60, 40, 20});

    neuronet.showWeightMap();
    bool flag = false;

    QTextStream out(stdout);
    out << x_size << " " << y_size << '\n';

    generateImage(neuronet, x_size, y_size, img_count);



    int n = 1000000;
    out << n << '\n';


    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    while (n--) {
        for (int x = 0; x < x_size; x++) {
            for (int y = 0; y < y_size; y++) {
                float x_ = (float)x/x_size;
                float y_ = (float)y/y_size;
                x_ = x_ * 2 - 1;
                y_ = y_ * 2 - 1;
                neuronet.setInput({x_, y_});
                QColor rgb = input_image.pixel(x, y);
                float r = rgb.red();
                float g = rgb.green();
                float b = rgb.blue();
                float K = 255;
                neuronet.setPerfectOutput({r / K, g / K, b / K});
                flag = neuronet.training();
                //out << neuronet.getOutput()[0] << " " << neuronet.getOutput()[1] << " " << neuronet.getOutput()[2] << '\n';
            }
        }
        generateImage(neuronet, x_size, y_size, img_count);
        //out << x << " " << y << " : " << int(x_ * 255) << " " << int(y_ * 255) << '\n';
    }
*/
/*
    while (n--) {
        float x_ = my_random(7);
        float y_ = my_random(7);
        int x = x_ * x_size;
        int y = y_ * y_size;
        x_ = x_ * 2 - 1;
        y_ = y_ * 2 - 1;
        neuronet.setInput({x_, y_});
        QColor rgb = input_image.pixel(x, y);
        float r = rgb.red();
        float g = rgb.green();
        float b = rgb.blue();
        float K = 255;
        neuronet.setPerfectOutput({r / K, g / K, b / K});
        flag = neuronet.training();
        //out << neuronet.getOutput()[0] << " " << neuronet.getOutput()[1] << " " << neuronet.getOutput()[2] << '\n';
        if(n % 1000 == 0)
            generateImage(neuronet, x_size, y_size, img_count);
        //out << x << " " << y << " : (" << r << ", " << g << ", " << b << ")\n";
    }


    generateImage(neuronet, x_size, y_size, img_count);

    std::string command_generate_gif = "ffmpeg -f image2 -i /home/Pavel/MathematicalAnalysis/images_output/%d.jpg"
                                       " /home/Pavel/MathematicalAnalysis/video_output/result.mp4";
    system(command_generate_gif.c_str());
    //system("xdg-open /home/Pavel/MathematicalAnalysis/video_output/");*/
}

QRgb MainWindow::calculate_function(int x, int y)
{
    if (x % 2 == 0 || y % 2 == 0) {
        return input_image.pixel(x, y);
    }
    return output_image.pixel(x, y);
}

void clearDir( const QString path )
{
    QDir dir( path );

    dir.setFilter( QDir::NoDotAndDotDot | QDir::Files );
    foreach( QString dirItem, dir.entryList() )
        dir.remove( dirItem );

}

