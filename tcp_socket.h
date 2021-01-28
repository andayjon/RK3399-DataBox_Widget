#ifndef TCP_SOCKET_H
#define TCP_SOCKET_H

#include <sys/stat.h>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>
#include <QProcess>

#define _TTY_ARMV4_ 0

class tcp_socket:public QObject
{
    Q_OBJECT
public:
    explicit tcp_socket(QObject *parent);
    ~tcp_socket();

    bool connectServer();
    bool m_isOkConect = false;
    bool m_isThreaStopped = false;
    bool m_ping_enable = false;
    bool m_ping_stat = false;
    bool m_online_stat = false;
    QString server_ip;
    quint16 server_port;
    quint8 reconn_num = 0;
private:
    QTcpSocket *socket;
    QTimer *recon_timer;
    QTimer *recon_timer2;
    quint16 send_num = 0;
    QByteArray recMsg;
    bool ping_net(QString);
    bool ping_IP(QString);
signals:
    void send_toserver(QByteArray);
    void recv_formserver(QByteArray);
    void conn_toServerOk();
    void onDisConnect();

private slots:
    void slot_writeMsgToServer(QByteArray);
    void slot_onDisConnect();
    void slot_onConnect();
    void slot_readMsgFromServer();
    void slot_time1out();
    void slot_time2out();
};

#endif // TCP_SOCKET_H
