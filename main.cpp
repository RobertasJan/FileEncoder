#include "mainwindow.h"
#include <QApplication>
#include <qaesencryption.h>
#include <QCryptographicHash>
#include <QDebug>
#include <JlCompress.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //JlCompress::compressDir("C:/plainenc/zvejyba3.zip", "C:/plain/zvejyba3");

    /*-QFile file("C:/test/testfile.txt");
    QFile fileout("C:/testenc/testfile.txt");
    file.open(QIODevice::ReadOnly);
    fileout.open(QIODevice::WriteOnly);

    const QByteArray inputByte = file.readAll();

    QString inputString(inputByte);
    qDebug() << inputByte;

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QString key("hello");
    QString iv("ivvector");

    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray encodeText = encryption.encode(inputByte, hashKey, hashIV);
    QByteArray decodeText = encryption.decode(encodeText, hashKey, hashIV);

    fileout.write(encryption.removePadding(encodeText));
    fileout.close();
    file.close();*/
  /*  QFile file("C:/plain/testfile.txt");
    file.open(QIODevice::ReadOnly);
//
   // qDebug() << file.readAll();
    QByteArray inputByte = file.readAll();
    file.close();
    qDebug() << inputByte;
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QString iv("ivvector");
    QString otherkey("keylenght");

    QByteArray hashKey = QCryptographicHash::hash(otherkey.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray encodeText = encryption.encode(inputByte, hashKey, hashIV);
  //  QByteArray decodeText = encryption.decode(encodeText, hashKey, hashIV);
   // qDebug() << encodeText;
    QFile wfile("C:/plainenc/testfile.txt");
    wfile.open(QIODevice::WriteOnly);
    wfile.write(encryption.removePadding(encodeText));
    wfile.close();
/*
 //   QString message2 = "Failas "+filesIt+" užkoduotas.";
 //   emit incrEncr();
  //  emit message(message2)*/
 //   QString string="hello.zip";
 //   int pos = string.lastIndexOf(QChar('.zip'));
//    qDebug() << string.left(string.length()-4);
    w.show();



    return a.exec();
}
