#include "wavedetector_dev.h"
#include <QThread>

WaveDetector_dev::WaveDetector_dev(QObject *parent):QObject(parent)
{
    tcp_cli = new tcp_socket(this);

    connect(this, &WaveDetector_dev::clear_allData, this, &WaveDetector_dev::slot_clear_allData);
    connect(this, &WaveDetector_dev::clear_uavData, this, &WaveDetector_dev::slot_clear_uavData);
    connect(this, &WaveDetector_dev::add_allData, this, &WaveDetector_dev::slot_add_allData);
    connect(this,&WaveDetector_dev::send_data, tcp_cli, &tcp_socket::send_toserver);
    connect(tcp_cli, &tcp_socket::conn_toServerOk, this, &WaveDetector_dev::slot_tcpConnect);
    connect(tcp_cli, &tcp_socket::recv_formserver, this, &WaveDetector_dev::slot_msgFormServer);
    connect(tcp_cli, &tcp_socket::onDisConnect, this, &WaveDetector_dev::slot_tcpDisConnect);

    tim_send = new QTimer(this);
    connect(tim_send, &QTimer::timeout, this, &WaveDetector_dev::slot_add_allData);
}

WaveDetector_dev::~WaveDetector_dev()
{

}

//解析侦测数据
void WaveDetector_dev::analysisJson(QByteArray m_bytes)
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

    //qDebug()<<tr("整理后的数据:")<<x_bytes<<endl;
    QJsonDocument document = QJsonDocument::fromJson(x_bytes,&jsonError);
    if(jsonError.error != QJsonParseError::NoError){
        qDebug()<<QStringLiteral("Wave Detector resolve Json failed");
        return;
    } else {
        //解析Json
        if(document.isObject()) {
            QJsonObject obj = document.object();
            int msg_type;
            if(obj.contains("type")) {
                QJsonValue tmp_val = obj.value("type");
                msg_type = tmp_val.toInt();
                if(msg_type==5) {
                    //接收到无人机设备信息
                    if(obj.contains("data")) {
                        QJsonValue tmp_val = obj.value("data");
                        QJsonObject data = tmp_val.toObject();
                        if(data.contains("time_stamp")) {
                            QJsonValue date = data.value("time_stamp");
                            js_ObjData.insert("date",date);
                            js_ObjData.insert("receiveDate",date);
                        }
                        //插入se
                        QJsonValue type_name;   //无人机型号
                        if(data.contains("type_name")) {
                            type_name = data.value("type_name");
                        }
                        js_ObjData.insert("se",type_name);
                        QJsonValue uav_code;    //无人机品牌型号名对应索引
                        int u_code;
                        if(data.contains("uav_code")) {
                            uav_code = data.value("uav_code");
                            u_code = uav_code.toInt();
                        }
                        QJsonValue mac_addr;    //MAC地址
                        if(data.contains("mac_addr")) {
                            mac_addr = data.value("mac_addr");
                        }
                        QJsonValue brand_name;  //无人机品牌名
                        if(data.contains("brand_name")) {
                            brand_name = data.value("brand_name");
                        } else {
                            brand_name = "";
                        }

                        //插入oid brand_name+type_name+mac+uav_code
                        QJsonValue oid = type_name.toString()+"-"+mac_addr.toString().remove(":")+"-"+QString::number(u_code);
                        localdevid = oid.toString();
                        js_ObjData.insert("oid",oid);
                        //插入speed
                        QJsonValue uav_speed;       //速度
                        if(data.contains("speed")) {
                            uav_speed = data.value("speed");
                        } else {
                            uav_speed = 0;
                        }
                        js_ObjData.insert("speed",uav_speed);
                        //插入altitude
                        QJsonValue uav_altitude;
                        if(data.contains("altitude")) {
                            uav_altitude = data.value("altitude");
                        } else {
                            uav_altitude = 0;
                        }
                        js_ObjData.insert("altitude",uav_altitude);
                        //插入userName
                        QJsonValue user_name;   //
                        if(data.contains("userName")) {
                            user_name = data.value("userName");
                        } else {
                            user_name = "";
                        }
                        js_ObjData.insert("userName",user_name);
                        //插入orderID 厂家生产批次号
                        QJsonValue orderID;
                        if(data.contains("orderID")) {
                            orderID = data.value("orderID");
                        } else {
                            orderID = "";
                        }
                        js_ObjData.insert("orderID",orderID);
                        //插入platform
                        QJsonValue platform;
                        if(data.contains("platform")) {
                            platform = data.value("platform");
                        } else {
                            platform = "android";
                        }
                        js_ObjData.insert("platform",platform);
                        //插入droneType
                        QJsonValue droneType;
                        if(data.contains("droneType")) {
                            droneType = data.value("droneType");
                        } else {
                            droneType = "";
                        }
                        js_ObjData.insert("droneType",droneType);
                        //插入location
                        QJsonObject location;
                        //插入lat
                        QJsonValue uav_lat;
                        if(data.contains("latitude")) {
                            uav_lat = data.value("latitude");

                        } else {
                            uav_lat = "";
                        }
                        location.insert("latitude",uav_lat);
                        //插入long
                        QJsonValue uav_long;
                        if(data.contains("longitude")) {
                            uav_long = data.value("longitude");

                        } else {
                            uav_long = "";
                        }
                        location.insert("longitude",uav_long);
                        js_ObjData.insert("location",location);
                        //插入flightTime
                        QJsonValue flightTime;
                        if(data.contains("flightTime")) {
                            flightTime = data.value("flightTime");
                        } else {
                            flightTime = "";
                        }
                        js_ObjData.insert("flightTime",flightTime);
                        //插入altitudeType
                        QJsonValue altitudeType;
                        if(data.contains("altitudeType")) {
                            altitudeType = data.value("altitudeType");
                        } else {
                            altitudeType = 0;
                        }
                        js_ObjData.insert("altitudeType",altitudeType);
                        //插入yaw
                        QJsonValue yaw;
                        if(data.contains("yaw")) {
                            yaw = data.value("yaw");
                        } else {
                            yaw = 0;
                        }
                        js_ObjData.insert("yaw",yaw);
                        QJsonArray tasks;
                        tasks.append("full");
                        js_ObjData.insert("tasks",tasks);
                        QJsonValue droneID;
                        droneID = "";
                        js_ObjData.insert("droneID",droneID);
                        QJsonValue contact;
                        contact = "";
                        js_ObjData.insert("contact",contact);
                    }
                    js_ArrData.append(js_ObjData);
                    QJsonDocument doc_arr;
                    doc_arr.setArray(js_ArrData);
                    //qDebug()<<"UAV_Information:"+QString(doc_arr.toJson())<<endl;     //打印侦测无人机信息

                }
                if (msg_type == 2) {
                    //接收到心跳信息
                    //emit clear_allData();
                    if(obj.contains("msg_level")) {
                        QJsonValue msg_lev = obj.value("msg_level");
                        int msg_level = msg_lev.toInt();
                        online_stat = true;    //设备在线
                        if(msg_level == 0) {   //设备正常
                            md_dev_stat = 0;
                            double dev_lat;
                            double dev_long;

                            if(obj.contains("device_id")) {
                                QJsonValue tmp_val = obj.value("device_id");
                                md_dev_id = tmp_val.toInt();
                                qDebug()<<"md_dev_id="<<md_dev_id<<endl;
                            }
                            if(obj.contains("gps_state")) {
                                QJsonValue tmp_val = obj.value("gps_state");
                                bool gps_stat = tmp_val.toBool();
                                if(gps_stat == true) {
                                    //获取设备经纬度
                                    if(obj.contains("latitude")) {
                                        QJsonValue tmp_val = obj.value("latitude");
                                        qDebug()<<"latitude="<<tmp_val.toString()<<endl;
                                        dev_lat = tmp_val.toDouble();
                                    }
                                    if(obj.contains("longitude")) {
                                        QJsonValue tmp_val = obj.value("longitude");
                                        qDebug()<<"longitude"<<tmp_val.toString()<<endl;
                                        dev_long = tmp_val.toDouble();
                                    }
                                } else {
                                    //获取设备经纬度
                                    if(obj.contains("latitude")) {
                                        QJsonValue tmp_val = obj.value("latitude");
                                        qDebug()<<"latitude="<<tmp_val.toString()<<endl;
                                        dev_lat = tmp_val.toDouble();
                                    }
                                    if(obj.contains("longitude")) {
                                        QJsonValue tmp_val = obj.value("longitude");
                                        qDebug()<<"longitude"<<tmp_val.toString()<<endl;
                                        dev_long = tmp_val.toDouble();
                                    }
                                }

                            }
                            QJsonObject devdata;
                            devdata.insert("longitude",dev_long);
                            devdata.insert("latitude",dev_lat);
                            devdata.insert("datatype", 14);
                            localdevid = "CS108Q"+QString::number(md_dev_id);
                            devdata.insert("localdevid",localdevid);
                            md_upMQ_info.insert("devdata",devdata);

                        } else if (msg_level == 1) {    //设备警告
                            md_dev_stat = 1;
                        } else if (msg_level == 2) {    //设备错误
                            md_dev_stat = 2;
                        }
                    }
                }
            }
        }
    }
}

