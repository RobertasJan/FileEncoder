#ifndef DECRYPTWORKER_H
#define DECRYPTWORKER_H

#include <QObject>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>

class DecryptWorker : public QObject
{
    Q_OBJECT
public:
    explicit DecryptWorker(QString, QDir, QObject *parent = nullptr);


signals:
    void finished();
    void incrDecr();
    void message(QString message);

public slots:
    void process();
    void md5hashfile();
    void setPaused();
private:
    QString key;
    QDir dir;
    QDir dirEnc;
    QMutex mutex;
    QWaitCondition waitCondition;
    bool paused=false;
    QVector<QByteArray> getHashes();
};

#endif // DECRYPTWORKER_H
