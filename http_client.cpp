#include "http_client.h"
//#include <synchapi.h>
#define DEBUG_EN
Http_Client::Http_Client(QObject *parent):QObject(parent)
{
    m_aManager = new QNetworkAccessManager(this);
    connect(m_aManager,&QNetworkAccessManager::finished,this,&Http_Client::replyFinished);
    connect(m_aManager,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),this,SLOT(AuthRequiredReply(QNetworkReply*,QAuthenticator*)));
    connect(this, &Http_Client::clear_allData, this, &Http_Client::slot_clear_allData);
    //QString str_url = "https://115.28.131.3:5000/api/v3/login";
    //QString str_url = "https://192.168.11.100:5000/api/v3/login";
    QSslConfiguration m_sslConfig = m_Request.sslConfiguration();
    m_sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    m_sslConfig.setProtocol(QSsl::TlsV1SslV3);
    m_Request.setSslConfiguration(m_sslConfig);
    tim_get = new QTimer(this);
    connect(tim_get, &QTimer::timeout, this, &Http_Client::slot_questData);
    connect(this, SIGNAL(Signal_ReadRecvFinished()), this, SLOT(rec_md_finished()));
}
Http_Client::~Http_Client()
{
    Http_logout(url_logout);
    delete this;
}
//登录
int Http_Client::Http_login(QString destUrl)
{

    //m_Request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    m_Request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    m_Request.setRawHeader("User-Agent", "QHttpsC 1.0");
    m_Request.setRawHeader("Authorization", "Basic WHtpsWeeSFW4=");
    m_Request.setRawHeader("Last-Modified", "Sun, 06 Nov 2019 08:49:37 GMT");


    url_login = destUrl + "/api/v3/login";
    url_getsys = destUrl + "/api/v3/system";
    url_logout = destUrl + "/api/v3/logout";
#ifdef DEBUG_EN
    qDebug()<<tr(">>>Https Login URL = %1, User = %2, Passwd = %3<<<").arg(url_login).arg(md_User).arg(md_Passwd)<<endl;
#endif
    m_Request.setUrl(QUrl(url_login));
    m_aManager->get(m_Request);
    quest_code = 0;
    return 0;
}
//退出
int Http_Client::Http_logout(QString destUrl)
{
    QString headers_auth = QString("Basic %1:").arg(m_token);
    mp_data = headers_auth.toUtf8();
#ifdef DEBUG_EN
    qDebug()<<tr(">>>Https:Get System Authorization = %1").arg(QString(mp_data))<<endl;
#endif
    m_Request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    m_Request.setRawHeader("User-Agent", "QHttpsC 1.0");
    m_Request.setRawHeader("Authorization", "Basic WHtpsWeeSFW4=");
    m_Request.setRawHeader("Last-Modified", "Sun, 06 Nov 2019 08:49:37 GMT");

    m_Request.setUrl(QUrl(destUrl));
    m_aManager->post(m_Request,mp_data);
    quest_code = 1;
    return 0;
}

