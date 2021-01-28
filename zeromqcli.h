#ifndef ZEROMQCLI_H
#define ZEROMQCLI_H

#include "zmq.h"
#include "stdio.h"
#include "stdlib.h"
#include <QObject>
#include <QThread>
#include <QString>
#include <QByteArray>
#include <QDebug>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QMutex>
#include <QTimer>

class mq_param : public QObject{
    Q_OBJECT
public:
    explicit mq_param(QObject *parent = nullptr){};
    QString ip_server;
    int port_sub = 5576;    //订阅
    int port_pub = 5577;    //发布

};

class zeromqCli : public QThread
{
    Q_OBJECT
public:
    explicit zeromqCli(QObject *parent = nullptr);
    ~zeromqCli();
    void stopImmediately();
    mq_param *m_mqparam;
    bool stop_flag = false;
    bool conn_stat = false;
signals:
    void sig_sendMsg(QByteArray);
    void sig_recvMsg(QByteArray);
private:
    void run();
    void * pCtx = NULL;
    void * sCtx = NULL;
    void * pSock = NULL;
    void * sSock = NULL;
    const char * pAddr = "tcp://192.168.10.125:7766";
    const char * pubAddr1 = "tcp://192.168.150.3:5576";
    const char * subAddr1 = "tcp://192.168.150.3:5577";
//    char * pubAddr;
//    char * subAddr;
    QMutex m_lock;
    bool m_isEnRun = true;
    QTimer *m_tim1;
public slots:
    void slot_sendMsg(QByteArray);
    void slot_recvMsg(QByteArray);
    void slot_tim1Timeout();
};

#endif // ZEROMQCLI_H
