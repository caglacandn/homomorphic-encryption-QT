//ÇALIŞAN KOD ************************
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <QDebug>
#include <QStringList> // QString'i işlemek için ekledik
#include "gmp.h"
#include "gmpxx.h"
#include <QEventLoop>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    eIntx=0;
    nInt =0;
    dInt = 0;

    ui->label_1->setText("Sifrelemek istediginiz sayilari girin:\n(sayilari virgulle ayirin) ");
    ui->label_1->setAlignment(Qt::AlignCenter);

    ui->label_1->setVisible(false);
    ui->label_2->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton_okey->setVisible(false);
    ui->radioButton_sifrele->setVisible(false);
    ui->radioButton_sifreCoz->setVisible(false);

    connect(ui->pushButton_anahtar, SIGNAL(clicked()), this, SLOT(on_pushButton_anahtar_clicked()));
    connect(ui->pushButton_okey, SIGNAL(clicked()), this, SLOT(on_pushButton_okey_clicked()));
    connect(ui->radioButton_sifrele, SIGNAL(clicked()), this, SLOT(on_radioButton_sifrele_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

bool hasCommonFactor(int n, const std::vector<int>& factors) {
    for (int factor : factors) {
        if (n % factor == 0) {
            return true;
        }
    }
    return false;
}

void MainWindow::on_pushButton_anahtar_clicked()
{
    srand(time(nullptr));

    int altSinir = 50;
    int ustSinir = 500;

    std::vector<int> primes;
    for (int i = altSinir; i <= ustSinir; ++i) {
        if (isPrime(i)) {
            primes.push_back(i);
        }
    }

    if (primes.empty()) {
        qDebug() << "Belirtilen aralıkta asal sayı bulunamadı.";
        return;
    }

    int A1 = primes[rand() % primes.size()];
    std::vector<int> remainingPrimes;
    for (int prime : primes) {
        if (prime != A1) {
            remainingPrimes.push_back(prime);
        }
    }

    if (remainingPrimes.empty()) {
        qDebug() << "İkinci asal sayı seçilemedi.";
        return;
    }

    int A2 = remainingPrimes[rand() % remainingPrimes.size()];
    mpz_t d, e;
    mpz_inits(d, e, NULL);

    //mpz_set_ui(n, A1 * A2);
    nInt = A1 * A2;
    int u_n = (A1 - 1) * (A2 - 1);

    std::vector<int> carpanlar;
    for (int i = 2; i <= u_n; ++i) {
        if (u_n % i == 0 && isPrime(i)) {
            carpanlar.push_back(i);
        }
    }

    int eInt;
    while (true) {
        eInt = rand() % (u_n - 1) + 1;
        if (!hasCommonFactor(eInt, carpanlar)) {
            break;
        }
    }
    eIntx = eInt;

    int k = 1;
    while ((k * u_n + 1) % eInt != 0) {
        k++;
    }

    mpz_set_ui(e, static_cast<unsigned long>(eInt));
    mpz_set_ui(d, static_cast<unsigned long>((k * u_n + 1) / eInt));

    dInt = mpz_get_si(d); // mpz_t'yi int'e dönüştür

    ui->pushButton_anahtar->setVisible(false);
    ui->label_1->setVisible(true);
    ui->lineEdit->setVisible(true);
    ui->label_2->setVisible(true);
    ui->pushButton_okey->setVisible(true);
    ui->radioButton_sifrele->setVisible(true);
    ui->radioButton_sifreCoz->setVisible(true);

}

void MainWindow::on_radioButton_sifrele_clicked()
{

}

void MainWindow::on_radioButton_sifreCoz_clicked()
{

}

void MainWindow::on_pushButton_okey_clicked()
{

    QString inputText = ui->lineEdit->text();
    QStringList inputList = inputText.split(',');
    std::vector<int> integerList; // Tamsayıları saklayacak vektör

    for (const QString& str : inputList) {
        bool ok;
        int num = str.toInt(&ok);
        if (ok) {
            integerList.push_back(num);
        } else {
            // Dönüşüm başarısız oldu, hata işlemleri burada yapılabilir
        }
    }

    if (ui->radioButton_sifrele->isChecked()) {
        qDebug() << "Sifreleme islemi yapiliyor.";

        std::vector<int> c(integerList.size()); // integerList'e göre boyutlandırıldı

        mpz_t m_i, c_i, e;
        mpz_inits(m_i, c_i, e, NULL);

        mpz_t n;
        mpz_init(n);
        mpz_set_ui(n, static_cast<unsigned long>(nInt));


        for (size_t i = 0; i < integerList.size(); ++i) {

            mpz_set_ui(m_i, static_cast<unsigned long>(integerList[i]));

            mpz_t result;
            mpz_init(result);

            mpz_pow_ui(result, m_i, eIntx);

            mpz_mod(c_i, result, n); // c_i = (m_i^e) % n

            c[i] = mpz_get_ui(c_i);
            mpz_clear(result);

        }

        // mpz_t değişkenlerini temizle
        mpz_clears(m_i, c_i, e, n, NULL);

        QStringList encryptedList;
        for (size_t i = 0; i < c.size(); ++i) {
            encryptedList << QString::number(c[i]);
        }

        QString encryptedText = encryptedList.join(", ");
        ui->label_2->setText("Sifrelenmis Metin: " + encryptedText);
    }

    else if (ui->radioButton_sifreCoz->isChecked()) {
        qDebug() << "Cozme islemi yapiliyor.";

        std::vector<int> m(integerList.size()); // integerList'e göre boyutlandırıldı

        mpz_t m_i, c_i, d;
        mpz_inits(m_i, c_i, d, NULL);

        mpz_t n;
        mpz_init(n);
        mpz_set_ui(n, static_cast<unsigned long>(nInt));


        for (size_t i = 0; i < integerList.size(); ++i) {


            mpz_set_ui(c_i, static_cast<unsigned long>(integerList[i]));

            mpz_t result;
            mpz_init(result);

            mpz_pow_ui(result, c_i, dInt);

            mpz_mod(m_i, result, n); // m_i = (c_i^d) % n

            m[i] = mpz_get_ui(m_i);
            mpz_clear(result);

        }

        // mpz_t değişkenlerini temizle
        mpz_clears(m_i, c_i, d, NULL);

        QStringList messageList;
        for (size_t i = 0; i < m.size(); ++i) {
            messageList << QString::number(m[i]);
        }

        QString messageText = messageList.join(", ");
        ui->label_2->setText("Sifrelenmis Metin: " + messageText);

    }
    else {
        ui->label_2->setText("Yapilacak islemi seciniz." );
        qDebug() << "Seciniz";
    }
}
