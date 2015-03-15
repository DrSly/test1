#-------------------------------------------------
#
# Project created by QtCreator 2015-01-05T20:41:20
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2_clienttest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow2.cpp \
    tcpclient.cpp

HEADERS  += mainwindow2.h \
    tcpclient.h

FORMS    += mainwindow2.ui