void Http_Client::Http_DetResults(QString destUrl)
{
    QString headers_auth = "Basic "+ m_token +":";
    mp_data = headers_auth.toUtf8();
#ifdef DEBUG_EN
    qDebug()<<tr(">>>Https:Get System Authorization = %1").arg(QString(mp_data))<<endl;
#endif
    m_Request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    m_Request.setRawHeader("User-Agent", "QHttpsC 1.0");
    m_Request.setRawHeader("Authorization", mp_data);
    m_Request.setRawHeader("Last-Modified", "Sun, 06 Nov 2019 08:49:37 GMT");
#ifdef DEBUG_EN
    qDebug()<<tr(">>>Https:Get Url = %1 ").arg(url_getsys)<<endl;
#endif
    m_Request.setUrl(QUrl(url_getsys));

    m_aManager->get(m_Request);
    //m_aManager->post(m_Request,mp_data);
    quest_code = 2;

}
//请求数据槽函数
void Http_Client::slot_questData()
{
    tim_get->stop();
    Http_DetResults(url_getsys);
    tim_get->start(2000);
}
//完成应答
void Http_Client::replyFinished(QNetworkReply *reply)
{

    QString strError = reply->errorString();
#ifdef DEBUG_EN
    qDebug() << strError;
#endif
    variant = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if (variant.isValid())
#ifdef DEBUG_EN
        qDebug() << variant.toInt();
#endif
    if(variant.toInt() == 302) {//重定向
        QUrl relativeUrl = QUrl(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());//获取重定向的信息
#ifdef DEBUG_EN
        qDebug()<<relativeUrl.toString();
#endif
        //Http_login(baseUrl.resolved(relativeUrl).toString());//拼接成完整的一个URL
        return ;
    }
    if(variant.toInt() == 200) {//TODO 获取cookie信息
//        cookies = m_aManager->cookieJar()->cookiesForUrl(QUrl(baseUrl));
//        cookies = m_aManager->cookieJar()->cookiesForUrl(baseUrl);
#ifdef DEBUG_EN
        qDebug()<<tr(">>>Https Get Data Acquisition successful!<<<")<<endl;
#endif
        switch (quest_code) {
        case 0:     //login
            log_stat = 1;
#ifdef DEBUG_EN
            qDebug()<<tr(">>>Https:User Login Quest successful!")<<endl;
#endif
            tim_get->start(2000);
            break;
        case 1:     //logout
            log_stat = 0;
#ifdef DEBUG_EN
            qDebug()<<tr(">>>Https:User Logout Quest successful!")<<endl;
#endif
            break;
        case 2:     //Request
            stat_code = 1;
            online_stat = true;
            work_stat = 0;
#ifdef DEBUG_EN
            qDebug()<<tr(">>>Https:User Request Data successful!")<<endl;
#endif
            break;
        default:
            stat_code = -1;
            work_stat = 1;
#ifdef DEBUG_EN
            qDebug()<<tr(">>>Https:User Request Data failed!")<<endl;
#endif
            break;
        }
#ifdef DEBUG_EN
        qDebug()<<tr(">>>Https:Cookies = ")<<cookies<<endl;
#endif
    } else if(variant.toInt() == 500) {
        online_stat = false;
        work_stat = 2;
#ifdef DEBUG_EN
        qDebug()<<tr(">>>Https Server Internal Error<<<")<<endl;
#endif
    }else {
        online_stat = false;
        work_stat = 2;
#ifdef DEBUG_EN
        qDebug()<<tr(">>>Https Get Data Failed!<<<")<<endl;
#endif
        return;
    }
    QByteArray bytes;
    m_rcv_data = reply->readAll();
    emit this->Signal_ReadRecvFinished();

    this->AnalysisJson(&m_rcv_data);

//    QJsonParseError jsonError;
//    QJsonDocument document = QJsonDocument::fromJson(m_rcv_data,&jsonError);
//    if(jsonError.error != QJsonParseError::NoError){
//        qDebug()<<QStringLiteral("resolve Json failed");
//        return;
//    }

//#ifdef _DEBUG_
//    qDebug()<<"bytes lenth ="<<m_rcv_data.length()<<endl;
//    qDebug()<<m_rcv_data<<endl;
//#endif

//    //解析Json
//    if(document.isObject()){
//        QJsonObject obj = document.object();
//        if(obj.contains("token")){
//            QJsonValue j_token = obj.value("token");
//            m_token = j_token.toString();
//#ifdef _DEBUG_
//            qDebug()<<m_token<<endl;
//#endif
//        }
///**********************************************************/
//        QByteArray uav_id;
//        QString dev_ver;
//        int dev_count;
//        int uav_count;

//        if(obj.contains("version")) {
//            QJsonValue v_ver = obj.value("version");
//            dev_ver = v_ver.toString();
//            qDebug()<<"version = "<<dev_ver<<endl;
//        }
//        if(obj.contains("total_devices")) { //侦测的设备数量
//            QJsonValue count_val = obj.value("total_devices");
//            dev_count = count_val.toInt();
//            qDebug()<<"侦测到"<<dev_count<<"个设备..."<<endl;
//        }
//        if(obj.contains("devices_info")) {

//            QJsonValue dev_info = obj.value("devices_info");
//            if(dev_info.isObject()) {
//                QJsonObject obj_dev = dev_info.toObject();
//                QJsonObject::iterator obj_it;
//                uav_count = 0;
//                for(obj_it=obj_dev.begin(); obj_it!=obj_dev.end(); obj_it++) {
//                    uav_count++;
//                    QString str_id = obj_it.key();
//                    if(str_id.contains("dr")) {     //无人机信息
//                        QJsonObject obj_uav = obj_it->toObject();

//                    }
//                    qDebug()<<"dev_id_list"<<uav_count<<" key = "<<str_id<<endl;
//                }
//                int dev_count = obj_dev.size();
//                for(int i =0; i<dev_count; i++) {     //遍历设备信息
//                    QJsonValue temp_val = obj_dev;
//                    if(temp_val.isObject()) {
//                        QJsonObject obj_uav = temp_val.toObject();
//                        if(obj_uav.contains("device_name")) {  //设备名称

//                        }
//                        if(obj_uav.contains("gps")) {       //GPS
//                            double dev_lat, dev_lng;
//                            QJsonValue v_gps = obj_uav.value("gps");
//                            if(v_gps.isObject()) {
//                                QJsonObject obj_gps = v_gps.toObject();
//                                if(obj_gps.contains("lat")) {
//                                    QJsonValue v_lat = obj_gps.value("lat");
//                                    dev_lat = v_lat.toDouble();
//                                }
//                                if(obj_gps.contains("lng")) {
//                                    QJsonValue v_lng = obj_gps.value("lng");
//                                    dev_lng = v_lng.toDouble();
//                                }
//                                qDebug()<<"第"<<i+1<<"无人机GPS信息："<<endl;
//                                qDebug()<<"lat:"<<dev_lat<<endl;
//                                qDebug()<<"lng:"<<dev_lng<<endl;
//                            }
//                        }
//                    }
//                }
//            }

//        }
//    }
/**********************************************************/
}
//请求头
void Http_Client::AuthRequiredReply(QNetworkReply *reply,QAuthenticator *auth)
{
//    QString username = md_User;
    QString username = md_superUser;
//    QString password = md_Passwd;
    QString password = md_superPasswd;
    if(username.isEmpty()||password.isEmpty())
    {
#ifdef DEBUG_EN
        qDebug()<<tr("Error")<<tr("Please input valid username and password!")<<endl;
#endif
        return;
    }
    auth->setUser(username);
    auth->setPassword(password);
}
//获取随机数
float Http_Client::Get_RandNumber(void)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int i_num = qrand()%100;
    int num_rand = rand()%100;

    float f_num = i_num/100;
