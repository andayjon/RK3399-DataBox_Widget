#include "radar.h"

Radar::Radar(QObject *parent) : QObject(parent)
{
    m_udpSocket = new udp_Socket(this);
    connect(m_udpSocket, &udp_Socket::sig_recvMsg, this, &Radar::slot_udpRecvMsg);
    connect(m_udpSocket, &udp_Socket::sig_sendAccomplish, this, &Radar::slot_sendAccomplish);

    m_timer = new QTimer(this);
    m_timer->setInterval(3000);
    connect(m_timer, &QTimer::timeout, this, &Radar::slot_timerOut);
    connect(this, &Radar::sig_start, this, &Radar::slot_start);
    connect(this, &Radar::sig_sendMsg, this, &Radar::slot_sendMsg);
    connect(this, &Radar::sig_clearJsData, this, &Radar::slot_clearJsData);
    connect(this, &Radar::sig_addJsData, this, &Radar::slot_addJsData);
    connect(this, &Radar::sig_addUavData, this, &Radar::slot_addUavInfo);
    connect(this, &Radar::sig_clearUavData, this, &Radar::slot_clearUavInfo);
}
Radar::~Radar()
{

}
//
void Radar::slot_start()
{
    emit m_udpSocket->sig_threadStart(this->port);
    m_timer->start();
}
void Radar::slot_sendMsg(QByteArray msg, int port)
{
    emit m_udpSocket->sig_sendMsg(msg, port);
}
void Radar::slot_udpRecvMsg(QByteArray msg)
{
    emit this->sig_recvMsg(msg);
    analysRadarData(msg);
}

