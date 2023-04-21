#include "mainwindow.h"
#include "./ui_mainwindow.h"

//#include "start.h"
#include "start.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MosaiqueMaker)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    // enregistrer chemins, reoslution, et methode
    input_path = ui->lineEdit_5->displayText();
    output_path = ui->lineEdit_4->displayText();
    resolution = ui->comboBox_4->currentText();
    methode = ui->comboBox_3->currentText();

    start s(this);
    s.letsgo();
}

