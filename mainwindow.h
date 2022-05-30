#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextStream>
#include <QFileDialog>
#include <QRgb>
#include <math.h>
#include "neuronet.h"
#include "neuronet_m.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void generateImage(Neuronet&, int, int, int&);
    void generateImage(Neuronet_M&, int, int, int&);

private slots:
    void on_choose_img_in_clicked();

    void on_choose_img_out_clicked();

    void on_start_clicked();

private:
    QRgb calculate_function(int x, int y);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
