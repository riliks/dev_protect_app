#include "mainwindow.h"
#include <windows.h>
#include <QApplication>

int main(int argc, char *argv[])
{

    // Смещение в виртуальной памяти
    unsigned long long moduleBase = (unsigned long long)GetModuleHandle(NULL);
    // Смещение сешмента .text
    unsigned long long text_segment_start = moduleBase + 0x1000;

    // Определение расзмера сегмента .text
    PIMAGE_DOS_HEADER pIDH = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
    PIMAGE_NT_HEADERS pINH = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + pIDH->e_lfanew);
    unsigned long long size_of_text = pINH->OptionalHeader.SizeOfCode;

    // Считается хэш в диапазоне от start до start+size
    QByteArray text_segment_contents = QByteArray((char *)text_segment_start, size_of_text);
    QByteArray current_hash = QCryptographicHash::hash(text_segment_contents, QCryptographicHash::Sha256);
    QByteArray current_hash_base64 = current_hash.toBase64();

    // Сравнение
    const QByteArray hash0_base64 = QByteArray("jslPn/zXM8nuVcI8ENs+cp3xEnqsw4VO9Mq3RYXHb20=");

    qDebug() << "current_hash = " << current_hash_base64;

    bool checkresult = (current_hash_base64 == hash0_base64);

    QApplication a(argc, argv);
    MainWindow w = new MainWindow(!checkresult);
    w.show();
    return a.exec();
}
