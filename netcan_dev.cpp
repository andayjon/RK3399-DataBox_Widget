#include "netcan_dev.h"

Netcan_dev::Netcan_dev(QObject *parent)
{
    tcp_cli = new tcp_socket(this);
    connect(tcp_cli, &tcp_socket::recv_formserver, this, &Netcan_dev::slot_msgFormServer);
    connect(tcp_cli, &tcp_socket::onDisConnect, this, &Netcan_dev::slot_tcpDisconnect);

}
Netcan_dev::~Netcan_dev()
{
    delete this;
}
void Netcan_dev::slot_msgFormServer(QByteArray bytes)
{
    if(bytes.length() == 13) {
        emit signal_recvdata(bytes);
    } else {
        ;
    }
}
void Netcan_dev::slot_tcpDisconnect()
{
    online_stat = false;
    qDebug()<<tr("--->>>Net CAN Device Online status = %1").arg(online_stat)<<endl;
}
