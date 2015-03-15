#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QStringList>

class QTcpSocket;
class QTcpServer;

class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *);
    ~TcpServer();
    void sendData(QString &dataToSend, QTcpSocket *);     // partially replaces requestNewFortune()
signals:
    void dataReceived(QString);
    void connectionClosed(QTcpSocket *);
private slots:
    //void sessionOpened();
    void sendFortune();
    void acceptConnection();    // replaces sendFortune()
    void removeClient();
    void readData();
    void displayError(QAbstractSocket::SocketError socketError);
private:
    QTcpServer *xtcpServer;
    QList<QTcpSocket *> clientConnections;
    QStringList fortunes;
    quint16 blockSize;
};

#endif // TCPSERVER_H