//解析雷达数据
void Radar::analysRadarData(QByteArray msg)
{
    if('\xcc' == msg.at(0)) {
        qDebug()<< "------test1------" <<endl;
    }else {
        dev_stat = false;
    }
    if('\xcc' == msg.at(0) && msg.at(1) == '\x55' && msg.at(2) == '\x55' && msg.at(3) == '\xcc') {
        dev_stat = true;
        qDebug() << "Receive Radar Track Packet!" << endl;
        qDebug()<< "Receive Radar Send Track Infomation Data:"<<endl;
        uchar *ch;
        ch = (uchar*)(msg.mid(4,2).data());
//        qDebug() << "len_ch[0] = " << len_ch[0] <<"len_ch[1] = " << len_ch[1] <<endl;
        quint16 len_litt = ch[0]*256+ch[1];
//        qDebug() << "len_litt = " << QString::number(len_litt) <<"len_big = " << QString::number(len_big) <<endl;
//        qDebug() << "Receive Radar Send Data Len = " << QString::number(len_big) <<endl;
//        quint16 data_len = len_big;
        quint16 msg_len = qToBigEndian(len_litt);
        qDebug()<< "msg_len = " << msg_len <<endl;
        ch = (uchar*)(msg.mid(6,2).data());
        quint16 id_litt = ch[0]*256+ch[1];
        m_radar.id = qToBigEndian(id_litt);
        md_dev_id = m_radar.id;
        qDebug()<< "Radar ID = " << m_radar.id <<endl;
        ch = (uchar*)(msg.mid(8,8).data());
        quint64 utc_litt = msg.at(15)*qPow(2,56) + msg.at(14)*qPow(2,48) + msg.at(13)*qPow(2,40) + msg.at(12)*qPow(2,32) + msg.at(11)*qPow(2,24) + msg.at(10)*qPow(2,16) + msg.at(9)*qPow(2,8) +msg.at(8);
        m_radar.utc =utc_litt;
        qDebug()<< "Radar UTC = " << m_radar.utc <<endl;
        quint8 pattern = (quint8)(msg.at(58));
        if(pattern == 0) {  //发送单条航迹模式
            if(msg_len == 64) {
                m_radar.pattern = msg.at(19)*256 + msg.at(18);
                if(m_radar.pattern == 1) {    //确定目标
                    Target_stc curr_tar;
                    curr_tar.number = msg.at(21)*256 + msg.at(20);
                    curr_tar.target_count = msg.at(23)*256 + msg.at(22);
                    curr_tar.C_angle = msg.at(25)*256 + msg.at(24);
                    curr_tar.P_angle = msg.at(27)*256 + msg.at(26);
                    curr_tar.dist = msg.at(31)*qPow(2, 24) + msg.at(30)*qPow(2, 16) + msg.at(29)*qPow(2, 8) + msg.at(28);
                    curr_tar.speed = (quint16)((quint16)(msg.at(33)*256) +((quint8)msg.at(32)));
                    qint16 speed = msg.at(35)*256 + msg.at(34);
                    if(speed < (65536/2)) {
                        curr_tar.rv_speed = (quint16)(msg.at(35)*256) + ((quint8)(msg.at(34)));
                    }else {
                        curr_tar.rv_speed = msg.at(35)*256 + msg.at(34);
                    }
                    curr_tar.pnp = msg.mid(36,2).toHex().toShort();
                    curr_tar.pnp = qToBigEndian(curr_tar.pnp);
                    curr_tar.pnp = msg.at(37)*256 + msg.at(36);
                    qDebug()<< QString("Target Information: number = %1, target_count = %2, C_angle = %3, P_angle = %4, dist = %5, rv_speed = %6, speed = %7, pnp = %8 ").arg(curr_tar.number).arg(curr_tar.target_count).arg(curr_tar.C_angle).arg(curr_tar.P_angle).arg(curr_tar.dist).arg(curr_tar.rv_speed).arg(curr_tar.speed).arg(curr_tar.pnp) <<endl;

                    quint64 latitude = msg.mid(38,8).toHex().toLongLong(0,16);
                    latitude = qToBigEndian(latitude);
                    curr_tar.lat = *(double*)&latitude;
                    qDebug()<< QString("target lat = %1").arg(curr_tar.lat, 0, 'f', 8)<<endl;
                    quint64 longitude = msg.mid(46,8).toHex().toULongLong(0,16);
                    longitude = qToBigEndian(longitude);
                    curr_tar.lon = *(double*)&longitude;
                    qDebug()<< QString("target lon = %1").arg(curr_tar.lon, 0, 'f', 8) <<endl;
                    quint32 altitude = msg.mid(54,4).toHex().toUInt(0,16);
                    qDebug()<< QString("altitude(little) = %1").arg(altitude) <<endl;
                    altitude = qToBigEndian(altitude);
                    qDebug()<< QString("altitude(big) = %1").arg(altitude) <<endl;
                    curr_tar.altitude = *(float*)&altitude;
                    qDebug()<< QString("target altitude = %1").arg(curr_tar.altitude, 0, 'f', 6) <<endl;
                    QVector<Target_stc>::iterator iter;

                    bool tmp_flag = true;
                    for(iter = m_radar.m_vct.begin(); iter != m_radar.m_vct.end(); ++iter) {
                        if(iter->number != curr_tar.number) {
                            tmp_flag = false;
                        }else {
                            tmp_flag = true;
                            *iter = curr_tar;   //替换相同Number信息
                            qDebug()<< QString("Replace The Same Number Information Done.") <<endl;
                        }
                    }
                    if(!tmp_flag || m_radar.m_vct.isEmpty()) {
                        m_radar.m_vct.push_back(curr_tar);  //添加新的航迹信息
                        qDebug()<< QString("Add New Track Information Done.") <<endl;
                    }
                }else if(m_radar.pattern == 2) {  //目标丢弃
                    Target_stc curr_tar;
                    curr_tar.number = msg.at(21)*256 + msg.at(20);
//                    QVector<Target_stc>::iterator iter;
                    if(!m_radar.m_vct.isEmpty()) {
                        for(int i = 0; i < m_radar.m_vct.size(); ++i) {
                            if(m_radar.m_vct.at(i).number == curr_tar.number) {
                                m_radar.m_vct.removeAt(i);
                                qDebug()<< QString("Remove Number = %1 Target Infomation.").arg(curr_tar.number) <<endl;
                            }else {

                            }
                        }
                    }
                }else if(m_radar.pattern == 0) {     //起批未确定

                }else {

                }
                qDebug()<< QString("Radar Vector Count = %1").arg( m_radar.m_vct.size()) <<endl;
            }
        }else if(pattern == 0) {    //发送所有航迹模式

        }else {

        }
    }
}

