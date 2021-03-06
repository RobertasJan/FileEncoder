#-------------------------------------------------
#
# Project created by QtCreator 2018-04-09T15:42:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FileEncoder
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    zipworker.cpp \
    encryptworker.cpp \
    qaesencryption.cpp \
    decryptworker.cpp \
    unzipworker.cpp

HEADERS += \
        mainwindow.h \
    zipworker.h \
    encryptworker.h \
    qaesencryption.h \
    decryptworker.h \
    unzipworker.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += C:/Qt/zlib-1.2.11
LIBS += -LC:/Qt/zlib-1.2.11 -lz
INCLUDEPATH += C:/Qt/quazip-0.7/quazip
LIBS += -LC:/Qt/quazip-0.7/quazip/release -lquazip
INCLUDEPATH += C:/OpenSSL-Win32/include


