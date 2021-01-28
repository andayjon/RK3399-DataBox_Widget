#include "tcp_socket.h"
#include <QDateTime>

tcp_socket::tcp_socket(QObject *parent)
{
    socket = new QTcpSocket(this);
    recon_timer = new QTimer(this);
    recon_timer2 = new QTimer(this);
    connect(socket, &QTcpSocket::readyRead, this, &tcp_socket:: slot_readMsgFromServer);
    connect(socket, &QTcpSocket::connected, this, &tcp_socket::slot_onConnect);
    connect(socket, &QTcpSocket::disconnected, this, &tcp_socket::slot_onDisConnect);
    connect(this, &tcp_socket::onDisConnect, this, &tcp_socket::slot_onDisConnect);
    //connect(tcp_socket, &QTcpSocket::error(QAbstractSocket::SocketError), this, &tcp_client::onError(QAbstractSocket::SocketError));
    connect(this, &tcp_socket::send_toserver, this, &tcp_socket::slot_writeMsgToServer);
    connect(this->recon_timer,&QTimer::timeout, this, &tcp_socket::slot_time1out);
    connect(this->recon_timer2, &QTimer::timeout, this, &tcp_socket::slot_time2out);
    m_ping_enable = true;
    recon_timer->start(5000);
}

tcp_socket::~tcp_socket()
{
    delete this;
}

//连接到服务器
bool tcp_socket::connectServer()
{
    while(!m_isThreaStopped) {
        if(!m_isOkConect) {
            if(server_port!=0 && !server_ip.isEmpty()) {
                socket->abort();
                socket->connectToHost(server_ip,server_port);
                qDebug()<<tr(">>>Tcp_socket connetcted to server IP:")<<server_ip<<":"<<server_port<<endl;
                m_isOkConect = socket->waitForConnected(3000);
                if(m_isOkConect) {
                    qDebug()<<tr(">>>Tcp socket Connect Status: %1 ").arg(m_isOkConect)<<endl;
                    recon_timer2->start(3000);
                }
            } else {
                //socket->connectToHost("192.168.1.168",18816);   //默认连接设备
                qDebug()<<tr(">>>Tcp_socket Invalid Server IP Address: %1 and Port Number: %2 ")
                          .arg(server_ip).arg(server_port)<<endl;
                m_ping_enable = false;
            }
        }
        if(socket->waitForConnected(5000)) {
            qDebug()<<">>>Tcp_socket connect to"<<server_ip+":"<<server_port<<" success!"<<endl;
            return true;
        } else {
            qDebug()<<">>>Tcp_socket connect to"<<server_ip+":"<<server_port<<" faild!"<<endl;
            m_ping_enable = true;
            return false;
        }
        if(socket->waitForReadyRead()) {
            qDebug()<<tr(">>>Tcp_socket Ready Read = true...")<<endl;
        }

    }
}

//已经连接
void tcp_socket::slot_onConnect()
{
    qDebug()<<tr(">>>TCP Socket connnect to IP = %1, Port = %2 succeed.").arg(server_ip).arg(server_port)<<endl;
    m_isOkConect = true;
    m_online_stat = true;
    reconn_num = 0;
    emit conn_toServerOk();
}

//连接断开
void tcp_socket::slot_onDisConnect()
{
    m_isOkConect = false;
    m_online_stat = false;
    socket->disconnectFromHost();
    socket->close();
    socket->abort();
    qDebug()<<tr(">>>TCP Socket Recreate a Connection IP = %1, Port = %2...").arg(server_ip).arg(server_port)<<endl;

}

//发送数据到服务器
void tcp_socket::slot_writeMsgToServer(QByteArray bytes)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    //socket->write(str.toLatin1());
    socket->write(bytes);
    qDebug()<<tr(">>>> %1 Send to Tcp_socket Server").arg(current_date)<<server_ip+":"<<server_port<<bytes.toHex()<<endl;
}

//读取服务器发送消息
void tcp_socket::slot_readMsgFromServer()
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    recMsg = socket->readAll();
    qDebug()<<tr(">>> %1 Tcp_socket RecMsg fFrom Server").arg(current_date)<<server_ip+":"<<server_port<<recMsg<<endl;
    m_isOkConect = true;
    emit recv_formserver(recMsg);

}

void tcp_socket::slot_time1out()
{
    recon_timer->stop();
    if(m_ping_enable) {
        if(!ping_net(server_ip)) {
            if(m_isOkConect)
                emit onDisConnect();
            m_ping_stat = false;
        }else {
            m_ping_stat = true;
            if(!m_isOkConect)
                connectServer();
        }
    }
    recon_timer->start(5000);
}
void tcp_socket::slot_time2out()
{
    recon_timer2->stop();
    if(!m_isOkConect) {
        emit onDisConnect();
        if(m_ping_stat)
            connectServer();
        return;
    }
    m_isOkConect = false;
    recon_timer2->start(3000);
}
//ping IP
//Linux上测试可以，windows永远是0 == exitCode
//这段代码 我在Linux上测试可以，但是windows永远是0 == exitCode
bool tcp_socket::ping_net(QString ip)
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
        } else
        {
            qDebug() << "shell ping " + ip + " failed!";
            return false;
        }
}

bool tcp_socket::ping_IP(QString ip)
{
    QProcess pingProcess;
    QString strArg = "ping -s 2 -c 2" + ip;  //strPingIP 为设备IP地址
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    QString p_stdout = QString::fromLocal8Bit(pingProcess.readAllStandardOutput());

    if(p_stdout.contains("TTL")) {
        return true;
    }
    return false;
}
