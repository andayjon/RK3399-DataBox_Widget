#ifndef CLOUD_WHISTLE_H
#define CLOUD_WHISTLE_H

#include "tcp_socket.h"
#include "thread.h"
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QMutex>
#include <QReadWriteLock>

class Thread;

class Cloud_whistle : public QObject
{
    Q_OBJECT
public:
    explicit Cloud_whistle(QObject *parent = 0);
    ~Cloud_whistle();
    void conn_toServer();
    Thread* tcp_thread;         //tcp socket监测线程
    tcp_socket *tcp_cli;        //socket客户端

    QString localdevid;         //本地云哨侦测设备ID
    bool online_stat;           //设备在线状态 false-不在线 true-在线
    int md_dev_stat;            //云哨设备状态
    quint8 info_stat = 0;
    QJsonObject md_upMQ_info;   //云哨侦测上传消息
    QString databox_id;         //数据盒ID
    QMutex mutex_md;            //操作锁
    QReadWriteLock m_rwlock;    //读写锁

    QString server_ip;
    int server_port;
private:
    void analysisJson(QByteArray);  //解析云哨侦测数据

    QJsonObject js_cwObj;
    QJsonArray js_uavArry;
    QJsonArray js_uavData;

    QByteArray send_Bytes;      //发送数据
signals:
    void clear_allData();           //清除所有数据
    void clear_uavData();           //清除无人机数据
    void send_data(QByteArray);     //发送数据
    void add_uavData(QByteArray);   //添加无人机数据
    void add_allData();             //添加全部数据
    void sign_sendData();           //发送数据
    void sign_addHeartBeat(QByteArray);       //添加心跳包
public slots:
    void slot_msgFormServer(QByteArray);    //接收服务器消息
    void slot_tcpDisconnect();              //TCP重连
    void slot_add_allData();
    void slot_add_uavData(QByteArray);
    void slot_add_heartBeat(QByteArray);
    void slot_clear_allData();
    void slot_clear_uavData();
};

#endif // CLOUD_WHISTLE_H
