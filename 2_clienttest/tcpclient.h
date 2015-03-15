#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QTcpSocket>

class QTcpSocket;

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *);
    explicit TcpClient(QObject *, QTcpSocket *);
    ~TcpClient();
    void connectToHost(QHostAddress &, quint16);  // partially replaces requestNewFortune()
    void disconnectFromHost();
    void sendData(QString &dataToSend);     // partially replaces requestNewFortune()
signals:
    void dataReceived(QString);
    void connectionClosed();
private slots:
    //void requestNewFortune();
    //void readFortune();
    void readData();    // replaces readFortune()
    void displayError(QAbstractSocket::SocketError socketError);
    void connectionClosedByHost();
    //void enableGetFortuneButton();
    //void sessionOpened();
//private:
    //void disconnectFromHost();
private:
    QTcpSocket *tcpSocket;
    //QString currentFortune;
    quint16 blockSize;
};

#endif // TCPCLIENT_H
