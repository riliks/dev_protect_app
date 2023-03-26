#include "cryptocontroller.h"
#include <QDebug>
#include <openssl\evp.h>
#include <QBuffer>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

CryptoController::CryptoController(QObject *parent)
    : QObject{parent}
{

}

QList<Cridentials> * CryptoController::decrypt_file(const QString &filename, const QByteArray &key) {
    QFile encrypted_file;
    encrypted_file.setFileName(filename);
    encrypted_file.open(QIODevice::ReadOnly);
    if (!encrypted_file.isOpen()) {
        return nullptr;
    }
    QByteArray decoded_buf = QByteArray::fromBase64(encrypted_file.readAll());
    QBuffer buffer_stream(&decoded_buf);
    buffer_stream.open(QBuffer::ReadOnly);

    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return nullptr;
    }

    // Инициализируем
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), CryptoController::m_iv)){
        return 0;
    }

    int read_len = 0, decrypted_len = 0;
    QByteArray buffer;
    while (true) {
        char encrypted_buf[256] = {0};
        unsigned char decrypted_buf[512] = {0};
        read_len = buffer_stream.read(encrypted_buf, 256); // считать 256 байт

        // Расшифруем
        if (1 != EVP_DecryptUpdate(ctx,
                                   decrypted_buf, // выходной параметр, буфер куда записывается зашифрованный текст
                                   &decrypted_len, // выходной параметр, кол-во зашифрованных символов
                                   (unsigned char *)encrypted_buf, // входной параметр, шифруемый буфер
                                   read_len)) {    // входной параметр, кол-во исходных символов
            return 0;
        }

        if (read_len < 256) {
            int tmplen;
            if (!EVP_DecryptFinal_ex(ctx, decrypted_buf + decrypted_len, &tmplen)) {
                /* Error */
                EVP_CIPHER_CTX_free(ctx);
                return 0;
            }
            decrypted_len += tmplen;
            buffer += QByteArray((char *)decrypted_buf, decrypted_len);
            break;
        } else {
            buffer += QByteArray((char *)decrypted_buf, decrypted_len);
        }

    }
    // Конец работы с openssl
    EVP_CIPHER_CTX_free(ctx);

    encrypted_file.close();
    buffer_stream.close();

    QJsonDocument json_doc = QJsonDocument::fromJson(buffer);
    QJsonArray cridentials_array = json_doc.object().value("list").toArray();

    QList<Cridentials> * result = new QList<Cridentials>();
    for (auto item : cridentials_array) {
        Cridentials new_record;
        new_record.site = item.toObject().value("name").toString().toUtf8();
        qDebug(new_record.site);
        new_record.encrypted = item.toObject().value("logpass").toString().toUtf8();
        qDebug(new_record.encrypted);
        result->append(new_record);
    }
    QByteArray hash= key;
    if (hash!=json_doc.object().value("hash").toString())
        result=0;
    return result;
}

QString CryptoController::decrypt_record(const QByteArray &record, QString index, const QByteArray &key) {
    QString result;

    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return nullptr;
    }

    // Инициализируем
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), CryptoController::m_iv)){
        return 0;
    }

    QByteArray ciphertext = QByteArray::fromBase64(record);
    QBuffer buffer(&ciphertext);
    buffer.open(QBuffer::ReadOnly);

    char encrypted_buf[256] = {0};
    unsigned char decrypted_buf[256] = {0};
    int decrypted_len = 0;
    int read_len = buffer.read(encrypted_buf, 256);

    // Расшифруем
    if (1 != EVP_DecryptUpdate(ctx,
                               decrypted_buf, // выходной параметр, буфер куда записывается зашифрованный текст
                               &decrypted_len, // выходной параметр, кол-во зашифрованных символов
                               (unsigned char *) encrypted_buf, // входной параметр, шифруемый буфер
                               read_len)) {    // входной параметр, кол-во исходных символов
        return 0;
    }

    int tmplen;
    if (!EVP_DecryptFinal_ex(ctx, decrypted_buf + decrypted_len, &tmplen)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    decrypted_len += tmplen;
    buffer.close();

    QJsonDocument json_doc = QJsonDocument::fromJson(QByteArray((char *)decrypted_buf, decrypted_len));
    result = json_doc.object().value(index).toString();

    EVP_CIPHER_CTX_free(ctx);

    return result;
}

QByteArray CryptoController::encrypt_login_password(QByteArray logpass, const QByteArray &key) {
    QByteArray result;

    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return 0;
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), CryptoController::m_iv)){
        return 0;
    }

    QByteArray plaintext = QByteArray(logpass);
    QBuffer buffer(&plaintext);
    buffer.open(QBuffer::ReadOnly);

    unsigned char encrypted_buf[256] = {0};
    char decrypted_buf[256] = {0};
    int encrypted_len = 0;
    int read_len = buffer.read(decrypted_buf, 256);

    if (1 != EVP_EncryptUpdate(ctx,
                               encrypted_buf, // выходной параметр, буфер
                               &encrypted_len, // выходной параметр, кол-во символов
                               (unsigned char *) decrypted_buf, // входной параметр, буфер
                               read_len)) {    // входной параметр, кол-во  символов
        return 0;
    }

    int tmplen;
    if (!EVP_EncryptFinal_ex(ctx, encrypted_buf + encrypted_len, &tmplen)) {
        /* Error */
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    encrypted_len += tmplen;
    buffer.close();

    result = QByteArray((char *)encrypted_buf, encrypted_len).toBase64();

    EVP_CIPHER_CTX_free(ctx);

    return result;
}


bool CryptoController::encrypt_file(const QByteArray plaintext, const QString &filename, const QByteArray &key) {
    EVP_CIPHER_CTX *ctx;
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return false;
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char*)key.data(), CryptoController::m_iv)){
        return false;
    }

    QByteArray not_yet_encoded = QByteArray(plaintext);
    QBuffer buffer_stream(&not_yet_encoded);
    buffer_stream.open(QBuffer::ReadOnly);

    QByteArray buffer;
    int read_len = 0, encrypted_len = 0;
    while (true) {
        unsigned char encrypted_buf[512] = {0};
        char decrypted_buf[256] = {0};
        read_len = buffer_stream.read(decrypted_buf, 256); // считать 256 байт

        if (1 != EVP_EncryptUpdate(ctx,
                                   encrypted_buf, // выходной параметр, буфер
                                   &encrypted_len, // выходной параметр, кол-во символов
                                   (unsigned char *) decrypted_buf, // входной параметр, буфер
                                   read_len)) {    // входной параметр, кол-во  символов
            return 0;
        }

        if (read_len < 256) {
            int tmplen;
            if (!EVP_EncryptFinal_ex(ctx, encrypted_buf + encrypted_len, &tmplen)) {
                /* Error */
                EVP_CIPHER_CTX_free(ctx);
                return false;
            }
            encrypted_len += tmplen;
            buffer += QByteArray((char *)encrypted_buf, encrypted_len);
            break;
        } else {
            buffer += QByteArray((char *)encrypted_buf, encrypted_len);
        }

    }
    EVP_CIPHER_CTX_free(ctx);
    buffer_stream.close();

    QFile encrypted_file;
    encrypted_file.setFileName(filename);
    encrypted_file.open(QIODevice::WriteOnly);
    if (!encrypted_file.isOpen()) {
        return false;
    }
    encrypted_file.write(buffer.toBase64());
    encrypted_file.close();

    return true;
}
