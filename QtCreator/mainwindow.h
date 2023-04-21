#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MosaiqueMaker; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString input_path;
    QString output_path;
    QString resolution;
    QString methode;


private slots:
    void on_pushButton_2_clicked();

private:
    Ui::MosaiqueMaker *ui;
};
#endif // MAINWINDOW_H
