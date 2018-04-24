#include "encryptworker.h"
#include <QCryptographicHash>
#include <qaesencryption.h>
#include <QDebug>

EncryptWorker::EncryptWorker(QString key, QDir dir, QObject *parent) : QObject(parent)
{
    this->key = key;
    this->dir = dir;
    this->dirEnc.setPath(dir.path()+"enc");
}

void EncryptWorker::process()
{
    QFile md5file("md5hash"+dirEnc.dirName()+".txt");
    md5file.open(QIODevice::WriteOnly);
    QVector<QString> hashVector;
    QStringList fileList = dirEnc.entryList(QDir::Files | QDir::NoDotAndDotDot);
   // qDebug () << fileList.size();
    for (auto filesIt : fileList) {
        mutex.lock();
        if (paused)
        {
           waitCondition.wait(&mutex);
        }
        mutex.unlock();
        QFile file(dirEnc.path()+"/"+filesIt);
        file.open(QIODevice::ReadOnly);
        QByteArray inputByte = file.readAll();


        // qDebug() << file.readAll();
        file.close();
        if (filesIt == "textfile.txt")
            qDebug() << inputByte;
        QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
        QString iv("ivvector");

        QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
        QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

        QByteArray encodeText = encryption.encode(inputByte, hashKey, hashIV);

      //  QByteArray decodeText = encryption.decode(encodeText, hashKey, hashIV);
       // qDebug() << encodeText;
        QByteArray data = encodeText;
        QByteArray hashResult, hashResult2;
        QCryptographicHash hash(QCryptographicHash::Md5);
        hash.addData(data);
        hashResult = hash.result();

        qDebug() << hashResult;
        md5file.write(hashResult.toHex());
        QFile wfile(dirEnc.path()+"/"+filesIt);
        wfile.open(QIODevice::WriteOnly);
        wfile.write(encodeText);
        wfile.close();

        QString message2 = "Failas "+filesIt+" užkoduotas.";
        emit incrEncr();
        emit message(message2);
    }
    md5file.close();
    md5hashfile();

    emit finished();
}

void EncryptWorker::md5hashfile()
{
    QFile file("md5hash"+dirEnc.dirName()+".txt");
    file.open(QIODevice::ReadOnly);
    QByteArray inputByte = file.readAll();
   // qDebug() << file.fileName();
    file.close();
    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);
    QString iv("ivvector");

    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray encodeText = encryption.encode(inputByte, hashKey, hashIV);
  //  qDebug() << encryption.removePadding(encodeText);
    QFile wfile("md5hash"+dirEnc.dirName()+".txt");
    wfile.open(QIODevice::WriteOnly);
    wfile.write(encodeText);
    wfile.close();
}
void EncryptWorker::setPaused()
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