#ifdef DEBUG_EN
    qDebug()<<"Get Rand Number:"<<f_num<<endl;
#endif
    return f_num;
}
void Http_Client::rec_md_finished()
{
#ifdef DEBUG_EN
    qDebug()<<tr("返回状态码:")<<this->variant.toString()<<endl;
    qDebug()<<tr("返回数据:")<<QString(this->m_rcv_data)<<endl;
#endif
}
//解析数据
void Http_Client::AnalysisJson(QByteArray* m_byte)
{
    QJsonParseError jsonError;

    QJsonDocument document = QJsonDocument::fromJson(*m_byte,&jsonError);
    if(jsonError.error != QJsonParseError::NoError){
#ifdef DEBUG_EN
        qDebug()<<QStringLiteral("resolve Json failed");
#endif
        return;
    }
    //解析Json
    if(document.isObject()){
        QJsonObject obj = document.object();
        if(obj.contains("token")){
            QJsonValue j_token = obj.value("token");
            m_token = j_token.toString();
#ifdef DEBUG_EN
            qDebug()<<tr("Get token: %1").arg(m_token)<<endl;
#endif
        }
        /***************************2020-2-26 JSON 解析*******************************/
                QByteArray uav_id;
                QString dev_ver;
                int dev_count;
                int uav_count;
                QString orderid;
                if(obj.contains("version")) {
                    QJsonValue v_ver = obj.value("version");
                    dev_ver = v_ver.toString();
#ifdef DEBUG_EN
                    qDebug()<<"version = "<<dev_ver<<endl;
#endif
                }
                if(obj.contains("total_devices")) { //侦测的设备数量
                    QJsonValue count_val = obj.value("total_devices");
                    dev_count = count_val.toInt();
#ifdef DEBUG_EN
                    qDebug()<<"侦测到"<<dev_count<<"个设备..."<<endl;
#endif
                }
                if(obj.contains("devices_info")) {  //侦测到的设备信息

                    QJsonValue dev_info = obj.value("devices_info");
                    if(dev_info.isObject()) {
                        QJsonObject obj_dev = dev_info.toObject();
                        QJsonObject::iterator obj_it;
                        QJsonArray js_data; //无人机数据
                        uav_count = 0;
                        for(obj_it=obj_dev.begin(); obj_it!=obj_dev.end(); obj_it++) {
                            uav_count++;                            
                            QJsonObject js_uav; //无人机信息
                            QString str_id = obj_it.key();
                            //无人机信息
                            if(str_id.contains("dr")) {
                                //QJsonValue val_uav = obj_it->toValue();
                                QJsonObject obj_location;   //MQ
                                if(obj_it->isObject()) {
                                    QJsonObject obj_dr = obj_it->toObject();
                                    if(obj_dr.contains("gps")) {
                                        QJsonValue val_gps = obj_dr.value("gps");   //GPS位置信息
                                        if(val_gps.isObject()) {
                                            QJsonObject obj_gps = val_gps.toObject();
                                            if(obj_gps.contains("lat") && obj_gps.contains("lng")) {
                                                double dr_lat = obj_gps.value("lat").toDouble();
                                                double dr_lng = obj_gps.value("lng").toDouble();
                                                obj_location.insert("latitude",dr_lat);
                                                obj_location.insert("longitude", dr_lng);
#ifdef DEBUG_EN
                                                qDebug()<<QString("dr %1 ").arg(uav_count)<<"lat:"<<QString::number(dr_lat, 'f', 9)<<endl;
                                                qDebug()<<QString("dr %1 ").arg(uav_count)<<"lng:"<<QString::number( dr_lng, 'f', 9)<<endl;
#endif
                                                js_uav.insert("location", obj_location);
                                            }
                                        }
                                    }

                                    if(obj_dr.contains("model")) {      //无人机Model
                                        QJsonValue val_model = obj_dr.value("model");
                                        QString str_model = val_model.toString();
                                        js_uav.insert("se", str_model);
#ifdef DEBUG_EN
                                        qDebug()<<QString("dr %1 ").arg(uav_count)<<"model:"<<str_model<<endl;
#endif
                                    }
                                    if(obj_dr.contains("device_name")) {    //无人机名称
                                        QJsonValue val_name = obj_dr.value("device_name");
                                        QString str_name = val_name.toString();
                                        orderid = str_name;
                                    }
                                    if(obj_dr.contains("id")) {         //无人机ID
                                        QJsonValue val_id = obj_dr.value("id");
                                        QString str_id = val_id.toString();
                                        js_uav.insert("droneID", str_id);
                                        orderid += "-" + str_id;
#ifdef DEBUG_EN
                                        qDebug()<<QString("dr %1 ").arg(uav_count)<<"id:"<<str_id<<endl;
#endif
                                    }                                    
                                    QDateTime cur_time = QDateTime::currentDateTime();  //时间戳
                                    uint label_time = cur_time.toTime_t();
                                    js_uav.insert("date",int(label_time));
                                    js_uav.insert("receiveDate", int(label_time));
                                    QString str_utc = QString::number(label_time);
#ifdef DEBUG_EN
                                    qDebug()<<QString("dr %1 ").arg(uav_count)<<"UTC:"<<str_utc<<endl;
#endif
                                }
                                float alt = Get_RandNumber();
                                alt *= 300;
                                js_uav.insert("altitude",double(alt));
                                js_uav.insert("orderID", orderid);
                                js_uav.insert("oid", orderid);
                                js_uav.insert("userName", "");
                                js_uav.insert("speed", 1.5);
                                js_uav.insert("platform", "android");
                                js_uav.insert("yaw", 254);
                                js_uav.insert("droneType", "MavicMini");
                                js_uav.insert("contact", "");
                                js_uav.insert("flightTime", 2233);
                                js_uav.insert("altitudeType", 0);
                                QJsonArray jarr_tasks;
                                //jarr_tasks.insert(1,"full");
                                jarr_tasks.append("full");
                                js_uav.insert("tasks", jarr_tasks);
                            }
                            //遥控器
                            if(str_id.contains("rc")) {
                                if(obj_it->isObject()) {
                                    QJsonObject obj_rc = obj_it->toObject();
                                    if(obj_rc.contains("gps")) {
                                        QJsonValue val_gps = obj_rc.value("gps");
                                        if(val_gps.isObject()) {
                                            QJsonObject obj_gps = val_gps.toObject();
                                            if(obj_gps.contains("lat") && obj_gps.contains("lng")) {
                                                double rc_lat = obj_gps.value("lat").toDouble();
                                                double rc_lng = obj_gps.value("lng").toDouble();
#ifdef DEBUG_EN
                                                qDebug()<<QString("rc %1 ").arg(uav_count+1)<<"lat:"<<QString::number(rc_lat, 'f', 9)<<endl;
                                                qDebug()<<QString("rc %1 ").arg(uav_count+1)<<"lng:"<<QString::number( rc_lng, 'f', 9)<<endl;
#endif
                                            }
                                        }
                                    }
                                }
                            }
                            js_data.append(js_uav);
#ifdef DEBUG_EN
                            qDebug()<<"dev_id_list"<<uav_count<<" key = "<<str_id<<endl;
#endif
                        }
                        //添加消息队列信息
                        json_MQ.insert("sourcetype", 1);
                        json_MQ.insert("sourceid", databox_id);
                        json_MQ.insert("data",js_data);

                    }

                }
                if(obj.contains("sensors_info")) {  //侦测设备信息
                    QJsonValue sensors_info = obj.value("sensors_info");
                    if(sensors_info.isObject()) {
                        QJsonObject obj_sensor = sensors_info.toObject();
                        QJsonObject::iterator obj_it;
                        for(obj_it=obj_sensor.begin(); obj_it!=obj_sensor.end(); obj_it++) {
                            local_devid = obj_it.key();

                        }
                        QJsonObject devdata;
                        devdata.insert("datatype", up_data_type);
                        devdata.insert("localdevid", local_devid);
                        json_MQ.insert("devdata", devdata);
                    }
                }
#ifdef DEBUG_EN
                qDebug()<<">>>HTTP Wavedetect Rocket MQ Message: "<<json_MQ<<endl;
#endif
                if(!json_MQ.isEmpty())
                    emit this->Signal_AnalysisFinished();
        /***************************2020-2-26 JSON 解析*******************************/
            }
}
//清除所有数据
void Http_Client::slot_clear_allData()
{
    if(!json_MQ.isEmpty()) {
        QStringList list_key = json_MQ.keys();
        for(int i = 0; i < json_MQ.size(); i++) {
            json_MQ.remove(list_key.at(i));
        }
#ifdef DEBUG_EN
        qDebug()<<tr(">>>HTTP WaveDetector:Clear AllData done!")<<endl;
#endif
        return;
    }
#ifdef DEBUG_EN
    qDebug()<<tr(">>>HTTP WaveDetector:There is no AllData to clear!")<<endl;
#endif
}
