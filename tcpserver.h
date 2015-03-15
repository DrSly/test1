#ifndef TCPSERVER_H
#define TCPSERVER_H

//#include <QTcpSocket>
#include <QTcpServer>
#include <QStringList>

//class QTcpSocket;
class TcpClient;
class QTcpServer;

class TcpServer : public QObject
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *);
    ~TcpServer();
    //void sendData(QString &dataToSend, QTcpSocket *);
    void sendData(QString &dataToSend, TcpClient *);
signals:
    void dataReceived(QString);
    //void connectionClosed(QTcpSocket *);
    //void connectionClosed(TcpClient *);
    void connectionClosed(QString);
private slots:
    //void sessionOpened();
    //void sendFortune();
    void acceptConnection();    // replaces sendFortune()
    void removeClient();
    //void readData();
    //void displayError(QAbstractSocket::SocketError socketError);
    //void readData(QString);
private:
    QTcpServer *xtcpServer;
    QList<TcpClient *> clientConnections;
    QStringList fortunes;
    //quint16 blockSize;
    int refmember;
};

#endif // TCPSERVER_H
