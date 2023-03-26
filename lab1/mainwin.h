#ifndef mainwin_H
#define mainwin_H

#include <QMainWindow>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QToolButton>
#include <QLineEdit>
#include <QClipboard>
#include <QTableWidgetItem>
#include <QTableWidget>
#include "cryptocontroller.h"
#include <QCryptographicHash>
#include <QList>
#include <QDialog>
QT_BEGIN_NAMESPACE
namespace Ui { class mainwin; }
QT_END_NAMESPACE

class mainwin : public QDialog
{
    Q_OBJECT

public:
    explicit mainwin(QWidget *parent = nullptr);
    ~mainwin();

private:
    Ui::mainwin *ui;
    QClipboard *clipboard;
    void createPassTable(QString text);
    QList<Cridentials> * cridentials = nullptr;
    QByteArray hex_hash;

public slots:
    void onTableClicked(const QModelIndex &index);
    void on_searchButton_clicked();
    void check_pass(QString pass);
    void passListSignal(QString url, QByteArray logpass);

signals:
    void pageSwap(bool isCorrect);
};

#endif // mainwin_H
