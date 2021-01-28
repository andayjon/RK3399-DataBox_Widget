#include "datareceive.h"
#include "cmaskgenerator.h"
#include <QDebug>
#include <QWindow>
#include <QMainWindow>

#define DEBUG_EN
DataReceive::DataReceive(QObject *parent):QObject(parent)
{
    dataRecvWS = Q_NULLPTR;
    connectStatus = false;
    m_timer = new QTimer();
}

DataReceive::~DataReceive()
{
    delete this;
}

/**
 * @breaf 创建WebSocket连接
 */
void DataReceive::createDataRecvWS(){

    if(dataRecvWS == Q_NULLPTR){
        CMaskGenerator *m_mask = new CMaskGenerator();
        dataRecvWS = new QWebSocket();
        dataRecvWS->setMaskGenerator(m_mask);
#ifdef DEBUG_EN
        qDebug()<<"create websocket!";
#endif
        //connect(dataRecvWS,&QWebSocket::disconnected,this,DataReceive::onDisConnected,Qt::AutoConnection);
        //connect(dataRecvWS,&QWebSocket::textMessageReceived,this,DataReceive::onTextReceived,Qt::AutoConnection);
        //connect(dataRecvWS,&QWebSocket::connected,this,DataReceive::onConnected,Qt::AutoConnection);
        //connect(m_timer,m_timer->timeout,this,DataReceive::reconnect,Qt::AutoConnection);
        connect(dataRecvWS,SIGNAL(disconnected()),this,SLOT(onDisConnected()));
        connect(dataRecvWS,SIGNAL(textMessageReceived(QString)),this,SLOT(onTextReceived(QString)));
        connect(dataRecvWS,SIGNAL(connected()),this,SLOT(onConnected()));
        connect(m_timer,SIGNAL(timeout()),this,SLOT(reconnect()));
        url.append(URL);
        url.append(dev_id);
        dataRecvWS->open(QUrl(url));
#ifdef DEBUG_EN
        qDebug()<<"url="<<url;
#endif
//        dataRecvWS->open(QUrl("ws://192.168.0.17:60000/data"));
    }
}

/**
 * @breaf 判断连接状态
 * @note  当连接成功后，触发该函数
 */
void DataReceive::onConnected(){
#ifdef DEBUG_EN
    qDebug()<<"DataReveive websocket is already connect!";
    qDebug()<<"Address："<<dataRecvWS->peerAddress();
#endif
    connectStatus = true;
    m_timer->stop();

}

/**
 * @breaf 数据处理函数
 * @param msg，数据信息
 * @note  当收到服务端发送的数据时，触发该函数
 */
void DataReceive::onTextReceived(QString msg){
    QTime rx_time = QTime::currentTime();
    QByteArray tmp_arr;
#ifdef DEBUG_EN
    qDebug()<<"Socket Receive data time --"<<rx_time.toString();
    qDebug()<<"----------------data-----------------";
    qDebug()<<url<<endl;
    qDebug()<<"接收到服务器下发信息:";
    qDebug()<<msg.toStdString().c_str();
#endif
    connectStatus = true;
    tmp_arr = msg.toLatin1();
    emit recv_Msg_pro(tmp_arr);
}

/**
 * @breaf 连接断开
 * @note  当连接断开时，触发该函数
 */
void DataReceive::onDisConnected(){
#ifdef DEBUG_EN
    qDebug()<<"DataReceive websocket is disconnected!!!";
    qDebug()<<url<<endl;
    qDebug("连接又断开了！怎么办？");
#endif
    connectStatus = false;
    m_timer->start(3000);/*-<当连接失败时，触发重连计时器，设置计数周期为3秒 */
}

/**
 * @breaf 周期重连函数
 * @note  定时器超时后，调用该函数
 */
void DataReceive::reconnect(){

    dataRecvWS->abort();
    dataRecvWS->open(QUrl(url));
#ifdef DEBUG_EN
    qDebug()<<"try to reconnect!";
    qDebug()<<url;
#endif
//    dataRecvWS->open(QUrl("ws://192.168.0.17:60000/data"));
}

void DataReceive::closeConnected(){
#ifdef DEBUG_EN
    qDebug()<<"关闭连接";
#endif
    dataRecvWS->close();
}
//socket发送文本消息
void DataReceive::sendTextmsg(QString msg){
    QTime tx_time = QTime::currentTime();
    QByteArray arry = msg.toUtf8();
    qint64 send_len;
#ifdef DEBUG_EN
    qDebug()<<"Socket send data time -- "<<tx_time.toString();
    qDebug()<<"Texe-发送数据中.....";
#endif
    send_len = dataRecvWS->sendTextMessage(msg);
#ifdef DEBUG_EN
    qDebug()<<"send_len:"<<send_len<<"Byte."<<endl;
    qDebug()<<">>>websocket send data to :" + url + "<<<"<<endl;
    qDebug()<<msg<<endl;
#endif
}
void DataReceive::sendBinaryy(QByteArray arr)
{
    QTime tx_time = QTime::currentTime();
    qint64 send_len;
#ifdef DEBUG_EN
    qDebug()<<"Socket send data time --"<<tx_time.toString();
    qDebug()<<"Utf8-发送数据中.....";
#endif
    send_len = dataRecvWS->sendBinaryMessage(arr);
#ifdef DEBUG_EN
    qDebug()<<"send_len:"<<send_len<<"Byte."<<endl;
#endif
}

