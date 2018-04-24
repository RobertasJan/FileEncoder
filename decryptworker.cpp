#include "decryptworker.h"
#include <QCryptographicHash>
#include <qaesencryption.h>
#include <QDebug>
#include <QMessageBox>

DecryptWorker::DecryptWorker(QString key, QDir dir, QObject *parent) : QObject(parent)
{
    this->key = key;
    this->dir = dir;
}

void DecryptWorker::process()
{
    md5hashfile();
    QVector<QByteArray> vector = getHashes();

    QStringList fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
   // qDebug () << fileList.size();
    int i=0;
    for (auto filesIt : fileList) {
        mutex.lock();
        if (paused)
        {
           waitCondition.wait(&mutex);
        }
        mutex.unlock();
        QFile file(dir.path()+"/"+filesIt);
        file.open(QIODevice::ReadOnly);

       // qDebug() << file.readAll();
        QByteArray inputByte = file.readAll();

        file.close();
       /*if (filesIt == "textfile.txt")
            qDebug() << inputByte;*/
        QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
        QString iv("ivvector");

        QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
        QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);


     //   QByteArray encodeText = encryption.encode(inputByte, hashKey, hashIV);

        QByteArray data = inputByte;
        QByteArray hashResult;
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(inputByte);
        hashResult = hash.result();
        if (hashResult.toHex()!=vector[i]) {
            QString message2 = "Failo "+filesIt+" md5 hash reiksme nesutapo";
            emit message(message2);
            i++;
            continue;
        }
       // qDebug() << hashResult.toHex();
       // qDebug() << vector[0] << filesIt;
        QByteArray decodeText = encryption.decode(inputByte, hashKey, hashIV);
       // qDebug() << encodeText;
        QFile wfile(dir.path()+"/"+filesIt);
        wfile.open(QIODevice::WriteOnly);
        wfile.write(decodeText);
        wfile.close();

        QString message2 = "Failas "+filesIt+" atkoduotas.";
        emit incrDecr();
        emit message(message2);
        i++;
    }

    emit finished();
}

void DecryptWorker::md5hashfile()
{
        QFile file("md5hash"+dir.dirName()+".txt");
        file.open(QIODevice::ReadOnly);
        QByteArray inputByte = file.readAll();

        file.close();

        QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
        QString iv("ivvector");

        QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
        QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);
        QByteArray decodeText = encryption.decode(inputByte, hashKey, hashIV);
    //    qDebug() << decodeText;
        QFile wfile("md5hash"+dir.dirName()+".txt");
        wfile.open(QIODevice::WriteOnly | QIODevice::Text);
        wfile.write(decodeText);
        wfile.close();
}
void DecryptWorker::setPaused()
{
   qDebug() << paused;
   mutex.lock();
   if (paused)
       paused=false;
   else
       paused=true;
   qDebug() << paused;
   if (!paused)
      waitCondition.wakeAll();
   mutex.unlock();
}

QVector<QByteArray> DecryptWorker::getHashes()
{
     QFile file("md5hash"+dir.dirName()+".txt");
     file.open(QIODevice::ReadOnly);
     QByteArray inputByte = file.readAll();
     QVector<QByteArray> vector;
     file.close();
     for (int i=0; i<file.size(); i+=32) {
         vector.push_back(inputByte.mid(i, 32));
     }
     qDebug() << vector;
     return vector;
}
