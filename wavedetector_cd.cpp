#include "wavedetector_cd.h"
#define DEBUG_EN
waveDetector_cd::waveDetector_cd(QObject *parent) : QObject(parent)
{
    m_zeroMqcli = new zeromqCli(this);
    connect(m_zeroMqcli, &zeromqCli::sig_recvMsg, this, &waveDetector_cd::slot_recvMsg);
    connect(this, &waveDetector_cd::sig_confDevCmd, this, &waveDetector_cd::slot_confDev);
    connect(this, &waveDetector_cd::sig_addUavInfo, this, &waveDetector_cd::slot_addUavInfo);
    connect(this, &waveDetector_cd::sig_addUpInfo, this, &waveDetector_cd::slot_addUpInfo);
    connect(this, &waveDetector_cd::sig_clearUavInfo, this, &waveDetector_cd::slot_clearUavInfo);
    connect(this, &waveDetector_cd::sig_clearUpInfo, this, &waveDetector_cd::slot_clearUpInfo);

    m_sendTim = new QTimer(this);
    connect(m_sendTim, &QTimer::timeout, this, &waveDetector_cd::slot_sendTimertimout);
    m_sendTim->setInterval(3000);

}

waveDetector_cd::~waveDetector_cd()
{

}

void waveDetector_cd::dev_sendCmd(QByteArray cmd)
{
    if(!cmd.isEmpty()) {
        emit sig_confDevCmd(cmd);
    }
}
void waveDetector_cd::dev_Init()
{
    QJsonObject cmd;
    cmd.insert("cmd", "startup");
    cmd.insert("interval", 5000);   //上报周期ms
    cmd.insert("train", false);     //关闭录制环境
    QJsonArray mark;
    cmd.insert("mark_signal", mark);    //标记信号
    QByteArray cmd_arr = QString(QJsonDocument(cmd).toJson()).toUtf8();
    m_sendTim->start();
    dev_sendCmd(cmd_arr);
}

void waveDetector_cd::dev_close()
{
    QJsonObject cmd;
    cmd.insert("cmd", "shutdown");
//    cmd.insert("train", false);
//    cmd.insert("interval", 1000);
//    QJsonArray mark;
//    cmd.insert("mark_signal", mark);

    QByteArray cmd_arr = QString(QJsonDocument(cmd).toJson()).toUtf8();
    dev_sendCmd(cmd_arr);
    m_sendTim->stop();
}

void waveDetector_cd::dev_start()
{
    QJsonObject cmd;
    cmd.insert("cmd", "startup");
    QByteArray cmd_arr = QString(QJsonDocument(cmd).toJson()).toUtf8();
    dev_sendCmd(cmd_arr);
    m_sendTim->start();
}
void waveDetector_cd::slot_confDev(QByteArray cmd)
{
    m_zeroMqcli->sig_sendMsg(cmd);
}
void waveDetector_cd::slot_recvMsg(QByteArray msg)
{
    QDateTime curr_tim = QDateTime::currentDateTime();
#ifdef DEBUG_EN
    qDebug()<< tr("-->>(%1)waveDetector:Recive Device Send Data:%2").arg(QString(curr_tim.toString())).arg(QString(msg))<<endl;
#endif
    if(subdev_type == 0) {
        analysisJsonPack(msg);
    }
}

