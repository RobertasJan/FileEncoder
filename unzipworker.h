#ifndef UNZIPWORKER_H
#define UNZIPWORKER_H

#include <QObject>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>

class UnzipWorker : public QObject
{
    Q_OBJECT
public:
    explicit UnzipWorker(QDir dir, QObject *parent = nullptr);

signals:
    void finished();
    void message(QString);
    void incrUnzip();
public slots:
    void process();
    void setPaused();
private:
    QDir dir;
    QMutex mutex;
    QWaitCondition waitCondition;
    bool paused=false;
};

#endif // UNZIPWORKER_H
