#ifndef ADDPASS_H
#define ADDPASS_H

#include <QDialog>
#include "mainwin.h"
namespace Ui {
class addpass;
}

class addpass : public QDialog
{
    Q_OBJECT

public:
    explicit addpass(QWidget *parent = nullptr);
    ~addpass();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::addpass *ui;
};

#endif // ADDPASS_H
