// tcp server, based off example at http://doc.qt.io/qt-5/qtnetwork-fortuneclient-example.html
// alternative: threaded model at http://doc.qt.io/qt-5/qtnetwork-blockingfortuneclient-example.html

// suggestion: add a flag to determine whether the socket should be reused or destroyed
// if the server closes the connection, and assign the DeleteLater slot accordingly

#include <QMessageBox>
#include "globals.h"
#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent = 0) :
    QObject(parent)
{
    tcpSocket = new QTcpSocket(this);
    blockSize = 0;

    //connect(hostCombo, SIGNAL(editTextChanged(QString)), this, SLOT(enableGetFortuneButton()));
    //connect(portLineEdit, SIGNAL(textChanged(QString)), this, SLOT(enableGetFortuneButton()));
    //connect(getFortuneButton, SIGNAL(clicked()), this, SLOT(requestNewFortune()));
    //connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    /*connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));*/
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(displayError(QAbstractSocket::SocketError)));
    // //connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));
    // connectionClosed, do not attempt to send any more data!
    connect(tcpSocket, SIGNAL(disconnected()), this, SIGNAL(connectionClosed()));


    // test --- should generate an error if the server is not running
    //tcpSocket->connectToHost("192.168.204.151", DEFAULT_PORT);
}

TcpClient::~TcpClient()
{
    disconnectFromHost();  // closes the socket if it is still open :)
    delete tcpSocket;
}

/*void TcpClient::requestNewFortune()
{
    blockSize = 0;
    tcpSocket->abort();
    // to do; specify remote IP end point as a parameter to this function
    // //tcpSocket->connectToHost(hostCombo->currentText(), portLineEdit->text().toInt());
    tcpSocket->connectToHost("192.168.204.151", DEFAULT_PORT);
}*/

void TcpClient::connectToHost(QHostAddress &host, quint16 port = DEFAULT_PORT)
{
    /*
    if (tcpSocket->state() != QAbstractSocket::ConnectedState)
    tcpSocket->connectToHost(host, port);*/

    QAbstractSocket::SocketState scktState = tcpSocket->state();
    if ((scktState != QAbstractSocket::ConnectedState) && (scktState != QAbstractSocket::ConnectingState) && (scktState != QAbstractSocket::HostLookupState))
        tcpSocket->connectToHost(host, port);
}

void TcpClient::disconnectFromHost()
{
    tcpSocket->disconnectFromHost();
    // -- or --
    //tcpSocket->close();
}

/*void TcpClient::readFortune()
{
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if (blockSize == 0) {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }

    if (tcpSocket->bytesAvailable() < blockSize)
        return;

    QString nextFortune;
    in >> nextFortune;

    //if (nextFortune == currentFortune) {
        //QTimer::singleShot(0, this, SLOT(requestNewFortune()));
        //return;
    //}

    //currentFortune = nextFortune;
    //statusLabel->setText(currentFortune);
    //getFortuneButton->setEnabled(true);
}*/

void TcpClient::readData()
{
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

}

void TcpClient::sendData(QString &dataToSend)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << (quint16)0;
        //out << fortunes.at(qrand() % fortunes.size());
    out << dataToSend;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    // //QTcpSocket *clientConnection = xtcpServer->nextPendingConnection();
    // //QObject::connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

    tcpSocket->write(block);
}

void TcpClient::displayError(QAbstractSocket::SocketError socketError)
{
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
}

