#include "tcp_client.h"
#include <QDateTime>


tcp_client::tcp_client(QObject *parent)
{
    tcp_socket = new QTcpSocket(this);
    recon_timer = new QTimer(this);

    connect(tcp_socket, &QTcpSocket::readyRead, this, &tcp_client:: slot_readMsgFromServer);
    connect(this, &tcp_client::clear_allData, this, &tcp_client::slot_clear_allData);
    connect(this, &tcp_client::clear_uavData, this, &tcp_client::slot_clear_uavData);
    connect(this, &tcp_client::add_allData, this, &tcp_client::slot_add_allData);
    connect(tcp_socket, &QTcpSocket::connected, this, &tcp_client::slot_onConnect);
    connect(tcp_socket, &QTcpSocket::disconnected, this, &tcp_client::slot_onDisConnect);
    //connect(tcp_socket, &QTcpSocket::error(QAbstractSocket::SocketError), this, &tcp_client::onError(QAbstractSocket::SocketError));
    connect(this->recon_timer,&QTimer::timeout, this, &tcp_client::connectServer);
}
tcp_client::~tcp_client()
{
    tcp_socket->close();
}
//连接断开
void tcp_client::slot_onDisConnect()
{
    m_isOkConect = false;
}

void tcp_client::slot_onConnect()
{
    //已经连接
    m_isOkConect = true;
}
//连接到服务器
bool tcp_client::connectServer()
{
    if(!m_isOkConect) {
        if(server_port!=0 && !server_ip.isEmpty()) {
            tcp_socket->connectToHost(server_ip,server_port);
        } else {
            tcp_socket->connectToHost("192.168.1.168",18816);
        }

        if(tcp_socket->waitForConnected(3000)) {
            qDebug()<<"tcp_client connect"<<server_ip+":"<<server_port<<"success!"<<endl;
            recon_timer->stop();
            return true;
        } else {
            qDebug()<<"tcp_client connect"<<server_ip+":"<<server_port<<"faild!"<<endl;
            recon_timer->start(5000);
            return false;
        }
    }
    return true;

}
//发送数据到服务器
void tcp_client::writeMsgToServer(QString str)
{
    tcp_socket->write(str.toLatin1());
}
//读取服务器发送消息
void tcp_client::slot_readMsgFromServer()
{
    recMsg = tcp_socket->readAll();
    qDebug()<<"tcp_client recMsg from Server:"<<recMsg<<endl;
    analysisJson(recMsg);
}
//获取随机数
float tcp_client::Get_RandNumber(void)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int i_num = qrand()%100;

    float f_num = i_num/100;

    qDebug()<<"Get Rand Number:"<<f_num<<endl;
    return f_num;
}
//解析侦测数据
void tcp_client::analysisJson(QByteArray m_bytes)
{
    QJsonParseError jsonError;
    QByteArray x_bytes;
    quint16 i;
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
        qDebug()<<QStringLiteral("resolve Json failed");
        return;
    }
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
                    //插入orderID
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
                emit clear_allData();
                if(obj.contains("msg_level")) {
                    QJsonValue msg_lev = obj.value("msg_level");
                    int msg_level = msg_lev.toInt();
                    if(msg_level == 0) {        //设备正常
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
                        devdata.insert("datatype", 13);
                        localdevid = "CS108H"+QString::number(md_dev_id);
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

void tcp_client::slot_clear_allData()
{
    if(!md_upMQ_info.isEmpty()) {
        QStringList list_key = md_upMQ_info.keys();
        for(int i = 0;i < md_upMQ_info.size(); i++) {
            md_upMQ_info.remove(list_key.at(i));
        }
        qDebug()<<"Clear AllData done!"<<endl;
        return;
    }
    qDebug()<<"There is no AllData to clear!"<<endl;
}
void tcp_client::slot_clear_uavData()
{
    if(!js_ArrData.isEmpty()) {
        for(int i = 0;i < js_ArrData.size(); i++) {
            js_ArrData.removeAt(i);
        }
        qDebug()<<"Clear UavData done!"<<endl;
        QJsonDocument doc_arr;
        doc_arr.setArray(js_ArrData);
        qDebug()<<"本地侦测设备数据包内容(清除无人机信息)："<<endl;
        qDebug()<<"UAV_Information:"+QString(doc_arr.toJson())<<endl;     //打印侦测无人机信息
        return;
    }
    qDebug()<<"There is no UavData to clear!"<<endl;
}

void tcp_client::slot_add_allData()
{
    md_upMQ_info.insert("data",js_ArrData);
    md_upMQ_info.insert("sourcetype",1);
    md_upMQ_info.insert("sourceid",databox_id);
    qDebug()<<"本地侦测设备数据包内容："<<endl;
    qDebug()<<QString(QJsonDocument(this->md_upMQ_info).toJson())<<endl;        //打印上传数据
    emit clear_uavData();

}
