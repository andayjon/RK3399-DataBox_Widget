#ifndef ZMQDEALER_H
#define ZMQDEALER_H

#include "zmq.h"
#include <QObject>
#include <QThread>
#include <QMutex>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

class ZmqCommand;
class ZmqDealer : public QThread
{
    Q_OBJECT
public:
    explicit ZmqDealer(QObject *parent = nullptr);
    ~ZmqDealer();
    void stopImmediately();
    int zmqConn(int,QString);
    int sendData(QByteArray &);
    ZmqCommand *m_zmqcmd;
    bool stop_flag = false;
protected:
    virtual void run();
signals:
    void sigRecv(const QByteArray &);

private:
    void * m_contex = NULL;
    void * m_socket = NULL;
    int initZMQ();

    QByteArray test_arr;
    QMutex m_lock;
    bool m_isEnRun = true;
public slots:
    void slot_recvMsg(const QByteArray &);
    //void DoTask();
};
//#pragma pack(1)     //设置结构体为1字节对齐
typedef struct {
    char head[2];
    quint16 cmdCode;
    quint16 len;
    char *msg;
    char checksum;
    char end[2];
} Zmqcmd_stc;
//#pragma pack()		//结束结构体对齐设置
class ZmqCommand: public QObject
{
    Q_OBJECT
public:
//    explicit ZmqCommand(QObject *parent = nullptr);
//    ~ZmqCommand();
    //Order Structure
    //cmd 55AA--Frame Head
    //03E9--cmdCode
    //0000--size
    //E9--checksum(cmdCode+size+data)
    //AA55--Frame Tail
    Zmqcmd_stc cmd_stc;
    QByteArray cmd_read = QByteArray::fromHex("55AA03E90000E9AA55");    //查询指令
    QByteArray cmd_write = QByteArray::fromHex("55AA03EA0000EAAA55");   //返回指令

    bool compChecksum(QByteArray &, char);
    char calChecksum(QByteArray &);
};

#endif // ZMQDEALER_H
