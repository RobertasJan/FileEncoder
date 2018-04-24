#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include <QDebug>
#include <zipworker.h>
#include <unzipworker.h>
#include <encryptworker.h>
#include <decryptworker.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    fileDialog = new QFileDialog();
    fileDialog->setFileMode(QFileDialog::DirectoryOnly);

    ui->categoryLine->setText(QString("C:/plain"));

    progressBar = new QProgressBar();
    progressBar->setMaximumHeight(20);
    progressBar->setMaximumWidth(120);
    progressBar->setTextVisible(false);
    ui->statusBar->addPermanentWidget(progressBar, 0);
    ui->statusBar->showMessage(QString("Paruošta."));

    ui->pauseButton->setEnabled(false);
    ui->cancelButton->setEnabled(false);

    connect(ui->dirButton, SIGNAL(clicked()), this, SLOT(changeDir()));
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(start()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeDir()
{
    ui->categoryLine->setText(fileDialog->getExistingDirectory());
}

void MainWindow::start()
{
   progressBar->setValue(0);
   ui->listWidget->clear();
   if (ui->encryptRadio->isChecked())
       startEncrypt();
   else
       startDecrypt();
}
void MainWindow::startEncrypt()
{
  //  qDebug() << "encryption";
    dir.setPath(ui->categoryLine->text());
    if (!dir.exists()) {
        QMessageBox msgBox;
        msgBox.setText("Netinkama direktorija.");
        msgBox.exec();
        return;
    }
    QThread *zipThread = new QThread();
    ZipWorker *zipWorker = new ZipWorker(dir);
    zipWorker->moveToThread(zipThread);

    connect(zipThread, SIGNAL (started()), zipWorker, SLOT (process()));
    connect(zipWorker, SIGNAL (finished()), zipThread, SLOT (quit()));
    connect(zipThread, SIGNAL (finished()), this, SLOT(Encrypt()));
    connect(zipWorker, SIGNAL (finished()), zipWorker, SLOT (deleteLater()));
    connect(zipThread, SIGNAL (finished()), zipThread, SLOT (deleteLater()));

    connect(ui->pauseButton, SIGNAL(clicked()), zipWorker, SLOT(setPaused()), Qt::DirectConnection);

    connect(zipWorker, SIGNAL (incrDir()), this, SLOT (incrDirZip()));
    connect(zipWorker, SIGNAL (incrFile()), this, SLOT (incrFileCopy()));
    connect(zipWorker, SIGNAL (message(QString)), this, SLOT (showMessage(QString)));

    progressBar->setMaximum(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size());
    ui->statusBar->showMessage("Archivuojami aplankai...");
    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);

    zipThread->start();
}

void MainWindow::Encrypt()
{
    QString key;
    if (ui->passLine->text().isEmpty())
        key = "random";
    else
        key = ui->passLine->text();

    QThread *encThread = new QThread();
    EncryptWorker *encWorker = new EncryptWorker(key, dir);
    encWorker->moveToThread(encThread);

    connect(encThread, SIGNAL (started()), encWorker, SLOT (process()));
    connect(encWorker, SIGNAL (finished()), encThread, SLOT (quit()));
    connect(encWorker, SIGNAL (finished()), encWorker, SLOT (deleteLater()));
    connect(encThread, SIGNAL (finished()), encThread, SLOT (deleteLater()));
    connect(ui->pauseButton, SIGNAL(clicked()), encWorker, SLOT(setPaused()), Qt::DirectConnection);


    connect(encWorker, SIGNAL (message(QString)), this, SLOT (showMessage(QString)));
    connect(encWorker, SIGNAL (incrEncr()), this, SLOT (incrEncrypt()));

    progressBar->setValue(0);
    QDir tempDir;
    tempDir.setPath(dir.path()+"enc/");
    progressBar->setMaximum(tempDir.entryList(QDir::Files | QDir::NoDotAndDotDot).size());

    ui->statusBar->showMessage("Užkoduojami failai...");
    encThread->start();
   // qDebug() << "maybehere";
}

void MainWindow::startDecrypt()
{
    dir.setPath(ui->categoryLine->text());
    if (!dir.exists()) {
        QMessageBox msgBox;
        msgBox.setText("Netinkama direktorija.");
        msgBox.exec();
        return;
    }
    progressBar->setValue(0);
    progressBar->setMaximum(dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size());

    Decrypt();
}

