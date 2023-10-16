#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <gmp.h>
#include <gmpxx.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE





class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_anahtar_clicked();

    void on_radioButton_sifrele_clicked();

    void on_radioButton_sifreCoz_clicked();

    void on_pushButton_okey_clicked();








private:
    Ui::MainWindow *ui;
    //int e, d, n;
    mpz_t n;
    mpz_t e;
    mpz_t d;
    int nInt;
    int eInt;
    int eIntx;
    int dInt;
};
#endif // MAINWINDOW_H
