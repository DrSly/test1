#-------------------------------------------------
#
# Project created by QtCreator 2014-12-23T14:40:56
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lanpeermgr.cpp \
    random.cpp \
    tcpserver.cpp \
    tcpclient.cpp

HEADERS  += mainwindow.h \
    lanpeermgr.h \
    random.h \
    globals.h \
    tcpserver.h \
    tcpclient.h

FORMS    += mainwindow.ui
