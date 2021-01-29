#include "zmqdealer.h"
#include <assert.h>
#include <qglobal.h>
#include <QtDebug>
#include <QThread>

QString zmq_addr;
int zmq_mode;

ZmqDealer::ZmqDealer(QObject *parent) : QThread(parent)
{

    //m_zmqcmd = new ZmqCommand(this);
    connect(this, &ZmqDealer::sigRecv, this, &ZmqDealer::slot_recvMsg);
    initZMQ();
    zmqConn(0, "tcp://*:10086");

}
ZmqDealer::~ZmqDealer()
{

}

int ZmqDealer::initZMQ()
{
    //创建context
    if((m_contex = zmq_ctx_new()) == NULL) {
        return -1;
    }

    //创建socket
    if((m_socket = zmq_socket(m_contex, ZMQ_DEALER)) == NULL) {
        zmq_ctx_destroy(m_contex);
        return -2;
    }
    int iSndTimeout = 3000; // millsecond
    int iRcvTimeout = 3000;
    //设置发送超时
    if(zmq_setsockopt(m_socket, ZMQ_SNDTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0) {
        zmq_close(m_socket);
        zmq_ctx_destroy(m_contex);
        return -3;
    }
    //设置接收超时
    if(zmq_setsockopt(m_socket, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0) {
        zmq_close(m_socket);
        zmq_ctx_destroy(m_contex);
        return -4;
    }
    return  0;
}

int ZmqDealer::zmqConn(int mode, QString pAddr)
{
    QByteArray ba = pAddr.toLatin1();
    const char *addr = ba.data();
    switch (mode) {
    case 0: //服务端模式
        if(zmq_bind(m_socket, addr) < 0) {
            zmq_close(m_socket);
            zmq_ctx_destroy(m_contex);
            qDebug()<< QString("zmq bin return fail.")<<endl;
            return -1;
        }
        break;
    case 1: //客户端模式
        if(zmq_connect(m_socket, addr) < 0) {

            zmq_close(m_socket);
            zmq_ctx_destroy(m_contex);
            qDebug()<< QString("zmq connect return fail.")<<endl;
            return -2;
        }
        break;
    default:
        break;
    }
    return 0;
}
void ZmqDealer::run()
{
    char buffer[1024];
    int nbytes = 0;

    while(1) {
        memset(buffer, 0, 1024*(sizeof(char)));
        nbytes = zmq_recv(m_socket, &buffer, 1024, 0);
        if(nbytes == 1) {
            qDebug() << "receiver cmd null, continue";
            continue;
        }
        qDebug()<< QString("Dealer线程%1 , 收到数据 %2字节").arg(quintptr(QThread::currentThreadId())).arg(nbytes);
        if(nbytes > 5) {
            QByteArray msg;
            msg.append((char*)buffer, nbytes);
            emit sigRecv(msg);
        }
        QMutexLocker locker(&m_lock);
        if(!m_isEnRun) {
            return;
        }
    }
}
//发送数据
int ZmqDealer::sendData(QByteArray &ba)
{

    if(!ba.isEmpty()) {
        int ret = zmq_send(m_socket, ba, ba.size(), 0);
        if(ret < 0) {
            return  -1;
        }
        qDebug()<< QString("Dealer线程%1 , 发送数据 %2字节").arg(quintptr(QThread::currentThreadId())).arg(ret);
    }
    return 0;
}

void ZmqDealer::slot_recvMsg(const QByteArray &ba)
{
    if(!ba.isEmpty()) {
        qDebug()<< QString("-->>ZMQ Receive Data:%1").arg(QString(ba)) <<endl;
    }
}
//立即停止Thread
void ZmqDealer::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isEnRun = false;
}

//对比计算校验和
bool ZmqCommand::compChecksum(QByteArray &ba, char sum)
{
    char checksum = 0;
    //计算校验和
    for(int i = 0; i < ba.size(); i++) {
        checksum += ba.at(i);
    }
    if(checksum != sum) return false;
    return true;
}
//计算校验和
char ZmqCommand::calChecksum(QByteArray &ba)
{
    char checksum = 0;
    //计算校验和
    for(int i = 0; i < ba.size(); i++) {
        checksum += ba.at(i);
    }
    return checksum;
}