void waveDetector_cd::analysisJsonPack(QByteArray m_bytes)
{
    QJsonParseError jsonError;
    QByteArray x_bytes;
    //去除json包头数据
    for(quint16 i = 0; i < m_bytes.size(); i++) {
        if(m_bytes.at(i) != '{') {
            m_bytes[i] = 0x20;
        } else {
            break;
        }
    }
    x_bytes = m_bytes.trimmed();
    QJsonDocument document = QJsonDocument::fromJson(x_bytes,&jsonError);
    if(jsonError.error != QJsonParseError::NoError){
#ifdef DEBUG_EN
        qDebug()<<QStringLiteral("Wave Detector resolve Json failed");
#endif
        return;
    }else {
        if(document.isObject()) {
            QJsonObject obj = document.object();
            if(obj.contains("device_info")) {
                QJsonValue tmp_val = obj.value("device_info");
                QJsonObject dev_obj = tmp_val.toObject();
                if(dev_obj.contains("dev_id")) {
                    dev_repinfo.dev_id = dev_obj.value("dev_id").toInt();
                }
                if(dev_obj.contains("location")) {
                    QJsonValue loc_val = dev_obj.value("location");
                    QJsonObject loc_obj = loc_val.toObject();
                    if(loc_obj.contains("altitude")) {
                        dev_repinfo.dev_locat.alt = loc_obj.value("altitude").toDouble();
                    }
                    if(loc_obj.contains("latitude")) {
                        dev_repinfo.dev_locat.lat = loc_obj.value("latitude").toDouble();
                    }
                    if(loc_obj.contains("longitude")) {
                        dev_repinfo.dev_locat.lon = loc_obj.value("longitude").toDouble();
                    }
                    if(loc_obj.contains("satellites")) {
                        dev_repinfo.dev_locat.state = loc_obj.value("satellites").toInt();
                    }
                    if(loc_obj.contains("status")) {
                        dev_repinfo.dev_locat.status = loc_obj.value("status").toInt();
                    }
                }
                if(dev_obj.contains("status")) {
                    dev_repinfo.dev_stat = dev_obj.value("status").toString();
                }
                if(dev_obj.contains("switch_ant")) {
                    dev_repinfo.ant_switch = dev_obj.value("switch_ant").toBool();
                }
            }
            if(obj.contains("uav_list")) {
                if(obj.value("uav_list").isArray()) {
                    QJsonArray uav_arr = obj.value("uav_list").toArray();
                    for(int i = 0; i < uav_arr.size(); i++) {
                        QJsonObject uav_obj = uav_arr[i].toObject();
                        UavInfo_st uavinfo;
                        if(uav_obj.contains("id")) {
                            uavinfo.id = QString::number(uav_obj.value("id").toDouble(), 'f', 0).toLongLong();
                        }
                        if(uav_obj.contains("name")) {
                            uavinfo.name = uav_obj.value("name").toString();
                        }
                        if(uav_obj.contains("ch_id")) {
                            uavinfo.ch_id = (quint32)(uav_obj.value("ch_id").toInt());
                        }
                        if(uav_obj.contains("frequency")) {
                            uavinfo.freq = uav_obj.value("frequency").toDouble();
                        }
                        if(uav_obj.contains("confidence")) {
                            uavinfo.conf_d = uav_obj.value("confidence").toDouble();
                        }
                        if(uav_obj.contains("type")) {
                            uavinfo.type = uav_obj.value("type").toString();
                        }
                        if(uav_obj.contains("is_dji")) {
                            uavinfo.dji_flg = uav_obj.value("is_dji").toBool();
                        }
                        QDateTime curr_time = QDateTime::currentDateTime();
                        uavinfo.utc = curr_time.toTime_t();
                        /* */
                        QVector<UavInfo_st>::iterator iter;
                        bool tmp_flag = false;
                        for(iter = dev_repinfo.uav_vec.begin(); iter != dev_repinfo.uav_vec.end(); ++iter) {
                            if(iter->id != uavinfo.id) {
                                tmp_flag = false;
                            } else {
                                tmp_flag = true;
                                *iter = uavinfo;
#ifdef DEBUG_EN
                                qDebug() << QString("-->>Wavedetector CD Replace The Same Number Information Done!") <<endl;
#endif
                            }
                        }
                        /* */
                        if(!tmp_flag || dev_repinfo.uav_vec.isEmpty()) {
                            dev_repinfo.uav_vec.push_back(uavinfo);
                        }
                    }
                    emit sig_addUavInfo();
                }
            }
        }
    }
}