void MainWindow::Decrypt()
{
    QString key;
    if (ui->passLine->text().isEmpty())
        key = "random";
    else
        key = ui->passLine->text();

    ui->startButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
    ui->cancelButton->setEnabled(true);

    QThread *decThread = new QThread();
    DecryptWorker *decWorker = new DecryptWorker(key, dir);
    decWorker->moveToThread(decThread);

    connect(decThread, SIGNAL (started()), decWorker, SLOT (process()));
    connect(decWorker, SIGNAL (finished()), decThread, SLOT (quit()));
    connect(decWorker, SIGNAL (finished()), decWorker, SLOT (deleteLater()));
    connect(decThread, SIGNAL (finished()), decThread, SLOT (deleteLater()));
    connect(decThread, SIGNAL (finished()), this, SLOT(startUnzip()));

    connect(decWorker, SIGNAL (message(QString)), this, SLOT (showMessage(QString)));
    connect(decWorker, SIGNAL (incrDecr()), this, SLOT (incrDecrypt()));
    connect(ui->pauseButton, SIGNAL(clicked()), decWorker, SLOT(setPaused()), Qt::DirectConnection);




    ui->statusBar->showMessage("Atkoduojami failai...");
    decThread->start();
}

void MainWindow::startUnzip()
{
    QThread *unzipThread = new QThread();
    UnzipWorker *unzipWorker = new UnzipWorker(dir);
    unzipWorker->moveToThread(unzipThread);

    connect(unzipThread, SIGNAL (started()), unzipWorker, SLOT (process()));
    connect(unzipWorker, SIGNAL (finished()), unzipThread, SLOT (quit()));
    connect(unzipWorker, SIGNAL (finished()), unzipWorker, SLOT (deleteLater()));
    connect(unzipThread, SIGNAL (finished()), unzipThread, SLOT (deleteLater()));

    connect(unzipWorker, SIGNAL (incrUnzip()), this, SLOT (incrDirUnzip()));
    connect(unzipWorker, SIGNAL (message(QString)), this, SLOT (showMessage(QString)));
    connect(ui->pauseButton, SIGNAL(clicked()), unzipWorker, SLOT(setPaused()), Qt::DirectConnection);



   // progressBar->setMaximum(dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size());
   // ui->statusBar->showMessage("Archivuojami aplankai...");
   // ui->startButton->setEnabled(false);
   // ui->pauseButton->setEnabled(true);
   // ui->cancelButton->setEnabled(true);

    unzipThread->start();
}

void MainWindow::incrDirZip()
{
  //  qDebug() << "ZIP";
  //  qDebug() << progressBar->value() << progressBar->maximum() << dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size();
    if (progressBar->value()+1 == progressBar->maximum()) {
        ui->statusBar->showMessage("Kopijuojami failai...");
        progressBar->setValue(0);
        progressBar->setMaximum(dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size());
        return;
    }
    progressBar->setValue(progressBar->value()+1);

}

void MainWindow::incrDirUnzip()
{

    //progressBar->setMaximum(tempDir.entryList(QDir::Files | QDir::NoDotAndDotDot).size());
    progressBar->setValue(progressBar->value()+1);
    if (progressBar->value() >= progressBar->maximum()) {
        ui->statusBar->showMessage("Baigta");
        ui->startButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->cancelButton->setEnabled(false);
        return;
    }
  //  progressBar->setValue(progressBar->value()+1);

}

void MainWindow::incrFileCopy()
{
    progressBar->setMaximum(dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size());
  //  qDebug() << "FILE";
//    qDebug() << progressBar->value() << progressBar->maximum() << dir.entryList(QDir::Files | QDir::NoDotAndDotDot).size();
    if (progressBar->value()+1 == progressBar->maximum()) {

        return;
    }
    progressBar->setValue(progressBar->value()+1);

}

void MainWindow::incrEncrypt()
{
  //  qDebug() << "ENCR";
    QDir tempDir;
    tempDir.setPath(dir.path()+"enc/");
    if (progressBar->value()+1 == progressBar->maximum()) {
        ui->statusBar->showMessage("Baigta.");
        ui->startButton->setEnabled(true);
        ui->pauseButton->setEnabled(false);
        ui->cancelButton->setEnabled(false);
    }
    progressBar->setValue(progressBar->value()+1);
  //  qDebug() << progressBar->value() << progressBar->maximum() << tempDir.entryList(QDir::Files | QDir::NoDotAndDotDot).size();

}

void MainWindow::incrDecrypt()
{
   // QDir tempDir;
    //tempDir.setPath(dir.path()+"enc/");
    if (progressBar->value()+1 == progressBar->maximum()) {
        ui->statusBar->showMessage("Išarchyvuojami failai");
        progressBar->setValue(0);
        QDir tempDir;
        tempDir.setPath(dir.path());
        tempDir.setNameFilters(QStringList()<<"*.zip");
        progressBar->setMaximum(tempDir.entryList(QDir::Files | QDir::NoDotAndDotDot).size());
     //   qDebug() << tempDir.path();
//        qDebug() << progressBar->value() << progressBar->maximum() << tempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size();
     //   progressBar->setMaximum(tempDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size());
        return;
    }
    progressBar->setValue(progressBar->value()+1);
}

void MainWindow::showMessage(QString message)
{
    ui->listWidget->addItem(message);
}

void MainWindow::DEBUG()
{
    qDebug() << "PAUSE BUTTON CLICKED";
}
