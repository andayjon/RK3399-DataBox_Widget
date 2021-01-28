#include "zeromqcli.h"
#define DEBUG_EN
zeromqCli::zeromqCli(QObject *parent) : QThread(parent)
{
    m_mqparam = new mq_param(this);
    connect(this, &zeromqCli::sig_sendMsg, this, &zeromqCli::slot_sendMsg);
    m_tim1 = new QTimer(this);
    connect(m_tim1, &QTimer::timeout, this, &zeromqCli::slot_tim1Timeout);

//    m_tim1->setInterval(2000);
//    m_tim1->start();
//    connect(this, &zeromqCli::sig_recvMsg, this, &zeromqCli::slot_recvMsg);
}

zeromqCli::~zeromqCli()
{

}

void zeromqCli::run()
{
    //通信使用的网络端口 为7766
        //const char * pAddr = "tcp://192.168.10.130:7766"; zlinwei.com
    const char *pubAddr = "tcp://zlinwei.com:5576";   /*QString("tcp://%1:%2").arg(m_mqparam->ip_server).arg(m_mqparam->port_pub).toLatin1().data();*/
    const char *subAddr = "tcp://zlinwei.com:5577";   /*QString("tcp://%1:%2").arg(m_mqparam->ip_server).arg(m_mqparam->port_sub).toLatin1().data();*/

    //创建context
        if((pCtx = zmq_ctx_new()) == NULL)
        {
            return;
        }
        //创建socket
        if((pSock = zmq_socket(pCtx, ZMQ_PUB)) == NULL)
        {
            zmq_ctx_destroy(pCtx);
            return;
        }
        int iSndTimeout = 5000; // millsecond
        int iRcvTimeout = 5000;
        //设置发送超时
        if(zmq_setsockopt(pSock, ZMQ_SNDTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
        {
            zmq_close(pSock);
            zmq_ctx_destroy(pCtx);
            return;
        }
        //设置接收超时
        if(zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0)
        {
            zmq_close(pSock);
            zmq_ctx_destroy(pCtx);
            return;
        }

        //连接目标IP192.168.1.2，端口7766
        if(zmq_connect(pSock, pubAddr) < 0)
        {
            zmq_close(pSock);
            zmq_ctx_destroy(pCtx);
            return;
        }

        if((sCtx = zmq_ctx_new()) == NULL)
        {
            return;
        }
        if((sSock = zmq_socket(sCtx, ZMQ_SUB)) == NULL)
        {
            zmq_ctx_destroy(sCtx);
            return;
        }
        //设置发送超时
        if(zmq_setsockopt(sSock, ZMQ_SNDTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
        {
            zmq_close(sSock);
            zmq_ctx_destroy(sCtx);
            return;
        }
        //设置接收超时
        if(zmq_setsockopt(sSock, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0)
        {
            zmq_close(sSock);
            zmq_ctx_destroy(sCtx);
            return;
        }
        if(zmq_setsockopt(sSock, ZMQ_SUBSCRIBE, "", 0) != 0) {
            zmq_disconnect(sSock, subAddr);
            zmq_close(sSock);
            zmq_ctx_destroy(sCtx);
            return;
        }
        //连接目标IP192.168.1.2，端口7766
        if(zmq_connect(sSock, subAddr) < 0)
        {
            zmq_close(sSock);
            zmq_ctx_destroy(sCtx);
            return;
        }

        m_isEnRun = true;
        //循环发送消息
        while(1)
        {
            char szMsg[1024*8] = {0};
#ifdef DEBUG_EN
            qDebug()<< tr("please waitting ...") <<endl;
#endif
            errno = 0;
            //循环等待接收到来的消息，当超过5秒没有接到消息时，
            //zmq_recv函数返回错误信息 ，并使用zmq_strerror函数进行错误定位
            if(zmq_recv(sSock, szMsg, sizeof(szMsg), 0) < 0)
            {
                //printf("error = %s\n", zmq_strerror(errno));
#ifdef DEBUG_EN
                qDebug()<< tr("Thread ID : %1").arg(quintptr(QThread::currentThreadId()));
                qDebug()<< tr("error = %1").arg(zmq_strerror(errno)) <<endl;
#endif
                conn_stat = false;
                //continue;
            } else {
                //printf("received message : %s\n", szMsg);
                //qDebug()<< tr("receive message : %1").arg(szMsg) <<endl;
                conn_stat = true;
                QByteArray msg;
                msg = szMsg;
                emit sig_recvMsg(msg);
            }

            QMutexLocker locker(&m_lock);
            if(!m_isEnRun) {conn_stat = false; return;}
            //msleep(200);
        }

        return;
}

void zeromqCli::slot_recvMsg(QByteArray msg)
{
#ifdef DEBUG_EN
    qDebug()<< tr("Recv Msg: %1").arg(QString(msg)) <<endl;
#endif
}

void zeromqCli::slot_sendMsg(QByteArray msg)
{
    unsigned int i = msg.size();
    char szMsg[1024] = {0};

    //snprintf(szMsg, sizeof(szMsg), "hello world : %3d", i++);
    //printf("Enter to send...\n");
#ifdef DEBUG_EN
    qDebug()<< tr("Enter to send...") <<endl;
#endif
    if(zmq_send(pSock, msg, i, 0) < 0)
    {
        //fprintf(stderr, "send message faild\n");
#ifdef DEBUG_EN
        qDebug()<< tr("send message faild") <<endl;
#endif
    } else {
#ifdef DEBUG_EN
        qDebug()<< tr("send message : [%1] succeed").arg(QString(msg)) <<endl;
#endif
    }
    //printf("send message : [%s] succeed\n", szMsg);
    //getchar();
}

//立即停止Thread
void zeromqCli::stopImmediately()
{
    QMutexLocker locker(&m_lock);
    m_isEnRun = false;
}

void zeromqCli::slot_tim1Timeout()
{

}
