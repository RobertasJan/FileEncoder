#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QProgressBar>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    QFileDialog *fileDialog;
    QProgressBar *progressBar;
    QDir dir;

private slots:
    void changeDir();
    void start();
    void startEncrypt();
    void Encrypt();
    void startDecrypt();
    void Decrypt();
    void startUnzip();
    void incrDirZip();
    void incrDirUnzip();
    void incrFileCopy();
    void incrEncrypt();
    void incrDecrypt();
    void showMessage(QString);
    void DEBUG();
};

#endif // MAINWINDOW_H
