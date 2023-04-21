#include "mainwindow.h"

#include <QApplication>

int resolution;
const int nb_img_db = 10001; //nombre d'images utilisées de la base de données, max 10001


int main(int argc, char* argv[]){

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}


