#ifndef ENCRYPTWORKER_H
#define ENCRYPTWORKER_H

#include <QObject>
#include <QDir>
#include <QCryptographicHash>
#include <QMutex>
#include <QWaitCondition>


class EncryptWorker : public QObject
{
    Q_OBJECT
public:
    explicit EncryptWorker(QString, QDir, QObject *parent = nullptr);

signals:
    void finished();
    void incrEncr();
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
};

#endif // ENCRYPTWORKER_H
