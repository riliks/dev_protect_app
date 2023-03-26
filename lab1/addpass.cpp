#include "addpass.h"
#include "ui_addpass.h"
#include "QFile"
#include "QJsonObject"
#include "QJsonDocument"
#include <QJsonArray>
addpass::addpass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addpass)
{
    ui->setupUi(this);
}

addpass::~addpass()
{
    delete ui;
}

void addpass::on_pushButton_clicked()
{
    ui->lineEdit_name->setText("");
    ui->lineEdit_pass->setText("");
    ui->lineEdit_login->setText("");
}


void addpass::on_pushButton_2_clicked()
{
    QString name=ui->lineEdit_name->text();
    QString passui=ui->lineEdit_pass->text();
    QString login=ui->lineEdit_login->text();

    QFile file;
    file.setFileName("C:/projects/lab1/test.json");
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    QString text = file.readAll();
    QJsonDocument d = QJsonDocument::fromJson(text.toUtf8());
    QJsonArray array=d.object().value("list").toArray();
    QJsonObject final;

    QJsonObject obj;
    obj.insert("name", name);
    obj.insert("login", passui);
    obj.insert("pass", login);


    array.append(obj);
    final.insert("list", array);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(final);
    file.write(jsonDoc.toJson());
    file.close();
    this->close();
    /*ui
    QJsonDocument d = QJsonDocument::fromJson(text.toUtf8());
    createListElem(d.object().value("list").toArray()[d.object().value("list").toArray().size()].toObject())*/;
    //d.object().value("list").toArray().size()
}

