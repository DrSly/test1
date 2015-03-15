// tcp server, based off example at http://doc.qt.io/qt-5/qtnetwork-fortuneserver-example.html
// alternative: threaded model at http://doc.qt.io/qt-5/qtnetwork-threadedfortuneserver-example.html

#include <QMessageBox>
#include "globals.h"
#include "tcpclient.h"
#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent = 0) :
    QObject(parent)
{
    //
    xtcpServer = new QTcpServer(this);
    if (!xtcpServer->listen(QHostAddress::Any, DEFAULT_PORT)) {
        //QMessageBox::critical(this, tr("Fortune Server"), tr("Unable to start the server: %1.") .arg(xtcpServer->errorString()));
        //QMessageBox::critical((QObject)this, tr("Fortune Server"), tr("Unable to start the server: %1.") .arg(xtcpServer->errorString()));
        QMessageBox::critical(0, tr("Fortune Server"), tr("Unable to start the server: %1."));
        return;
    }

    //
    fortunes << tr("You've been leading a dog's life. Stay off the furniture.")
             << tr("You've got to think about tomorrow.")
             << tr("You will be surprised by a loud noise.")
             << tr("You will feel hungry again in another hour.")
             << tr("You might have mail.")
             << tr("You cannot kill time without injuring eternity.")
             << tr("Computers are not intelligent. They only think they are.");

    //
    // //connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    // //connect(xtcpServer, SIGNAL(newConnection()), this, SLOT(sendFortune()));
    connect(xtcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
    /*connect(xtcpServer, SIGNAL(connectionClosed(TcpClient *)), this, SLOT(xxx(TcpClient *)));*/

    //
    refmember = 1;

}

TcpServer::~TcpServer()
{

    //
    refmember--;

}

/*void TcpServer::sendFortune()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
    out << fortunes.at(qrand() % fortunes.size());
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QTcpSocket *clientConnection = xtcpServer->nextPendingConnection();
    QObject::connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));


    clientConnection->write(block);
    // 31/12/2014: modified the server not to close the connection
    //clientConnection->disconnectFromHost();
}*/

void TcpServer::acceptConnection()
{
    QTcpSocket *newConnection = xtcpServer->nextPendingConnection();
    //QObject::connect(newConnection, SIGNAL(disconnected()), this, SLOT(removeClient()));
    // //QObject::connect(newConnection, SIGNAL(disconnected()), newConnection, SLOT(deleteLater()));
    // //connect(xtcpServer, SIGNAL(readyRead()), this, SLOT(readData()));
    //QObject::connect(newConnection, SIGNAL(dataReceived(QString)), this, SLOT(readData(QString)));
    TcpClient *asgdf = new TcpClient(this, newConnection);
    QObject::connect(asgdf, SIGNAL(connectionClosed()), this, SLOT(removeClient()));
    QObject::connect(asgdf, SIGNAL(dataReceived(QString)), this, SIGNAL(dataReceived(QString)));
    clientConnections.append(asgdf);
}

void TcpServer::removeClient()
{
    // client disconnected; remove reference to it from clientConnections
    //QTcpSocket *x = (QTcpSocket *)sender();
    TcpClient *x = (TcpClient *) sender();
    emit connectionClosed("Connection closed.");
    clientConnections.removeOne(x);
}

/*void TcpServer::readData()
{

    //
    QTcpSocket *tcpSocket = (QTcpSocket *)sender();
    //

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }

    // we may need to try somethig with decrementing the blocksize in case the entire message did not fit into a single packet
    if (tcpSocket->bytesAvailable() < blockSize)
        return;

    QString nextFortune;
    in >> nextFortune;

    // we have read entire datagram; output a data received signal
    blockSize = 0;
    emit dataReceived(nextFortune);

}*/

void TcpServer::sendData(QString &dataToSend, TcpClient *tcpClient)
{
    tcpClient->sendData(dataToSend);
}

/*void TcpServer::displayError(QAbstractSocket::SocketError socketError)
{

    //
    QTcpSocket *tcpSocket = (QTcpSocket *)sender();
    //

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        //QMessageBox::information(this, tr("Fortune Client"), tr("The host was not found. Please check the host name and port settings."));
        QMessageBox::information(0, tr("Fortune Client"), tr("The host was not found. Please check the host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        //QMessageBox::information(this, tr("Fortune Client"), tr("The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct."));
        QMessageBox::information(0, tr("Fortune Client"), tr("The connection was refused by the peer. Make sure the fortune server is running, and check that the host name and port settings are correct."));
        break;
    default:
        //QMessageBox::information(this, tr("Fortune Client"), tr("The following error occurred: %1.") .arg(tcpSocket->errorString()));
        QMessageBox::information(0, tr("Fortune Client"), tr("The following error occurred: %1.") .arg(tcpSocket->errorString()));
    }

    //getFortuneButton->setEnabled(true);
}*/

/*void TcpServer::xxx(TcpClient *xx)
{
}*/

