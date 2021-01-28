#include "cloud_whistle.h"
#include <QThread>
#define DEBUG_EN
Cloud_whistle::Cloud_whistle(QObject *parent)
{
    tcp_cli = new tcp_socket(this);

    connect(this, &Cloud_whistle::clear_allData, this, &Cloud_whistle::slot_clear_allData);
    connect(this, &Cloud_whistle::clear_uavData, this, &Cloud_whistle::slot_clear_uavData);
    //connect(this, &Cloud_whistle::add_allData, this, &Cloud_whistle::slot_add_allData);
    //connect(this, &Cloud_whistle::send_data, tcp_cli, &tcp_socket::send_toserver);
    //connect(tcp_cli, SIGNAL(recv_formserver(QByteArray)), this, SLOT(slot_add_uavData(QByteArray)));
    connect(tcp_cli, &tcp_socket::recv_formserver, this, &Cloud_whistle::slot_msgFormServer);
    connect(tcp_cli, &tcp_socket::recv_formserver, this, &Cloud_whistle::slot_add_uavData);
    connect(this, &Cloud_whistle::sign_addHeartBeat, this, &Cloud_whistle::slot_add_uavData);
    connect(tcp_cli, &tcp_socket::onDisConnect, this, &Cloud_whistle::slot_tcpDisconnect);

}
Cloud_whistle::~Cloud_whistle()
{

}
//线程方式连接云哨--暂时未使用
void Cloud_whistle::conn_toServer()
{
    tcp_thread = new Thread(this);
    connect(this, &Cloud_whistle::clear_allData, this, &Cloud_whistle::slot_clear_allData);
    connect(this, &Cloud_whistle::clear_uavData, this, &Cloud_whistle::slot_clear_uavData);
    connect(this, &Cloud_whistle::add_allData, this, &Cloud_whistle::slot_add_allData);
    connect(this, &Cloud_whistle::send_data, tcp_thread, &Thread::send_toserver);

    connect(tcp_thread, &Thread::recv_formserver, this, &Cloud_whistle::slot_msgFormServer);
    connect(tcp_thread, &Thread::recv_formserver, this, &Cloud_whistle::slot_add_uavData);
    connect(tcp_thread, &Thread::offline_server, this, &Cloud_whistle::slot_tcpDisconnect);

    tcp_thread->m_QStrSocketIP = server_ip;
    tcp_thread->m_nSockPort = server_port;
    tcp_thread->startThread(server_ip, server_port);
#ifdef DEBUG_EN
    qDebug()<<tr(">>>Connect to Server: %1 : %2").arg(server_ip).arg(server_port)<<endl;
#endif
}
//解析云哨侦测数据
void Cloud_whistle::analysisJson(QByteArray bytes)
{
#ifdef DEBUG_EN
    qDebug()<<tr(">>>Analysis Receive Cloud Whistle Server Send JsonData: %1 <<<").arg(QString(bytes))<<endl;
#endif
}
//接收服务器消息
void Cloud_whistle::slot_msgFormServer(QByteArray bytes)
{
#ifdef DEBUG_EN
    qDebug()<<tr("--->>>Receive Cloud Whistle Server Send Data: %1").arg(QString(bytes))<<endl;
#endif
    //触发发送信号

}
//Tcp重连
void Cloud_whistle::slot_tcpDisconnect()
{
    online_stat = false;
    localdevid = "";
    md_dev_stat = 2;
    info_stat = 0;
    emit clear_allData();
#ifdef DEBUG_EN
    qDebug()<<tr("--->>>Cloud Whistle Online status = %1 ").arg(online_stat)<<endl;
#endif
}
//添加所有数据
void Cloud_whistle::slot_add_allData()
{

    QJsonObject dev_data;
//    this->m_rwlock.lockForRead();   //2020-5-10
    dev_data.insert("datatype", 13);
    dev_data.insert("localdevid", this->localdevid);
    md_upMQ_info.insert("data", js_uavArry);
    md_upMQ_info.insert("devdata", dev_data);
    md_upMQ_info.insert("sourcetype", 4);
    md_upMQ_info.insert("sourceid", databox_id);
#ifdef DEBUG_EN
//    qDebug()<<tr("本地云哨设备数据包内容:")<<endl;
//    qDebug()<<tr("%1").arg(QString(QJsonDocument(this->md_upMQ_info).toJson()))<<endl;
#endif
    emit clear_uavData();
//    this->m_rwlock.unlock();    //2020-5-10
    emit sign_sendData();
}
//添加无人机数据
void Cloud_whistle::slot_add_uavData(QByteArray bytes)
{
    if(bytes.contains("TCP heartbeat")) {
        online_stat = true;
#ifdef DEBUG_EN
        qDebug()<<tr("--->>>Cloud Whistle:Recevie Server Send Heart Beat!")<<endl;
#endif
    }else {
    #ifdef DEBUG_EN
        qDebug()<<tr("--->>>Cloud Whistle Debug Information<<<---")<<endl;
    #endif
        QJsonParseError jsonError;
        QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
        if(jsonError.error != QJsonParseError::NoError) {
    #ifdef DEBUG_EN
            qDebug()<<QStringLiteral("Cloud Whistle resolve Json failed!");
            qDebug()<<tr("--->>>bytes:%1<<<---").arg(QString(bytes))<<endl;
    #endif
            return;
        } else {
            //添加无人机信息
            if(document.isObject()) {
                QJsonObject j_obj = document.object();
                this->m_rwlock.lockForWrite();      //2020-5-10
                //获取云哨设备ID
                if(j_obj.contains("localdevid")) {
                    QString devid;
                    devid = j_obj.value("localdevid").toString();

                    if(!devid.isEmpty()) {
                        this->localdevid = devid;
    #ifdef DEBUG_EN
                        qDebug()<<tr(">>>Receive Cloud Whistle Send Data localdevid = %1 <<<").arg(this->localdevid)<<endl;
    #endif
                        //this->online_stat = j_obj.value("switch").toBool();
                        //this->md_dev_stat = j_obj.value("stat").toInt();
                    }
                /*
                } else if(j_obj.contains("orderID")) {
                    js_uavArry.append(document.object());
                    //调试添加固定信息  2020-5-8
                    this->localdevid = "0QRDG780030197";
                    this->online_stat = true;
                    this->md_dev_stat = 0;
                */
                } /*else {
    #ifdef DEBUG_EN
                    qDebug()<<tr(">>>Receive Cloud Whistle Invalid Detect The Result!")<<endl;
    #endif
                    this->m_rwlock.unlock();    //2020-5-10
                    return;
                }*/
                if(j_obj.contains("switch")) {
                    this->online_stat = j_obj.value("switch").toBool();
    #ifdef DEBUG_EN
                    qDebug()<<tr(">>>Receive Cloud Whistle Send Data switch = %1 <<<").arg(this->online_stat)<<endl;
    #endif
                }
                if(j_obj.contains("stat")) {
                    this->md_dev_stat = j_obj.value("stat").toInt();
    #ifdef DEBUG_EN
                    qDebug()<<tr(">>>Receive Cloud Whistle Send Data stat = %1 <<<").arg(this->md_dev_stat)<<endl;
    #endif
                }
                //增加无人机信息
                if(j_obj.contains("uav_info")) {
                    QJsonValue j_val;
                    j_val = j_obj.value("uav_info");
                    if(j_val.isArray()) {
    #ifdef DEBUG_EN
                        qDebug()<<tr(">>>Receive Cloud Whistle Send uavinfo QjsonArray! <<<")<<endl;
    #endif
                        QJsonArray j_arr;
                        j_arr = j_obj.value("uav_info").toArray();
                        for(int i = 0; i < j_arr.size(); i++) {
                            if(j_arr.at(i).isObject()) {
                                js_uavArry.append(j_arr.at(i));
    #ifdef DEBUG_EN
                                qDebug()<<tr(">>>Add uavInfo Number: %1 <<<").arg(i+1)<<endl;
    #endif
                            }
                        }
                    }
                    info_stat = 1;
                }
                if(j_obj.contains("Msg Type")) {
                    info_stat = 1;
                }
                this->m_rwlock.unlock();    //2020-5-10

            } else {
                info_stat = 0;
    #ifdef DEBUG_EN
                qDebug()<<tr(">>>Receive Cloud Whistle UAV Information Invalid!<<<")<<endl;
    #endif
                return;
            }
            slot_add_allData();
        }
    }
}
//add heartbeat data
void Cloud_whistle::slot_add_heartBeat(QByteArray bytes)
{

}
//清除所有数据
void Cloud_whistle::slot_clear_allData()
{
    if(!md_upMQ_info.isEmpty()) {
        this->m_rwlock.lockForWrite();  //2020-5-10
        QStringList list_key = md_upMQ_info.keys();
        for(int i = 0; i < md_upMQ_info.size(); i++) {
            md_upMQ_info.remove(list_key.at(i));
        }
#ifdef DEBUG_EN
        qDebug()<<tr("Clear Cloud Whistle AllData done!")<<endl;
#endif
        this->m_rwlock.unlock();    //2020-5-10
        return;
    }
#ifdef DEBUG_EN
    qDebug()<<tr("There is Cloud Whistle no AllData to clear!")<<endl;
#endif
}
//清除无人机数据
void Cloud_whistle::slot_clear_uavData()
{
    if(!js_uavArry.isEmpty()) {
        this->m_rwlock.lockForWrite();  //2020-5-10
        for(int i = 0; i < js_uavArry.size(); i++) {
            js_uavArry.removeAt(i);
        }
#ifdef DEBUG_EN
        qDebug()<<tr("Clear Cloud Whistle UavData done!")<<endl;
#endif
        this->m_rwlock.unlock();    //2020-5-10
        return;
    }
#ifdef DEBUG_EN
    qDebug()<<tr("There is Cloud Whistle no UavData to clear!")<<endl;
#endif
}
