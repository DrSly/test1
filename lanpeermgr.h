#ifndef LANPEERMGR_H
#define LANPEERMGR_H

#include <QUdpSocket>
//#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QTimer>
#include <QList>
#include <QStringList>

class QUdpSocket;
class QNetworkAddressEntry;
class QNetworkInterface;
class QTimer;

class LanPeerMgr : public QObject
{
    Q_OBJECT

public:
    explicit LanPeerMgr(QObject *, int, int);
    ~LanPeerMgr();
    void BeginBroadcast();
    void EndBroadcast();
signals:
    void peerfound(QString);
    void peerAdded(QString);
    void messageReceived(QString);
    // //void peerAdded(QHostAddress);
    // //void peerfound2();//(char *myParameter);
private slots:
    void broadcastDatagram();
    void processPendingDatagrams();
private:
    void Initialise();
private:
    quint16 udpBroadcastPort;
    int udpBroadcastInterval;
    QUdpSocket *udpBroadcastSocket;
    QUdpSocket *udpReceiveSocket;
    QTimer *timer;
    int SenderId;
    // list of network interfaces available to broadcast on
    QList<QNetworkAddressEntry> netAddrList;
    // list of Host addresses
    QList<QHostAddress> peerList;
};

#endif // LANPEERMGR_H
