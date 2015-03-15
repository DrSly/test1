#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "tcpclient.h"
#include <QHostAddress>

MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{
    ui->setupUi(this);

    cli = new TcpClient(this);

    // connect SIGNALS to SLOTS
    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(connectClick()));
    connect(ui->btnSend, SIGNAL(clicked()), this, SLOT(sendClick()));
    connect(cli, SIGNAL(dataReceived(QString)), this, SLOT(testSlotforUserSignal(QString)));
    connect(cli, SIGNAL(connectionClosed()), this, SLOT(server_closed_connection()));
}

MainWindow2::~MainWindow2()
{
    delete ui;
}

void MainWindow2::connectClick()
{
    ui->btnConnect->setEnabled(false);
    QHostAddress *host = new QHostAddress("192.168.204.168");
    cli->connectToHost(*host, 45454);
}

void MainWindow2::sendClick()
{
    QString *d = new QString("abcdefghijklmnop");
    cli->sendData(*d);
}

void MainWindow2::testSlotforUserSignal(QString xxx)
{
    ui->listWidget->addItem(xxx);
}

void MainWindow2::server_closed_connection()
{

    // // QMessageBox::information(0, tr("Fortune Client"), tr("Server closed the connection!") );
    // // QMessageBox::information(0, tr("Fortune Client"), tr("Server closed the connection!") );

    // i think thid is  a mistake here!
    //delete cli;
    //cli = new TcpClient(this);

    ui->listWidget->addItem("Connection closed.");
    ui->btnConnect->setEnabled(true);
}

