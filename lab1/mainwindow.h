#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <windows.h>
#include <winbase.h>
#include <QMainWindow>
#include <QString>
#include "mainwin.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(bool checkresult,QWidget  *parent = nullptr);
    ~MainWindow();
    mainwin *mainw;

signals:
    void check_pass(QString pass);

private slots:
    void on_pushButton_clicked();
    void pageSwap(bool isCorrect);

private:
    Ui::MainWindow *ui;
    QTimer m_antidebug_timer;
    void antidebug_timer();
};
#endif // MAINWINDOW_H
