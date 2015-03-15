#include "globals.h"
#include "random.h"
#include "lanpeermgr.h"

LanPeerMgr::LanPeerMgr(QObject *parent = 0, int broadcastPort = DEFAULT_PORT, int broadcastInterval = 8) :
    QObject(parent)
{
    udpBroadcastPort = broadcastPort;
    udpBroadcastInterval = broadcastInterval;
    // setup sockets and timer
    Initialise();
}

LanPeerMgr::~LanPeerMgr()
{
    delete timer;
    delete udpBroadcastSocket;
    delete udpReceiveSocket;
}

void LanPeerMgr::BeginBroadcast()
{
    timer->start(udpBroadcastInterval * 1000);
}

void LanPeerMgr::EndBroadcast()
{
    timer->stop();
}

void LanPeerMgr::broadcastDatagram()
{
    QByteArray datagram = QByteArray::number(SenderId) + ";" + STR_ANNOUNCE;
    /* using QHostAddress::Broadcast no packets were received by the linux host
    udpBroadcastSocket->writeDatagram(datagram.data(), datagram.size(), QHostAddress::Broadcast, 45454);*/
    /* with host 192.168.255.255, only packets sent by linux host were received by windows host
     * packets sent by windows host were no seen by either machine
     * host 192.168.204.255 worked on all systems
    QHostAddress *bcast = new QHostAddress("192.168.255.255");*/
    /* original method; hard coded address
    QHostAddress *bcast = new QHostAddress("192.168.204.255");
    udpBroadcastSocket->writeDatagram(datagram.data(), datagram.size(), *bcast, udpBroadcastPort);
    */
    //++messageNo;
    // new way, parse all network interfaces to find which we are able to broadcast on
    // iterate over the saved list of network addresses; maybe we shouldn't do this every time
    foreach (QNetworkAddressEntry addrEntry, netAddrList) {
        if (!addrEntry.broadcast().isNull()) {
            udpBroadcastSocket->writeDatagram(datagram.data(), datagram.size(), addrEntry.broadcast(), udpBroadcastPort);
        }
    }
}

void LanPeerMgr::processPendingDatagrams()
{
    while (udpReceiveSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpReceiveSocket->pendingDatagramSize());
        QHostAddress sender;
        udpReceiveSocket->readDatagram(datagram.data(), datagram.size(), &sender, 0);
        // format of datagram elements seperated by a semicolon (;)
        // [SenderId];[MESSAGE];[optional MESSAGE PARAMETERS ...]
        QString strDatagram = QString(datagram.data());
        // process strDatagram and generate SIGNALS
        // split datagram.data() by semicolon (;)
        QStringList MessageArray = strDatagram.split(';');
        if (MessageArray[0].toInt() != SenderId) {
            /*switch (MessageArray[1]) {
            case STR_ANNOUNCE:
                break;
            default:
                break;
            }*/
            if (MessageArray[1] == STR_ANNOUNCE) {
                // generate a new peer SIGNAL every time we receive a packet from a sender not already in our list
                if (!peerList.contains(sender))
                {
                    peerList.insert(peerList.size(), sender);
                    emit peerAdded(sender.toString());
                    //=====================================================
                    // output interface details for debugging purposes
                    /*foreach (QNetworkAddressEntry x, netAddrList) {
                        if (!x.broadcast().isNull()) {
                        emit peerfound(QString("IP: ") + x.ip().toString());
                        emit peerfound(QString("MA: ") + x.netmask().toString());
                        emit peerfound(QString("BR: ") + x.broadcast().toString());
                        }
                    }*/
                    //=====================================================
                }
            }
            else if (MessageArray[1] == "") {
            }
            else {
                // generate a generic SIGNAL to allow client to handle the message
                emit messageReceived(strDatagram);
            }
            // emit peerfound for debugging purposes
            emit peerfound(datagram.data() + QString("... from ") + sender.toString());
        }
        // if m[1] = STR_ANNOUNCE
        // if m[0] = SenderId [ignore]
        /* emit peerfound2(); */
    }
}

void LanPeerMgr::Initialise()
{
    // socket and timer to broadcast
    timer = new QTimer(this);
    udpBroadcastSocket = new QUdpSocket(this);

    // get all interfaces available to broadcast on
    // QHostAddress *bcast = new QHostAddress("192.168.204.255");
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        netAddrList.append(netInterface.addressEntries());
    }

    // use a random number for Sender Id
    SenderId = Random::getRandomNumber(1, 999);

    // create socket to receive broadcasts on port udpBroadcastPort
    udpReceiveSocket = new QUdpSocket(this);
    udpReceiveSocket->bind(udpBroadcastPort, QUdpSocket::ShareAddress);

    // connect SIGNALS to SLOTS
    // udp broadcast
    connect(timer, SIGNAL(timeout()), this, SLOT(broadcastDatagram()));
    // udp receive
    connect(udpReceiveSocket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

