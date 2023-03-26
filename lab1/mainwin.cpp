#include "mainwin.h"
#include "ui_mainwin.h"
#include <QDebug>

mainwin::mainwin(QWidget *parent) :
    ui(new Ui::mainwin) {

    ui->setupUi(this);
    clipboard = QApplication::clipboard();

    connect(ui->tableWidget, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onTableClicked(const QModelIndex &)));
}

void mainwin::check_pass(QString pass) {
    QByteArray hash = QCryptographicHash::hash(pass.toUtf8(), QCryptographicHash::Sha256);

    hex_hash = hash.toHex();
    qDebug(hex_hash);
    cridentials = CryptoController::decrypt_file("C:/projects/lab1/cridentials.enc", hex_hash);

    if (cridentials!=0){
        createPassTable("");
        emit pageSwap(true);}
    else
        emit pageSwap(false);

}

void mainwin::createPassTable(QString search_line) {
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("name"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("login"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("pass"));

    if (cridentials) {
        for (auto record : *cridentials) {
            if (((QString)record.site).contains(search_line)) {
                ui->tableWidget->insertRow(ui->tableWidget->rowCount());

                QTableWidgetItem *item = new QTableWidgetItem;
                item->setText("" + (QString)record.site + "");
                item->setData(Qt::UserRole, (QString)record.site);
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, item);

                item = new QTableWidgetItem;
                item->setText("******");
                item->setData(Qt::UserRole, "login");
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, item);

                item = new QTableWidgetItem;
                item->setText("******");
                item->setData(Qt::UserRole, "pass");
                item->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 2, item);
            }
        }
    }
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->resizeColumnsToContents();
}

void mainwin::onTableClicked(const QModelIndex &index) {
    if (index.column() != 0) {
        clipboard->setText(CryptoController::decrypt_record(cridentials->toList()[index.row()].encrypted, index.data(Qt::UserRole).toString(), hex_hash));
    } else {
        clipboard->setText(index.data().toString());
        qDebug() << index.data(Qt::UserRole).toString();
    }
}

mainwin::~mainwin() {
    delete ui;
}

void mainwin::on_searchButton_clicked() {
    QString searchText = ui->searchLine->text();
    ui->tableWidget->clear();

    createPassTable(searchText);
}

void mainwin::passListSignal(QString url, QByteArray logpass) {
    this->show();

    QByteArray encrypted_logpass = CryptoController::encrypt_login_password(logpass, hex_hash);
    Cridentials new_record;
    new_record.site = url.toUtf8();
    new_record.encrypted = encrypted_logpass;
    cridentials->append(new_record);

    QJsonDocument doc;
    QJsonArray array;
    for (auto item : *cridentials) {
        QJsonObject record;
        record.insert("name", item.site.data());
        record.insert("logpass", item.encrypted.data());
        array.append(record);
    }
    QJsonObject list;
    list.insert("list", array);
    doc.setObject(list);

    if (CryptoController::encrypt_file(doc.toJson(), "C:/projects/lab1/cridentials.enc", hex_hash)) {
        createPassTable("");
    } else {
        return;
    }

}

