#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <QWidget>
#include <QMainWindow>
//#include <QUdpSocket>   // required for reference to QHostAddress

namespace Ui {
class MainWindow;
}
class LanPeerMgr;
class TcpServer;
class TcpClient;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void dataReceived(QString);

private slots:
    //void on_pushButton_clicked();
    void startBroadcasting();
    void btnJoin_clicked();
    // // //void broadcastDatagram();
    // // //void processPendingDatagrams();
    void testSlotforUserSignal(QString);
    void newPeerFound(QString);
    // // //void newPeerFound(QHostAddress);
    /* void testSlotforUserSignal2(); */
    void server_closed_connection();

private:
    Ui::MainWindow *ui;
    LanPeerMgr *peerMgr;
    TcpServer *svr;
    TcpClient *cli;
};

#endif // MAINWINDOW_H
