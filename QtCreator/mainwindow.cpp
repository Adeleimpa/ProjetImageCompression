#include "mainwindow.h"
#include "./ui_mainwindow.h"

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


void MainWindow::on_pushButton_clicked()
{
    // enregistrer chemins, reoslution, et methode
    input_path = ui->lineEdit_3->displayText();
    output_path = ui->lineEdit_2->displayText();
    resolution = ui->comboBox_2->currentText();
    methode = ui->comboBox->currentText();

    //start();

}

