#ifndef NETCAN_DEV_H
#define NETCAN_DEV_H

#include "tcp_socket.h"
#include <QObject>
#include <QWidget>

class Netcan_dev:public QObject
{
    Q_OBJECT
public:
    explicit Netcan_dev(QObject *parent);
    ~Netcan_dev();
    QString server_ip;      //
    int server_port;        //
    bool online_stat;       //设备在线状态
    tcp_socket *tcp_cli;    //socket客户端
    //Thread *tcp_thread;     //tcp socket监测线程
protected:
private:
    QByteArray recv_data;   //接收数据
signals:
    void signal_recvdata(QByteArray);
public slots:
    void slot_msgFormServer(QByteArray);    //接收服务器数据
    void slot_tcpDisconnect();      //TCP重连
};

#endif // NETCAN_DEV_H
