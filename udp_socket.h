#ifndef UDP_SOCKET_H
#define UDP_SOCKET_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QHostInfo>
#include <QByteArray>
#include <QString>
#include <QThread>
#include <QtMath>
#include <qmath.h>

class udp_Socket : public QObject
{
    Q_OBJECT
public:
    udp_Socket(QObject *parent);
    ~udp_Socket();
    void udpSocketInit(const int);
    QThread *m_thread;
    volatile bool stop_stat;
    void stop(){ stop_stat = true;}

signals:
    void sig_recvMsg(QByteArray);
    void sig_threadStart(int);
    void sig_sendMsg(QByteArray, int);
    void sig_sendAccomplish();
public slots:
    void slot_recvMsg(void);
    void slot_sendMsg(QByteArray, int);
    void slot_threadStart(int);
    void slot_timeOut();
private:
    QUdpSocket *m_udpSocket;
    QTimer *m_timer;
    QString localHost;
};

#endif // UDP_SOCKET_H