//微波侦测设备信息
void WaveDetector_dev::analysisJsonPack(QByteArray m_bytes)
{
    QJsonParseError jsonError;
    QByteArray x_bytes;
    quint8 uav_count = 0;
    //去除json包头数据
    for(quint16 i = 0; i < m_bytes.size(); i++) {
        if(m_bytes.at(i) != '{') {
            m_bytes[i] = 0x20;
        } else {
            break;
        }
    }
    x_bytes = m_bytes.trimmed();

    //qDebug()<<tr("整理后的数据:")<<x_bytes<<endl;
    QJsonDocument document = QJsonDocument::fromJson(x_bytes,&jsonError);
    if(jsonError.error != QJsonParseError::NoError){
        qDebug()<<QStringLiteral("Wave Detector resolve Json failed");
        return;
    } else {
        //解析Json
        if(document.isObject()) {
            QJsonObject obj = document.object();
            int msg_type;
            if(obj.contains("dev_type")) {
                QJsonValue tmp_val = obj.value("dev_type");
                msg_type = tmp_val.toInt();
                if(msg_type==14) {
                    //接收到无人机设备信息
                    if(obj.contains("uav_info")) {
                        QJsonValue tmp_val = obj.value("uav_info");
                        if(tmp_val.isArray()) {
                        /* 无人机信息解析 */
                            QJsonArray arr_uav = tmp_val.toArray();
                            int nSize = arr_uav.size();
                            uav_count = nSize;
                            for(int i = 0; i < nSize; i++) {
                                QJsonValue uav_val = arr_uav.at(i);
                                if(uav_val.isObject()) {
                                    QJsonObject uav_obj = uav_val.toObject();
                                    if(uav_obj.contains("droneDJFlag")) {
                                        QJsonValue dFlag_val = uav_obj.value("droneDJFlag");
                                        if(dFlag_val.isBool()) {
                                            bool droneFlag = dFlag_val.toBool();
                                            if(!droneFlag && dev_Location.lat != 0.0 && dev_Location.lon != 0.0 && js_ArrData.isEmpty()) {
                                                //判断是大疆无人机
                                                add_AngleData();
#if 0
                                                if(uav_obj.contains("droneID")) {
                                                    QJsonValue droneid_val = uav_obj.value("droneID");
                                                    if(!droneid_val.toString().isEmpty()) {
                                                        //目标设备ID信息非空
                                                        if(uav_obj.contains("brandName")) {
                                                            QJsonValue bdname_val = uav_obj.value("brandName");
//                                                            if(!bdname_val.toString().isEmpty() && bdname_val.toString() != "APA") {
                                                            if(!bdname_val.toString().isEmpty()) {
                                                                //目标设备厂家信息为空
                                                                QJsonValue tmp_val = bdname_val.toString()+droneid_val.toString();
                                                                js_ObjData.insert("droneID", tmp_val);
                                                                js_ObjData.insert("orderID", tmp_val);
                                                                js_ObjData.insert("oid", tmp_val);
                                                                js_ObjData.insert("platform", "android");
                                                                tmp_val = bdname_val.toString();
                                                                js_ObjData.insert("se", tmp_val);
                                                                localdevid = bdname_val.toString()+droneid_val.toString();
                                                            }
                                                            else {
                                                                continue;
                                                            }
                                                        }
                                                    }
                                                }
                                                if(uav_obj.contains("date")) {
                                                    QJsonValue date_val = uav_obj.value("date");
                                                    if(date_val.toInt() > 0) {
                                                        //UTC信息不为0
                                                        QJsonValue tmp_val;
                                                        tmp_val = date_val.toInt();
                                                        js_ObjData.insert("date", tmp_val);
                                                        js_ObjData.insert("receiveDate", tmp_val);
                                                    }
                                                }
                                                if(uav_obj.contains("orderID")) {
                                                    QJsonValue orderid_val = uav_obj.value("orderID");
                                                    if(orderid_val.toString().isEmpty()) {
                                                        //大疆无人机orderID信息为空
                                                    }
                                                }

                                                if(uav_obj.contains("droneModel")) {
                                                    QJsonValue dmodel_val = uav_obj.value("droneModel");
                                                    if(!dmodel_val.toString().isEmpty()) {
                                                        //目标设备型号信息非空
                                                        js_ObjData.insert("droneType", dmodel_val);
                                                    }

                                                }
                                                if(uav_obj.contains("droneType")) {
                                                    QJsonValue dtype_val = uav_obj.value("droneType");
                                                    if(dtype_val.toInt() == 0) {
                                                        //信息为空
                                                    }
                                                }
                                                if(uav_obj.contains("range")) {
                                                    QJsonValue range_val = uav_obj.value("range");
                                                    if(range_val.toInt() > 0 && range_val.toInt() < 3000) {
                                                        //判断无人机距离小于3km
                                                        QJsonObject location_obj;
                                                        Location_st uav_loc;
//                                                        uav_loc = ConvertDistanceToLogLatGD(dev_Location, 90.0, 368.0);
                                                        uav_loc.lon = dev_Location.lon;
                                                        uav_loc.lat = dev_Location.lat;
                                                        location_obj.insert("latitude", uav_loc.lat);
                                                        location_obj.insert("longitude", uav_loc.lon);
                                                        js_ObjData.insert("location", location_obj);
                                                    }else {
                                                        continue;
                                                    }
                                                }
                                                if(uav_obj.contains("altitude")) {
                                                    QJsonValue alt_val = uav_obj.value("altitude");
                                                    if(alt_val.toInt() == 0) {
                                                        //判别无人机高度等于0m
                                                        QJsonValue tmp_val;
                                                        js_ObjData.insert("altitude", 125);
                                                        js_ObjData.insert("speed", 1.5);
                                                        js_ObjData.insert("userName", "");
                                                        js_ObjData.insert("altitudeType", 0);
                                                        js_ObjData.insert("flightTime", 0);
                                                        js_ObjData.insert("yaw",0);
                                                        QJsonArray tasks;
                                                        tasks.append("full");
                                                        js_ObjData.insert("tasks",tasks);
//                                                        QJsonValue contact;
//                                                        contact = "";
                                                        js_ObjData.insert("contact", "");
                                                    }
                                                }
                                                if(uav_obj.contains("speed")) {
                                                    QJsonValue speed_val = uav_obj.value("speed");
                                                    if(speed_val.toInt() == 0) {
                                                        //判别无人机飞行速度等于0m/s
//                                                        double tmp_speed;
//                                                        tmp_speed = qrand();
                                                    }
                                                }
                                                if(uav_obj.contains("altiudeType")) {
                                                    QJsonValue alttype_val = uav_obj.value("altiudeType");
                                                    if(alttype_val.toInt() == 0) {

                                                    }
                                                }
#endif
                                            }else {
                                                continue;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        /* 无人机信息解析 */
                    }
                }
            }
        }
    }
}

void WaveDetector_dev::add_uavtoData()
{

    QJsonValue tmp_val = "APA_7001";
    js_ObjData.insert("droneID", tmp_val);
    js_ObjData.insert("orderID", tmp_val);
    js_ObjData.insert("oid", tmp_val);
    js_ObjData.insert("platform", "android");
    tmp_val = "APA";
    js_ObjData.insert("se", tmp_val);
    js_ObjData.insert("droneType", tmp_val);
    tmp_val = 1606580446;
    js_ObjData.insert("date", tmp_val);
    js_ObjData.insert("receiveDate", tmp_val);

    QJsonObject location_obj;
    LOC_st uav_loc, conv_loc;
//    conv_loc = ConvertGPSToGcj02(dev_Location);`
//    uav_loc = ConvertDistanceToLogLatGD(conv_loc, 90, 500.0);

    QJsonValue tmp_uavloc = dev_Location.lat;
    location_obj.insert("latitude", dev_Location.lat);
    tmp_uavloc = dev_Location.lon;
    location_obj.insert("longitude", "");
    js_ObjData.insert("location", dev_Location.lon);
    js_ObjData.insert("altitude", 125);
    js_ObjData.insert("speed", 1.5);
    js_ObjData.insert("userName", "");
    js_ObjData.insert("altitudeType", 0);
    js_ObjData.insert("flightTime", 0);
    js_ObjData.insert("yaw", 226);
    QJsonArray tasks;
    tasks.append("full");
    js_ObjData.insert("tasks",tasks);
    js_ObjData.insert("contact", "");
    js_ArrData.append(js_ObjData);
    QJsonDocument doc_arr;
    doc_arr.setArray(js_ArrData);
    QJsonObject devdata;
    tmp_val = 13;
    devdata.insert("datatype", tmp_val);
    localdevid = "CDKZY"+QString::number(md_dev_id);
    tmp_val = localdevid;
    devdata.insert("localdevid", tmp_val);
    md_upMQ_info.insert("devdata",devdata);
}

void WaveDetector_dev::add_AngleData()
{
    QJsonValue tmp_val = "APA_7001";
    js_ObjData.insert("droneID", tmp_val);
    js_ObjData.insert("orderID", tmp_val);
    js_ObjData.insert("oid", tmp_val);
    js_ObjData.insert("platform", "android");
    tmp_val = "APA";
    js_ObjData.insert("se", tmp_val);
    js_ObjData.insert("droneType", tmp_val);
    tmp_val = 1606580446;
    js_ObjData.insert("date", tmp_val);
    js_ObjData.insert("receiveDate", tmp_val);

    QJsonObject location_obj;
    LOC_st uav_loc, conv_loc;
//    conv_loc = ConvertGPSToGcj02(dev_Location);`
//    uav_loc = ConvertDistanceToLogLatGD(conv_loc, 90, 500.0);

    uav_loc = ConvertDistanceToLogLat(dev_Location, 190, 490);
    QJsonValue tmp_uavloc = uav_loc.lat;
    location_obj.insert("latitude", tmp_uavloc);
    tmp_uavloc = uav_loc.lon;
    location_obj.insert("longitude", tmp_uavloc);
    js_ObjData.insert("location", location_obj);
    js_ObjData.insert("altitude", 125);
    js_ObjData.insert("speed", 1.5);
    js_ObjData.insert("userName", "");
    js_ObjData.insert("altitudeType", 0);
    js_ObjData.insert("flightTime", 0);
    js_ObjData.insert("yaw", 226);
    QJsonArray tasks;
    tasks.append("full");
    js_ObjData.insert("tasks",tasks);
    js_ObjData.insert("contact", "");
    js_ArrData.append(js_ObjData);
    QJsonDocument doc_arr;
    doc_arr.setArray(js_ArrData);
    QJsonObject devdata;
    tmp_val = 13;
    devdata.insert("datatype", tmp_val);
    localdevid = "CDKZY"+QString::number(md_dev_id);
    tmp_val = localdevid;
    devdata.insert("localdevid", tmp_val);
    md_upMQ_info.insert("devdata",devdata);
}

LOC_st WaveDetector_dev::ConvertDistanceToLogLat(LOC_st location, int azimuth, double distance)
{
    LOC_st tmp_loc;
    if(location.lat != 0.0 && location.lon != 0.0 && distance > 0.0) {
        tmp_loc.lon = location.lon+distance*qSin(azimuth*M_PI/180)*180/(M_PI*6371229*qCos(location.lat*M_PI/180));
        tmp_loc.lat = location.lat+distance*qCos(azimuth*M_PI/180)/(M_PI*6371229/180);
    }else {
        tmp_loc.lon = location.lon;
        tmp_loc.lat = location.lat;
    }

    return tmp_loc;
}

LOC_st WaveDetector_dev::ConvertDistanceToLogLatGD(LOC_st location, double azimuth, double distance)
{
    LOC_st loc_st;
    double raidus = 6371e3;

    double latitude1 = location.lat * (M_PI/180);
    double longitude1 = location.lon * (M_PI/180);
    double br = azimuth * (M_PI/180);

    double latitude2 = asin(sin(latitude1) * cos(distance/raidus) + cos(latitude1) * sin(distance/raidus) * cos(br));
    double longitude2 = longitude1 + atan2(sin(br) * sin(distance/raidus) * cos(latitude1), cos(distance/raidus)-sin(latitude1) * sin(latitude2));

    loc_st.lat = latitude2 * (180/M_PI);
    loc_st.lon = longitude2 * (180/M_PI);
    qDebug()<<tr("-->>Input Location Lon = %1, Lat = %2 <<--").arg(location.lon).arg(location.lat)<<endl;
    qDebug()<<tr("-->>Convert Location Lon = %1, Lat = %2 <<--").arg(loc_st.lon).arg(loc_st.lat)<<endl;
    return loc_st;

}

LOC_st WaveDetector_dev::ConvertGPSToGcj02(LOC_st location_st)
{
    LOC_st ret_loc;

    if (outof_China(location_st.lat, location_st.lon))
    {
        return location_st;
    }
    double dLat = TransformLat(location_st.lon - 105.0, location_st.lat - 35.0);
    double dLon = TransformLon(location_st.lon - 105.0, location_st.lat - 35.0);
    double radLat = location_st.lat / 180.0 * M_PI;
    double magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    double SqrtMagic = sqrt(magic);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * SqrtMagic) * M_PI);
    dLon = (dLon * 180.0) / (a / SqrtMagic * cos(radLat) * M_PI);
    double mgLat = location_st.lat + dLat;
    double mgLon = location_st.lon + dLon;
    ret_loc.lon = mgLon;
    ret_loc.lat = mgLat;
    return ret_loc;

}
/* WGS转换高德地图坐标系 */
bool WaveDetector_dev::outof_China(double lon, double lat)
{
    return(lon<72.004 || lon>137.8374 || lat<0.8293 || lat >55.8271 || false);
}
double WaveDetector_dev::TransformLat(double x, double y)
    {
        double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y
                + 0.2 * sqrt(abs(x));
        ret += (20.0 * sin(6.0 * x * M_PI) + 20.0 * sin(2.0 * x * M_PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(y * M_PI) + 40.0 * sin(y / 3.0 * M_PI)) * 2.0 / 3.0;
        ret += (160.0 * sin(y / 12.0 * M_PI) + 320 * sin(y * M_PI / 30.0)) * 2.0 / 3.0;
        return ret;
    }
double WaveDetector_dev::TransformLon(double x, double y)
    {
        double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1
                * sqrt(abs(x));
        ret += (20.0 * sin(6.0 * x * M_PI) + 20.0 * sin(2.0 * x * M_PI)) * 2.0 / 3.0;
        ret += (20.0 * sin(x * M_PI) + 40.0 * sin(x / 3.0 * M_PI)) * 2.0 / 3.0;
        ret += (150.0 * sin(x / 12.0 * M_PI) + 300.0 * sin(x / 30.0
                * M_PI)) * 2.0 / 3.0;
        return ret;
    }
double WaveDetector_dev::translate_lon(double lon, double lat)
{
    double ret = 300.0 + lon +2.0*lat + 0.1*lon*lon +0.1*lon*lat + 0.1*sqrt(abs(lon));
    ret += (20.0 * sin(6.0*lon*M_PI) + 20.0*sin(2.0*lon*M_PI)) *2.0 / 3.0;
    ret += (20.0 * sin(lon*M_PI) + 40.0*sin(lon/3.0*M_PI))*2.0 /3.0;
    ret += (150 * sin(lon/12.0*M_PI) + 300.0*sin(lon/30.0*M_PI)) *2.0 /3.0;
    return ret;
}
double WaveDetector_dev::translate_lat(double lon, double lat)
{
    double ret = -100 + 2.0*lon + 3.0*lat + 0.2*lat*lat + 0.1*lon*lat + 0.2*sqrt((abs(lon)));
    ret += (20.0 *sin(6.0*lon*M_PI) + 20*sin(2.0*lon*M_PI)) *2.0 /3.0;
    ret += (20.0 *sin(lat*M_PI) + 40.0*sin(lat/3.0*M_PI)) *2.0 /3.0;
    ret += (160.0*sin(lat/12.0*M_PI) + 320.0*sin(lat/30.0 *M_PI)) *2.0 /3.0;
    return ret;
}
LOC_st WaveDetector_dev::transformwgs(double gcj_lon, double gcj_lat)
{
    LOC_st wgs_pos;

    if(outof_China(gcj_lon,gcj_lat))
    {
        wgs_pos.lon = gcj_lon;
        wgs_pos.lat = gcj_lat;
        return wgs_pos;
    }
    else
    {
        double dlat = translate_lat(gcj_lon - 105.0,gcj_lat -35.0);
        double dlon = translate_lon(gcj_lon - 105.0,gcj_lat -35.0);
        double radlat = gcj_lat/180.0 *M_PI;
        double magic = sin(radlat);
        magic = 1 - ee*magic*magic;
        double squrtmagic = sqrt(magic);
        dlon = (dlon *180.0)/(a/squrtmagic*cos(radlat)*M_PI);
        dlat = (dlat *180.0)/((a*(1-ee))/(magic * squrtmagic)*M_PI);
        wgs_pos.lon = gcj_lon - dlon;
        wgs_pos.lat = gcj_lat - dlat;
        return wgs_pos;
    }
}
/* WGS转换高德地图坐标系 */
//接收服务端消息
void WaveDetector_dev::slot_msgFormServer(QByteArray bytes)
{
    qDebug()<<tr("解析微波侦测数据中...")<<endl;
//

    switch (subdev_type) {
    case 0:
        analysisJson(bytes);
        break;
    case 1:
        if(!location_flag) {    // 0--正常数据  1--模拟数据
            analysisJsonPack(bytes);
        }else {
            add_AngleData();    //2020-11-29    模拟数据
        }
        break;
    case 2:
        break;
    default:
        break;
    }
}
//清除所有数据
void WaveDetector_dev::slot_clear_allData()
{
    if(!md_upMQ_info.isEmpty()) {
        QStringList list_key = md_upMQ_info.keys();
        for(int i = 0;i < md_upMQ_info.size(); i++) {
            md_upMQ_info.remove(list_key.at(i));
        }
        qDebug()<<tr(">>>TCP WaveDetector:Clear AllData done!")<<endl;
        return;
    }
    qDebug()<<tr(">>>TCP WaveDetector:There is no AllData to clear!")<<endl;
}
//清除当前无人机信息
void WaveDetector_dev::slot_clear_uavData()
{
    if(!js_ArrData.isEmpty()) {
        for(int i = 0;i < js_ArrData.size(); i++) {
            js_ArrData.removeAt(i);
        }
        qDebug()<<">>>TCP WaveDetector:Clear UavData done!"<<endl;
        QJsonDocument doc_arr;
        doc_arr.setArray(js_ArrData);
        qDebug()<<"本地侦测设备清除无人机信息："<<endl;
        qDebug()<<"UAV_Information:"+QString(doc_arr.toJson())<<endl;     //打印侦测无人机信息
        return;
    }else {
        qDebug()<<">>>TCP WaveDetector:There is no UavData to clear!"<<endl;
    }

}
//添加所有数据
void WaveDetector_dev::slot_add_allData()
{
    tim_send->stop();
    md_upMQ_info.insert("data",js_ArrData);
    md_upMQ_info.insert("sourcetype",1);
    md_upMQ_info.insert("sourceid",databox_id);
    qDebug()<<"本地TCP侦测设备数据包内容："<<endl;
    qDebug()<<QString(QJsonDocument(this->md_upMQ_info).toJson())<<endl;        //打印上传数据
    emit sign_sendData();
    emit clear_uavData();
    tim_send->start(2000);
}
//tcp socket连接
void WaveDetector_dev::slot_tcpConnect()
{
    tim_send->start(2000);

}
//tcp socket断开
void WaveDetector_dev::slot_tcpDisConnect()
{
    online_stat = false;
    tim_send->stop();
    emit clear_allData();
    qDebug()<<tr(">>>Wavedetector Online_stat = ")<<online_stat<<endl;
}
