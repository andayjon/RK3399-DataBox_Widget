#include "udp_socket.h"
#include <QDebug>

udp_Socket::udp_Socket(QObject *parent)
{
    m_udpSocket = new QUdpSocket();
    m_thread = new QThread();
    m_timer = new QTimer(this);
    stop_stat = false;
    m_timer->setInterval(3000);
    connect(this, &udp_Socket::sig_threadStart, this, &udp_Socket::slot_threadStart);
    connect(m_timer, &QTimer::timeout, this, &udp_Socket::slot_timeOut);
    connect(this, &udp_Socket::sig_sendMsg, this, &udp_Socket::slot_sendMsg);
}

udp_Socket::~udp_Socket()
{
    m_thread->quit();
    m_thread->wait();
}

void udp_Socket::udpSocketInit(const int port)
{
    localHost = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(localHost);

    m_udpSocket->bind(port);
    connect(m_udpSocket, &QUdpSocket::readyRead, this, &udp_Socket::slot_recvMsg);
    qDebug()<<"--- udpSocket Init ---"<<endl;
}

void udp_Socket::slot_recvMsg()
{
    QByteArray msg;
    QHostAddress *recAddr;
    quint16 *recPort;

    while(m_udpSocket->hasPendingDatagrams())
    {
        if(-1 != m_udpSocket->pendingDatagramSize()) {
            msg.resize((int)(m_udpSocket->pendingDatagramSize()));
            m_udpSocket->readDatagram(msg.data(), msg.size());
            qDebug() << "UDP Receive Data Len = " << msg.size() << " Bytes";
            qDebug()<<tr("Udp Receive Data:%1").arg(QString(msg.toHex()))<<endl;
            emit sig_recvMsg(msg);
        }

    }
}

void udp_Socket::slot_sendMsg(QByteArray msg, int port)
{
    //writeDatagram方法传入４个参数，分别是数据，数据大小，接收端ip，接收端端口
    //如果传输成功，该方法返回传输数据的大小（字节），如果失败返回－１
    int len=m_udpSocket->writeDatagram(msg.data(), msg.length(), QHostAddress::Broadcast, port);
    if(len!=msg.length())
    {return;}

    emit sig_sendAccomplish();          //发出传输完毕的信号
}

void udp_Socket::slot_threadStart(int port)
{
    udpSocketInit(port);
    m_timer->start();
//    m_udpSocket->moveToThread(m_thread);
//    m_thread->start();

}

void udp_Socket::slot_timeOut()
{
    m_timer->stop();
    QHostInfo info = QHostInfo::fromName(localHost);
    qDebug() << "LocalHostAddress:" << info.addresses() <<endl;
    qDebug()<<"开启线程"<<QThread::currentThreadId();           //查看槽函数在哪个线程运行
    m_timer->start();
}