void waveDetector_cd::slot_addUavInfo()
{
    QVector<UavInfo_st>::iterator iter;
    QJsonArray js_arr;
    for(iter = dev_repinfo.uav_vec.begin(); iter != dev_repinfo.uav_vec.end(); ++iter) {
        QJsonObject uav_obj;
        float rand = Get_RandNumber();
        float alt = dev_repinfo.dev_locat.alt * (2 + rand);
        uav_obj.insert("altitude", int(alt));
        uav_obj.insert("altitudeType", 0);
        uav_obj.insert("contact", "");
        int utc = iter->utc;
        uav_obj.insert("date", utc);
        //QString droneid = QString::number(iter->id);
        QString droneid = tr("%1").arg(iter->id);
//        qDebug()<<tr("iter.id = %1").arg(iter->id)<<endl;
        uav_obj.insert("droneID", droneid);
        uav_obj.insert("flightTime", 0);
        QString oid = iter->type + "-" +m_zeroMqcli->m_mqparam->ip_server + "-" + QString::number(dev_repinfo.dev_id);
        QJsonObject js_devGps;
        //目标坐标转换
        Loc_st cen_loc, tar_loc;
        if(dev_lat != 0.0 && dev_lon != 0.0){
            cen_loc.lat = dev_lat;
            cen_loc.lon = dev_lon;
        }else if(dev_repinfo.dev_locat.lat != 0.0 && dev_repinfo.dev_locat.lon != 0.0) {
            cen_loc.lat = dev_repinfo.dev_locat.lat;
            cen_loc.lon = dev_repinfo.dev_locat.lon;
        }else {
            cen_loc.lat = 42.202542;
            cen_loc.lon = 85.760338;
        }

        tar_loc = ConvertDistanceToLogLat(cen_loc, int(rand*360), double(rand*3000));
        js_devGps.insert("latitude", tar_loc.lat);
        js_devGps.insert("longitude", tar_loc.lon);
        uav_obj.insert("location", js_devGps);
        uav_obj.insert("oid", oid);
        uav_obj.insert("orderID", "");
        uav_obj.insert("platform", "android");
        uav_obj.insert("receiveDate", int(iter->utc));
        uav_obj.insert("se", iter->type);
        uav_obj.insert("speed", 0.0);
        QJsonArray tasks;
        tasks.append("full");
        uav_obj.insert("tasks", tasks);
        uav_obj.insert("userName", iter->name);
        uav_obj.insert("yaw", 0.0);

        js_arr.append(uav_obj);
    }
    if(!dev_repinfo.uav_vec.isEmpty()) {
        dev_repinfo.uav_vec.clear();
    }
    js_obj.insert("data", js_arr);
    emit sig_addUpInfo();
}

float waveDetector_cd::Get_RandNumber(void)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int i_num = qrand()%100;

    float f_num = float(i_num)/100;

    qDebug()<<tr("Get Rand Number:i_num = %1, f_num = %2").arg(QString::number(i_num)).arg(QString(QString::number(double(f_num), 'f', 6)))<<endl;
    return f_num;
}

void waveDetector_cd::slot_addUpInfo()
{
    QJsonObject devdata;
    devdata.insert("datatype", 13);
    devdata.insert("localdevid", QString::number(dev_repinfo.dev_id));

    js_obj.insert("devdata", devdata);
    js_obj.insert("sourceid", databox_id);
    js_obj.insert("sourcetype", 4);

}
void waveDetector_cd::slot_sendTimertimout()
{
    emit sig_sendUpInfo();
}
void waveDetector_cd::slot_clearUpInfo()
{
    if(!js_obj.isEmpty()) {
        QStringList list_key = js_obj.keys();
        for(int i = 0; i < js_obj.size(); i++) {
            if(list_key.at(i) == "data") {
                js_obj.remove(list_key.at(i));
                return;
            }
        }
#ifdef DEBUG_EN
        qDebug()<<tr(">>>zmq WaveDetector:Clear AllData done!")<<endl;
#endif
        return;
    }
#ifdef DEBUG_EN
    qDebug()<<tr(">>>zmq WaveDetector:There is no AllData to clear!")<<endl;
#endif
}
void waveDetector_cd::slot_clearUavInfo()
{
    if(!js_arr.isEmpty()) {
        for(int i = 0; i < js_arr.size(); i++) {
            js_arr.removeAt(i);
        }
#ifdef DEBUG_EN
        qDebug()<<">>>zmq WaveDetector:Clear UavData done!"<<endl;
#endif
        return;
    }
#ifdef DEBUG_EN
    qDebug()<<">>>zmq WaveDetector:There is no UavData to clear!"<<endl;
#endif
}
Loc_st waveDetector_cd::ConvertDistanceToLogLat(Loc_st location, int azimuth, double distance)
{
    Loc_st tmp_loc;
    if(location.lat != 0.0 && location.lon != 0.0 && distance > 0) {
        tmp_loc.lon = location.lon+distance*qSin(azimuth*M_PI/180)*180/(M_PI*6371229*qCos(location.lat*M_PI/180));
        tmp_loc.lat = location.lat+distance*qCos(azimuth*M_PI/180)/(M_PI*6371229/180);
    }else {
        tmp_loc.lon = location.lon;
        tmp_loc.lat = location.lat;
    }

    return tmp_loc;
}
