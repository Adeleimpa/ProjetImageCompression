#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "main.cpp"

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

    start();

}

