#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainwin.h"
#include <QCryptographicHash>

MainWindow::MainWindow(bool checkresult,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (checkresult) {

        } else {
            qDebug() << "Неверный hash!";
            ui->label->setText("Неверный hash!");
            ui->pushButton->setVisible(false);
        }

//    m_antidebug_timer.setInterval(1000);
//    connect(&m_antidebug_timer, &QTimer::timeout, this, &MainWindow::antidebug_timer);
//    m_antidebug_timer.start();
    ui->debuggerPresentButton->setVisible(false);
    mainw = new mainwin;
    connect(this, SIGNAL(check_pass(QString)), mainw, SLOT(check_pass(QString)));
    connect(mainw, SIGNAL(pageSwap(bool)), this, SLOT(pageSwap(bool)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::pageSwap(bool isCorrect) {
    if (isCorrect) {
        this->close();
        mainw->show();
    } else {
        ui->label->setText("Пин-код не верный");
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString hash = ui->lineEdit->text();
    emit check_pass(hash);
}

void MainWindow::antidebug_timer() {
    if (IsDebuggerPresent()) {
        ui->debuggerPresentButton->setVisible(true);
    }
}

