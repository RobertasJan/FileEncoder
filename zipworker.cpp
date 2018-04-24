#include "zipworker.h"
#include <QFile>
#include <QDebug>
#include <JlCompress.h>
#include <QThread>


ZipWorker::ZipWorker(QDir dir, QObject *parent) : QObject(parent)
{
    this->dir = dir;
    QDir tempDir;
    tempDir.setPath(dir.path()+"enc/");
    if (!tempDir.exists()) {
        QDir().mkdir(tempDir.path());
    }
}

void ZipWorker::process()
{
    QStringList dirList, fileList;
    dirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (auto &dirIt : dirList) {
        mutex.lock();
        if (paused)
        {
           waitCondition.wait(&mutex);
        }
        mutex.unlock();

        JlCompress::compressDir(dir.path()+"enc/"+dirIt+".zip", dir.path()+"/"+dirIt);
        emit incrDir();
        QString message2 = "Aplankas "+dirIt+" suarchyvuotas į "+dir.path()+"enc/"+dirIt+".zip";
        emit message(message2);
    }
    fileList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (auto &filesIt : fileList) {
        mutex.lock();
        if (paused)
        {
           waitCondition.wait(&mutex);
        }
        mutex.unlock();
        QFile::copy(dir.path()+"/"+filesIt, dir.path()+"enc/"+filesIt);
        emit incrFile();
        QString message2 = "Failas "+filesIt+" nukopijuotas į "+dir.path()+"enc/"+filesIt;
        emit message(message2);
        //qDebug() << "finishedcopying";
    }
    emit finished();
}

void ZipWorker::setPaused(){
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
