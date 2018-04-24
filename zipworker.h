#ifndef ZIPWORKER_H
#define ZIPWORKER_H

#include <QObject>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>

class ZipWorker : public QObject
{
    Q_OBJECT
public:
    explicit ZipWorker(QDir dir, QObject *parent = nullptr);
private:
    QDir dir;
    QMutex mutex;
    QWaitCondition waitCondition;
    bool paused=false;
signals:
    void finished();
    void incrDir();
    void incrFile();
    void message(QString message);
public slots:
    void process();
    void setPaused();

};

#endif // ZIPWORKER_H
