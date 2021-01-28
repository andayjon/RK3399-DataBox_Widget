#include "thread.h"
#include <QTcpSocket>
#include <QTextCodec>
#include <QDateTime>
#include <QProcess>

Thread::Thread(QObject *parent)
    :QThread (parent),
     m_TcpSocket(NULL),
     m_isThreaStopped(false),
     m_isOkConnect(false)
{

}
Thread::~Thread()
{

}
void Thread::run()
{
    qDebug()<<tr(">>>Thread Start...")<<endl;
Q:
    //主动断开连接时设置m_isThreaStopped为true
    while(!m_isThreaStopped) {
        //qDebug()<<tr(">>>m_TcpSocket = ")<<m_TcpSocket<<endl;
        //检测客户端socket指针是否为空
        if(!m_TcpSocket) {
            m_TcpSocket = new QTcpSocket(this);
            connect(m_TcpSocket, SIGNAL(readyRead()), this, SLOT(onReadMsg()));
            connect(m_TcpSocket, SIGNAL(connected()), this, SLOT(onConnect()));
            connect(m_TcpSocket, &QTcpSocket::disconnected, this, &Thread::onDisConnect);
            connect(this, &Thread::send_toserver, this, &Thread::onSendMSG);
            qDebug()<<tr(">>>TcpSocket Config done!")<<endl;
        }
        if(!m_isOkConnect) {
            m_TcpSocket->connectToHost(m_QStrSocketIP, m_nSockPort);
            //等待连接 延时3秒，3秒内连接不上返回false
            m_isOkConnect = m_TcpSocket->waitForConnected(3000);
            qDebug()<<tr(">>>TcpSocket Connect Target Information: %1 : %2 --- Status = %3")
                      .arg(m_QStrSocketIP).arg(m_nSockPort).arg(m_isOkConnect)<<endl;
        }
        if(ping_Ip(m_QStrSocketIP)) {
            ;
        } else {
            m_isThreaStopped = true;
            emit offline_server();
        }
        if(!m_isOkConnect) {
            continue;
        }
        m_TcpSocket->waitForReadyRead(5000);      
    }
    if(m_isThreaStopped) {
        m_isThreaStopped = false;
        qDebug()<<tr(">>>m_isThreaStopped = %1 ").arg(m_isThreaStopped)<<endl;
        goto Q;
    }
}
void Thread::onConnect()
{
    m_isOkConnect = true;
}
//socket断开连接槽函数
void Thread::onDisConnect()
{
    //通过把m_isOkConnect设为false,在socket线程的run函数中将会重新连接主机
    m_isOkConnect = false;
}
void Thread::startThread(const QString &ip, int port)
{
    m_QStrSocketIP = ip;
    m_nSockPort = port;
    m_isThreaStopped = false;
    start();
}
//发送数据到服务器
void Thread::onSendMSG(QByteArray bytes)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    //socket->write(str.toLatin1());
    m_TcpSocket->write(bytes);
    qDebug()<<tr(">>>> %1 Send to Tcp_socket Server").arg(current_date)<<m_QStrSocketIP+":"<<m_nSockPort<<bytes.toHex()<<endl;
}
//socket接收槽函数
void Thread::onReadMSG()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");

    while(m_TcpSocket->bytesAvailable() > 0) {
        m_datagram.clear();
        m_datagram.resize(m_TcpSocket->bytesAvailable());
        m_TcpSocket->read(m_datagram.data(), m_datagram.size());
//        QString str = QString::fromLocal8Bit(m_datagram);
//        emit sendMsg(str);
        emit recv_formserver(m_datagram);
    }
}
//ping IP
//Linux上测试可以，windows永远是0 == exitCode
bool Thread::ping_Ip(QString ip)
{
        int exitCode;
#ifdef _TTY_ARMV4_
        QString strArg = "ping -s 2 -c 2 " + ip;        //linux平台下的格式
#else
        QString strArg = "ping " + ip + " -n 1 -i 2";  //windows下的格式
#endif
        exitCode = QProcess::execute(strArg);
        if(0 == exitCode)
        {
                //it's alive
                qDebug() << "shell ping " + ip + " sucessed!";
                return true;
          }
         else
          {
                qDebug() << "shell ping " + ip + " failed!";
                return false;
             }
}
