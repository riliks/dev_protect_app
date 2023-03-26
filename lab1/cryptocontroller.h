#ifndef CRYPTOCONTROLLER_H
#define CRYPTOCONTROLLER_H

#include <QObject>
#include <QFile>

struct Cridentials {
public:
    QByteArray site;
    QByteArray encrypted;
};

class CryptoController : public QObject {
    Q_OBJECT
public:
    explicit CryptoController(QObject *parent = nullptr);

    static QList<Cridentials> * decrypt_file(const QString &filename, const QByteArray &key);
    static QString decrypt_record(const QByteArray &record, QString index, const QByteArray &key);
    static QByteArray encrypt_login_password(QByteArray logpass, const QByteArray &key);
    static bool encrypt_file(const QByteArray plaintext, const QString &filename, const QByteArray &key);

    constexpr static unsigned char m_iv[17] = "A6eA4YB9DAHgEDCO";
};

#endif // CRYPTOCONTROLLER_H
