#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>

namespace Ui {
class MainWindow2;
}
class TcpClient;

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = 0);
    ~MainWindow2();

private slots:
    void connectClick();
    void sendClick();
    void testSlotforUserSignal(QString);
    void server_closed_connection();

private:
    Ui::MainWindow2 *ui;
    TcpClient *cli;
};

#endif // MAINWINDOW2_H
