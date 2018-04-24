#include "unzipworker.h"
#include <QFile>
#include <QDebug>
#include <JlCompress.h>

UnzipWorker::UnzipWorker(QDir dir, QObject *parent) : QObject(parent)
{
    this->dir = dir;

}

void UnzipWorker::process()
{
    QDir tempDir;
    tempDir.setPath(dir.path());
    tempDir.setNameFilters(QStringList()<<"*.zip");
    QStringList fileList = tempDir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    qDebug() << fileList << fileList.size();

    mutex.lock();
        if (paused)
        {
           waitCondition.wait(&mutex);
        }
    mutex.unlock();

    for (auto &filesIt : fileList) {
        //if (is okay)

        JlCompress::extractDir(tempDir.path()+"/"+filesIt, tempDir.path()+"/"+filesIt.left(filesIt.length()-4));
        QFile file(tempDir.path()+"/"+filesIt);
        file.remove();
   //    QFile::copy(dir.path()+"/"+filesIt, dir.path()+"enc/"+filesIt);
        emit incrUnzip();
        QString message2 = "Failas "+filesIt+" išarchyvuotas";
        emit message(message2);
        //qDebug() << "finishedcopying";
    }
    emit finished();
}

void UnzipWorker::setPaused(){
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

