//#include <QtNetwork>
#include "globals.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lanpeermgr.h"
#include "tcpserver.h"
#include "tcpclient.h"



#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    peerMgr = new LanPeerMgr(this, DEFAULT_PORT, 8);
    svr = new TcpServer(this);
    cli = new TcpClient(this);

    // instance of network peer manager
    //peerMgr.BeginBroadcast();

    // connect SIGNALS to SLOTS
    connect(ui->btnNew, SIGNAL(clicked()), this, SLOT(startBroadcasting()));
    //connect(ui->btnNew, SIGNAL(clicked()), peerMgr, SLOT(BeginBroadcast()));
    //connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnJoin, SIGNAL(clicked()), this, SLOT(btnJoin_clicked()));
    // // // connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
    connect(peerMgr, SIGNAL(peerfound(QString)), this, SLOT(testSlotforUserSignal(QString)));
    connect(peerMgr, SIGNAL(messageReceived(QString)), this, SLOT(testSlotforUserSignal(QString)));
    //connect(peerMgr, SIGNAL(peerAdded(QHostAddress)), this, SLOT(newPeerFound(QHostAddress)));
    connect(peerMgr, SIGNAL(peerAdded(QString)), this, SLOT(newPeerFound(QString)));
    /* connect(peerMgr, SIGNAL(peerfound2()), this, SLOT(testSlotforUserSignal2())); */
    // udp receive
    // // // connect(udpReceiveSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
    // tcp client
    connect(cli, SIGNAL(dataReceived(QString)), this, SLOT(testSlotforUserSignal(QString)));
    connect(cli, SIGNAL(connectionClosed()), this, SLOT(server_closed_connection()));
    // tcp server
    connect(svr, SIGNAL(dataReceived(QString)), this, SLOT(testSlotforUserSignal(QString)));
    //connect(svr, SIGNAL(connectionClosed(TcpClient *)), this, SLOT(connectionClosed(TcpClient *)));
    connect(svr, SIGNAL(connectionClosed(QString)), this, SLOT(testSlotforUserSignal(QString)));

    // for debugging let's connect right away!!
    //QHostAddress *host = new QHostAddress("192.168.204.151");
    //cli->connectToHost(*host, 45454);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testSlotforUserSignal(QString xxx)
{
        //ui->listWidget->addItem(datagram.data() + QString("... from ") + sender.toString());
    //ui->listWidget->addItem("xxxx");
    ui->listWidget->addItem(xxx);
}

/*void MainWindow::testSlotforUserSignal2()
{
    ui->listWidget->addItem("xxxx");
}*/

/*void MainWindow::newPeerFound(QHostAddress newHost)
{
    ui->listWidget->addItem(newHost.toString() + QString(" added."));
}*/

void MainWindow::newPeerFound(QString newHost)
{
    ui->listWidget->addItem(QString("Host ") + newHost + QString(" is hosting a game. Click to join."));
}

void MainWindow::startBroadcasting()
{
    //btnNew->setEnabled(false);
    ui->btnNew->setEnabled(false);
    peerMgr->BeginBroadcast();
}

void MainWindow::btnJoin_clicked()
{
    //QHostAddress *host = new QHostAddress("192.168.204.151");
    //cli->connectToHost(*host, 45454);
    QString *d = new QString("abcdefghijklmnop");
    cli->sendData(*d);
}

void MainWindow::server_closed_connection()
{

    // // QMessageBox::information(0, tr("Fortune Client"), tr("Server closed the connection!") );
    // // QMessageBox::information(0, tr("Fortune Client"), tr("Server closed the connection!") );

    // i think thid is  a mistake here!
    //delete cli;
    //cli = new TcpClient(this);
}