//添加无人机信息
void Radar::slot_addUavInfo()
{
    QVector<Target_stc>::iterator iter;
    for(iter = m_radar.m_vct.begin(); iter < m_radar.m_vct.end(); ++iter) {

        if(iter->lat != 0.0 && iter->lon != 0.0) {
            QJsonObject uav_info;
            QJsonValue tmp_val;
            tmp_val = QJsonValue(m_radar.utc);
            uav_info.insert("date", tmp_val);
            uav_info.insert("receiveDate", tmp_val);
            uav_info.insert("altitude", QJsonValue(iter->altitude));
            uav_info.insert("orderID", QJsonValue("RD" + md_dev_model + QString::number(md_dev_id)));
            uav_info.insert("oid", QJsonValue("RD" + md_dev_model + conn_ip));
            uav_info.insert("userName", QJsonValue(""));
            uav_info.insert("speed", QJsonValue(iter->speed/10));
            uav_info.insert("platform", QJsonValue("android"));
            uav_info.insert("yaw", QJsonValue(iter->C_angle/100));
            uav_info.insert("se", QJsonValue("Null"));
            uav_info.insert("droneType", QJsonValue("Null"));
            uav_info.insert("droneID", QJsonValue("RD" + md_dev_model + QString::number(md_dev_id) + QString::number(iter->number)));
            uav_info.insert("contact", QJsonValue(""));
            QJsonObject tmp_obj;
            tmp_obj.insert("latitude", QJsonValue(iter->lat));
            tmp_obj.insert("longitude", QJsonValue(iter->lon));
            uav_info.insert("location", tmp_obj);
            uav_info.insert("flightTime", QJsonValue(0));
            uav_info.insert("altitudeType", 0);
            QJsonArray tmp_arr;
            tmp_arr.append("full");
            uav_info.insert("tasks", tmp_arr);

            js_ArrData.append(uav_info);
        }
    }
    emit sig_addJsData();
}

//添加Json数据
void Radar::slot_addJsData()
{
    QJsonObject dev_obj;

    dev_obj.insert("datatype", 13);
    dev_obj.insert("localdevid", m_radar.id);
    md_upMQ_info.insert("devdata", dev_obj);
    md_upMQ_info.insert("sourcetype", 1);
    md_upMQ_info.insert("sourceid", databox_id);
    md_upMQ_info.insert("data", js_ArrData);

    emit sig_clearUavData();

}
//清除Json数据
void Radar::slot_clearJsData()
{

    if(!md_upMQ_info.isEmpty()) {
        QStringList list_key = md_upMQ_info.keys();
        for(int i = 0;i < md_upMQ_info.size(); i++) {
            md_upMQ_info.remove(list_key.at(i));
        }
        qDebug()<<tr(">>>Radar:Clear AllData done!")<<endl;
        return;
    }else {
        qDebug()<<tr(">>>Radar:There is no AllData to clear!")<<endl;
    }

}

//清除UAV数据
void Radar::slot_clearUavInfo()
{
    if(m_radar.m_vct.size() != 0) {
        QVector<Target_stc>::iterator iter;
        for(int i = 0; i < m_radar.m_vct.size(); ++i) {
            m_radar.m_vct.remove(i);
        }
    }
    if(!js_ArrData.isEmpty()) {
        for(int i = 0;i < js_ArrData.size(); i++) {
            js_ArrData.removeAt(i);
        }
        qDebug()<<"Clear UavData done!"<<endl;
        QJsonDocument doc_arr;
        doc_arr.setArray(js_ArrData);
        qDebug()<<"清除本地雷达侦测无人机信息.."<<endl;
        /*qDebug()<<"UAV_Information:"+QString(doc_arr.toJson())<<endl;*/     //打印侦测无人机信息
        return;
    }else {
        qDebug()<<"There is no UavData to clear!"<<endl;
    }
}
//UDP发送完成槽
void Radar::slot_sendAccomplish()
{
    emit this->sig_sendAccomplish();
}

//
void Radar::slot_timerOut()
{
    if(dev_stat) {
        online_stat = true;
        emit sig_addUavData();
        emit sig_sendJsData();
    } else {
        online_stat = false;
    }
}
//小端转大端
void Radar::littTobig(uchar *m_char, quint8 len, quint64 ret)
{
    quint64 ch_litt;
    quint64 ch_big;

    switch (len) {
    case 16:
        ch_litt = m_char[0]*2^8+m_char[1];
        break;
    case 32:
        ch_litt = m_char[0]*2^24+m_char[1]*2^16+m_char[2]*2^8+m_char[3];
        break;
    case 64:
        ch_litt = m_char[0]*2^56+m_char[1]*2^48+m_char[2]*2^40+m_char[3]*2^32+m_char[4]*2^24+m_char[5]*2^16+m_char[6]*2^8+m_char[7];
        break;
    default:
        break;
    }
    ch_big = qToBigEndian(ch_litt);

    ret = ch_big;
}
