#include "net_widget.h"
//#define DEBUG_EN
Net_Widget::Net_Widget(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(0, 75, 800, 280);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:black");
    wid_x1 = this->x();
    wid_y1 = this->y();
#ifdef DEBUG_EN
    qDebug()<<tr("Get Network Widget Point x = %1, y = %2").arg(wid_x1).arg(wid_y1)<<endl;
    qDebug()<<tr("Get Network Widget Size x = %1, y = %2").arg(this->width()).arg(this->height())<<endl;
#endif
    QFont ft_lab;
    ft_lab.setFamily("黑体");
    ft_lab.setPointSize(this->y()/4);
    db_1 = new dashBoard(this);
    db_2 = new dashBoard(this);
    db_3 = new dashBoard(this);

    db_1->resize(260,260);
    db_1->setGeometry(0,20,260,260);
//    db_1->m_title = "CHINA MOBILE";
//    db_1->m_title = QStringLiteral("中国  移动");
    db_1->m_title = "";
    db_1->card_stat = 0;
    db_1->rssi = 0;
    db_1->delta_angle = 2;

    db_2->resize(260,260);
    db_2->setGeometry(260,20,260,260);
//    db_2->m_title = "CHINA UNICOM";
//    db_2->m_title = QStringLiteral("中国  联通");
    db_2->m_title = "";
    db_2->card_stat = 0;
    db_2->rssi = 0;
    db_2->delta_angle = 2;

    db_3->resize(260,260);
    db_3->setGeometry(520,20,260,260);
//    db_3->m_title = "CHINA TELECOM ";
//    db_3->m_title = QStringLiteral("中国  电信");
    db_3->m_title = "";
    db_3->card_stat = 0;
    db_3->rssi = 0;
    db_3->delta_angle = 2;

    manager = new QNetworkAccessManager(this);
    tim_setNetspeed = new QTimer(this);
    tim_getNetinfo = new QTimer(this);
    //关联信号和槽
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));

//    connect(tim_setNetspeed, &QTimer::timeout, this, &Net_Widget::slot_timOut);
    connect(tim_getNetinfo, &QTimer::timeout, this, &Net_Widget::slot_timgetOut);
    connect(this, &Net_Widget::sn_getspeed, this, &Net_Widget::slot_timOut);
    connect(this, &Net_Widget::sn_setNetspeed1, db_1, &dashBoard::slot_GetnetSpeed);
    connect(this, &Net_Widget::sn_setNetspeed2, db_2, &dashBoard::slot_GetnetSpeed);
    connect(this, &Net_Widget::sn_setNetspeed3, db_3, &dashBoard::slot_GetnetSpeed);
//    tim_setNetspeed->start(500);
    tim_getNetinfo->start(500); //2020-11-13 50ms
}
Net_Widget::~Net_Widget()
{

}
/* 水平滑块实现函数*/
void Net_Widget::sliderhValue(int val)
{
#ifdef DEBUG_EN
    qDebug()<<tr("valueChanged = %1").arg(val)<<endl;
#endif
}

//获取随机数
quint32 Net_Widget::GetrandNum()
{
    QTime time;
    time = QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    quint32 ret = qrand()%150000;

    return ret;
}
void Net_Widget::slot_timgetOut()
{
    tim_getNetinfo->stop();
    query_index++;
    if(query_index != 0) {
        if(query_index > 3) {
            query_index = 1;
        }
        if(query_index > 0 && query_index < 4) {
            switch (query_index) {
            case 1:
                //发送请求
                manager->get(QNetworkRequest(QUrl("http://192.168.11.1/cgi-bin/get4ginfo")));
#ifdef DEBUG_EN
                qDebug()<<tr("-->>Net Widget Get 4G Information...")<<endl;
#endif
                break;
            case 2:
                //发送请求
                manager->get(QNetworkRequest(QUrl("http://192.168.11.1/cgi-bin/getbondwaninfo")));
#ifdef DEBUG_EN
                qDebug()<<tr("-->>Net Widget Get BondWan Information...")<<endl;
#endif
                break;
            case 3:
                //发送请求
                manager->get(QNetworkRequest(QUrl("http://192.168.11.1/cgi-bin/getspeed")));
#ifdef DEBUG_EN
                qDebug()<<tr("-->>Net Widget Get Speed Information...")<<endl;
#endif
                break;
            default:
                break;
            }
        }
    }
    tim_getNetinfo->start(500); //2020-11-13 50ms
}
//设置网速
void Net_Widget::slot_timOut()
{
//    tim_setNetspeed->stop();
    double card1_speed;
//    quint32 s1,s2,s3;
//    s1 = GetrandNum();
//    s2 = GetrandNum();
//    s3 = GetrandNum();
//    qDebug()<<tr("Get Rand Number s1 = %1, s2 = %2, s3 = %3").arg(s1).arg(s2).arg(s3)<<endl;
    if(public_info.card_array[0].card_rxspeed > public_info.card_array[0].card_txspeed) {
        card1_speed = public_info.card_array[0].card_rxspeed;
    } else
        card1_speed = public_info.card_array[0].card_txspeed;
    db_1->m_title = public_info.model_array[0].operator_info;
    emit sn_setNetspeed1(card1_speed);
    double card2_speed;
    if(public_info.card_array[1].card_rxspeed > public_info.card_array[1].card_txspeed) {
        card2_speed = public_info.card_array[1].card_rxspeed;
    } else
        card2_speed = public_info.card_array[1].card_txspeed;
    db_2->m_title = public_info.model_array[1].operator_info;
    emit sn_setNetspeed2(card2_speed);
    double card3_speed;
    if(public_info.card_array[2].card_rxspeed > public_info.card_array[2].card_txspeed) {
        card3_speed = public_info.card_array[2].card_rxspeed;
    } else
        card3_speed = public_info.card_array[2].card_txspeed;
    db_3->m_title = public_info.model_array[2].operator_info;
    emit sn_setNetspeed3(card3_speed);
#ifdef DEBUG_EN
    qDebug()<<tr("Get Card Speed Card1 = %1Kb/s, Card2 = %2Kb/s, Card3 = %3Kb/s").arg(card1_speed).arg(card2_speed).arg(card3_speed)<<endl;
#endif
//    tim_setNetspeed->start(500);
}
//http
void Net_Widget::replyFinished(QNetworkReply *reply)
{
    //使用utf8编码，这样才可以显示中文
    QTextCodec *codec = QTextCodec::codecForName("utf8");
    QString all = codec->toUnicode(reply->readAll());
    QByteArray bytes = all.toUtf8();
//    qDebug()<<tr("-->>HTTP Return:%1").arg(QString(bytes))<<endl;
    reply->deleteLater();   //最后要释放reply对象
    analysisJson(bytes);

}
//解析路由器信息
void Net_Widget::analysisJson(QByteArray bytes)
{
    QJsonParseError jsonError;
    QByteArray tmp_bytes = "{\n\"sim1_status\":\"ready\",\n\"sim2_status\":\"ready\",\n\"sim3_status\":\"ready\",\n\"sim4_status\":\"NULL\"\n}";
//    bytes.remove(bytes.length()-4, 1);
    QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
    if(jsonError.error != QJsonParseError::NoError) {
#ifdef DEBUG_EN
        qDebug()<<tr("Net Widget resolve Json failed! Error code = %1, Data = %2").arg(jsonError.error).arg(QString(bytes))<<endl;
#endif
        return;
    } else {
        if(document.isObject()) {
            QJsonObject tmp_obj = document.object();
            this->m_rwlock.lockForWrite();
            if(tmp_obj.contains("sim1_status") && tmp_obj.contains("rssi1")) {   //4g信息
                if(tmp_obj.contains("sim1_status")) {
                    QString str_stat = tmp_obj.value("sim1_status").toString();
                    QString str_rssi = tmp_obj.value("rssi1").toString();
                    int rssi = str_rssi.toInt();
                    if(str_stat == "ready") {
                        public_info.card_array[0].card_stat = true;
                        db_1->card_stat = 1;
                    } else {
                        public_info.card_array[0].card_stat = false;
                        db_1->card_stat = 0;
                    }
                    if(rssi < 32) {
                        public_info.card_array[0].card_rssi = rssi*3;
                        db_1->rssi = rssi*3;
                    } else {
                        public_info.card_array[0].card_rssi = 0;
                        db_1->rssi = 0;
                    }
#ifdef DEBUG_EN
                    qDebug()<<tr("-->>Net Widget SIM1 Status = %1 Rssi = %2").arg(public_info.card_array[0].card_stat).arg(public_info.card_array[0].card_rssi)<<endl;
#endif
                }
                if(tmp_obj.contains("sim2_status") && tmp_obj.contains("rssi2")) {
                    QString str_stat = tmp_obj.value("sim2_status").toString();
                    QString str_rssi = tmp_obj.value("rssi2").toString();
                    int rssi = str_rssi.toInt();
                    if(str_stat == "ready") {
                        public_info.card_array[1].card_stat = true;
                        db_2->card_stat = 1;
                    } else {
                        public_info.card_array[1].card_stat = false;
                        db_2->card_stat = 0;
                    }
                    if(rssi < 32) {
                        public_info.card_array[1].card_rssi = rssi*3;
                        db_2->rssi = rssi*3;
                    } else {
                        public_info.card_array[1].card_rssi = 0;
                        db_2->rssi = 0;
                    }
#ifdef DEBUG_EN
                    qDebug()<<tr("-->>Net Widget SIM2 Status = %1 Rssi = %2").arg(public_info.card_array[1].card_stat).arg(public_info.card_array[1].card_rssi)<<endl;
#endif
                }
                if(tmp_obj.contains("sim3_status") && tmp_obj.contains("rssi3")) {
                    QString str_stat = tmp_obj.value("sim3_status").toString();
                    QString str_rssi = tmp_obj.value("rssi3").toString();
                    int rssi = str_rssi.toInt();
                    if(str_stat == "ready") {
                        public_info.card_array[2].card_stat = true;
                        db_3->card_stat = 1;
                    } else {
                        public_info.card_array[2].card_stat = false;
                        db_3->card_stat = 0;
                    }
                    if(rssi < 32) {
                        public_info.card_array[2].card_rssi = rssi*3;
                        db_3->rssi = rssi*3;
                    } else {
                        public_info.card_array[2].card_rssi = 0;
                        db_3->rssi = 0;
                    }
#ifdef DEBUG_EN
                    qDebug()<<tr("-->>Net Widget SIM3 Status = %1 Rssi = %2").arg(public_info.card_array[2].card_stat).arg(public_info.card_array[2].card_rssi)<<endl;
#endif
                }
                if(tmp_obj.contains("sim4_status") && tmp_obj.contains("rssi4")) {
                    QString str_stat = tmp_obj.value("sim4_status").toString();
                    QString str_rssi = tmp_obj.value("rssi4").toString();
                    int rssi = str_rssi.toInt();
                    if(str_stat == "ready") {
                        public_info.card_array[3].card_stat = true;
                    } else {
                        public_info.card_array[3].card_stat = false;
                    }
                    if(rssi < 32) {
                        public_info.card_array[3].card_rssi = rssi*3;
                    } else {
                        public_info.card_array[3].card_rssi = 0;
                    }
#ifdef DEBUG_EN
                    qDebug()<<tr("-->>Net Widget SIM4 Status = %1 Rssi = %2").arg(public_info.card_array[3].card_stat).arg(public_info.card_array[3].card_rssi)<<endl;
#endif
                }
                //运营商信息 2020-6-28
                if(tmp_obj.contains("operator1")) {
                    QString str_oper = tmp_obj.value("operator1").toString();
                    if(!str_oper.isEmpty()) {
                        if(str_oper == "CHINA-MOBILE") {    //中国移动
                            public_info.model_array[0].operator_info = "China  Mobile";
//                            db_1->m_title = QStringLiteral("中国  移动");
                        }else if(str_oper == "CHN-CT") {    //中国电信
                            public_info.model_array[0].operator_info = "China  Telcom";
//                            db_1->m_title = QStringLiteral("中国  电信");
                        }else if(str_oper == "CHN-CU") {    //中国联通
                            public_info.model_array[0].operator_info = "China  Unicom";
//                            db_1->m_title = QStringLiteral("中国  联通");
                        }else {

                        }
                    }
                }
                if(tmp_obj.contains("operator2")) {
                    QString str_oper = tmp_obj.value("operator2").toString();
                    if(!str_oper.isEmpty()) {
                        if(str_oper == "CHINA-MOBILE") {    //中国移动
                            public_info.model_array[1].operator_info = "China  Mobile";
//                            db_2->m_title = QStringLiteral("中国  移动");
                        }else if(str_oper == "CHN-CT") {    //中国电信
                            public_info.model_array[1].operator_info = "China  Telcom";
//                            db_2->m_title = QStringLiteral("中国  电信");
                        }else if(str_oper == "CHN-CU") {    //中国联通
                            public_info.model_array[1].operator_info = "China  Unicom";
//                            db_2->m_title = QStringLiteral("中国  联通");
                        }else {

                        }
                    }
                }
                if(tmp_obj.contains("operator3")) {
                    QString str_oper = tmp_obj.value("operator3").toString();
                    if(!str_oper.isEmpty()) {
                        if(str_oper == "CHINA-MOBILE") {    //中国移动
                            public_info.model_array[2].operator_info = "China  Mobile";
//                            db_3->m_title = QStringLiteral("中国  移动");
                        }else if(str_oper == "CHN-CT") {    //中国电信
                            public_info.model_array[2].operator_info = "China  Telcom";
//                            db_3->m_title = QStringLiteral("中国  电信");
                        }else if(str_oper == "CHN-CU") {    //中国联通
                            public_info.model_array[2].operator_info = "China  Unicom";
//                            db_3->m_title = QStringLiteral("中国  联通");
                        }else {

                        }
                    }
                }
                if(tmp_obj.contains("nettype1")) {
                    QString str_nettype = tmp_obj.value("nettype1").toString();
                    if(!str_nettype.isEmpty() && str_nettype != "NULL") {
                        public_info.model_array[0].net_type = str_nettype;
                    }
                }
                if(tmp_obj.contains("nettype2")) {
                    QString str_nettype = tmp_obj.value("nettype2").toString();
                    if(!str_nettype.isEmpty() && str_nettype != "NULL") {
                        public_info.model_array[1].net_type = str_nettype;
                    }
                }
                if(tmp_obj.contains("nettype3")) {
                    QString str_nettype = tmp_obj.value("nettype3").toString();
                    if(!str_nettype.isEmpty() && str_nettype != "NULL") {
                        public_info.model_array[2].net_type = str_nettype;
                    }
                }
                if(tmp_obj.contains("nettype4")) {
                    QString str_nettype = tmp_obj.value("nettype4").toString();
                    if(!str_nettype.isEmpty() && str_nettype != "NULL") {
                        public_info.model_array[3].net_type = str_nettype;
                    }
                }
                if(tmp_obj.contains("ip1")) {
                    QString str_ip = tmp_obj.value("ip1").toString();
                    if(!str_ip.isEmpty()) {
                        public_info.model_array[0].ip_addr = str_ip;
                    }
                }
                if(tmp_obj.contains("ip2")) {
                    QString str_ip = tmp_obj.value("ip2").toString();
                    if(!str_ip.isEmpty()) {
                        public_info.model_array[1].ip_addr = str_ip;
                    }
                }
                if(tmp_obj.contains("ip3")) {
                    QString str_ip = tmp_obj.value("ip3").toString();
                    if(!str_ip.isEmpty()) {
                        public_info.model_array[2].ip_addr = str_ip;
                    }
                }
                if(tmp_obj.contains("ip4")) {
                    QString str_ip = tmp_obj.value("ip4").toString();
                    if(!str_ip.isEmpty()) {
                        public_info.model_array[3].ip_addr = str_ip;
                    }
                }
                if(tmp_obj.contains("model1")) {
                    QString str_model_number = tmp_obj.value("model1").toString();
                    if(!str_model_number.isEmpty() && str_model_number != "NULL") {
                        public_info.model_array[0].model_number = str_model_number;
                    }
                }
                if(tmp_obj.contains("model2")) {
                    QString str_model_number = tmp_obj.value("model2").toString();
                    if(!str_model_number.isEmpty() && str_model_number != "NULL") {
                        public_info.model_array[1].model_number = str_model_number;
                    }
                }
                if(tmp_obj.contains("model3")) {
                    QString str_model_number = tmp_obj.value("model3").toString();
                    if(!str_model_number.isEmpty() && str_model_number != "NULL") {
                        public_info.model_array[2].model_number = str_model_number;
                    }
                }
                if(tmp_obj.contains("model4")) {
                    QString str_model_number = tmp_obj.value("model4").toString();
                    if(!str_model_number.isEmpty() && str_model_number != "NULL") {
                        public_info.model_array[3].model_number = str_model_number;
                    }
                }
                if(tmp_obj.contains("imei1")) {
                    QString str_imei = tmp_obj.value("imei1").toString();
                    if(!str_imei.isEmpty() && str_imei != "NULL") {
                        public_info.model_array[0].model_imei = str_imei;
                    }
                }
                if(tmp_obj.contains("imei2")) {
                    QString str_imei = tmp_obj.value("imei2").toString();
                    if(!str_imei.isEmpty() && str_imei != "NULL") {
                        public_info.model_array[1].model_imei = str_imei;
                    }
                }
                if(tmp_obj.contains("imei3")) {
                    QString str_imei = tmp_obj.value("imei3").toString();
                    if(!str_imei.isEmpty() && str_imei != "NULL") {
                        public_info.model_array[2].model_imei = str_imei;
                    }
                }
                if(tmp_obj.contains("imei4")) {
                    QString str_imei = tmp_obj.value("imei4").toString();
                    if(!str_imei.isEmpty() && str_imei != "NULL") {
                        public_info.model_array[3].model_imei = str_imei;
                    }
                }
                if(tmp_obj.contains("imsi1")) {
                    QString str_imsi = tmp_obj.value("imsi1").toString();
                    if(!str_imsi.isEmpty() && str_imsi != "NULL") {
                        public_info.card_array[0].card_imsi = str_imsi;
                    }
                }
                if(tmp_obj.contains("imsi2")) {
                    QString str_imsi = tmp_obj.value("imsi2").toString();
                    if(!str_imsi.isEmpty() && str_imsi != "NULL") {
                        public_info.card_array[1].card_imsi = str_imsi;
                    }
                }
                if(tmp_obj.contains("imsi3")) {
                    QString str_imsi = tmp_obj.value("imsi3").toString();
                    if(!str_imsi.isEmpty() && str_imsi != "NULL") {
                        public_info.card_array[2].card_imsi = str_imsi;
                    }
                }
                if(tmp_obj.contains("imsi4")) {
                    QString str_imsi = tmp_obj.value("imsi4").toString();
                    if(!str_imsi.isEmpty() && str_imsi != "NULL") {
                        public_info.card_array[3].card_imsi = str_imsi;
                    }
                }
                if(tmp_obj.contains("iccid1")) {
                    QString str_iccid = tmp_obj.value("iccid1").toString();
                    if(!str_iccid.isEmpty() && str_iccid != "NULL") {
                        public_info.card_array[0].card_iccid = str_iccid;
                    }
                }
                if(tmp_obj.contains("iccid2")) {
                    QString str_iccid = tmp_obj.value("iccid2").toString();
                    if(!str_iccid.isEmpty() && str_iccid != "NULL") {
                        public_info.card_array[1].card_iccid = str_iccid;
                    }
                }
                if(tmp_obj.contains("iccid3")) {
                    QString str_iccid = tmp_obj.value("iccid3").toString();
                    if(!str_iccid.isEmpty() && str_iccid != "NULL") {
                        public_info.card_array[2].card_iccid = str_iccid;
                    }
                }
                if(tmp_obj.contains("iccid4")) {
                    QString str_iccid = tmp_obj.value("iccid4").toString();
                    if(!str_iccid.isEmpty() && str_iccid != "NULL") {
                        public_info.card_array[3].card_iccid = str_iccid;
                    }
                }
            } else if(tmp_obj.contains("bondstatus") && tmp_obj.contains("wanstatus")) {     //WAN information
                QString str_bond_stat = tmp_obj.value("bondstatus").toString();
                if(str_bond_stat == "bonded") {
                    public_info.bond_stat = true;
                } else {
                    public_info.bond_stat = false;
                }
                QString str_wan_stat = tmp_obj.value("wanstatus").toString();
                if(str_wan_stat == "connected") {
                    public_info.wan_stat = true;
                } else {
                    public_info.wan_stat = false;
                }
                public_info.bond_ip = tmp_obj.value("bondserverip").toString();
                public_info.wan_ip = tmp_obj.value("wanip").toString();
#ifdef DEBUG_EN
                qDebug()<<tr("-->>Net Widget Public Info Bond Stat = %1, Wan Stat = %2, Bond IP = %3, Wan IP = %4")
                       .arg(public_info.bond_stat).arg(public_info.wan_stat).arg(public_info.bond_ip).arg(public_info.wan_ip)<<endl;
#endif
            } else if(tmp_obj.contains("lte1_rx_speed") && tmp_obj.contains("lte1_tx_speed")) {  //speed
                //Card1
                if(tmp_obj.contains("lte1_rx_speed")) {
                    QString str_speed = tmp_obj.value("lte1_rx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[0].card_rxspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card1 rx Speed = %1 Kb/s").arg(public_info.card_array[0].card_rxspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[0].card_rxspeed = str_list.at(0).toDouble()*(1024);
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card1 rx Speed = %1 Mb/s").arg(public_info.card_array[0].card_rxspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[0].card_rxspeed = str_list.at(0).toInt()/1000;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card1 rx Speed = %1 Kb/s").arg(public_info.card_array[0].card_rxspeed)<<endl;
#endif
                    }
                }
                if(tmp_obj.contains("lte1_tx_speed")) {
                    QString str_speed = tmp_obj.value("lte1_tx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[0].card_txspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card1 tx Speed = %1 Kb/s").arg(public_info.card_array[0].card_txspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[0].card_txspeed = str_list.at(0).toDouble()*(1024);
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card1 tx Speed = %1 Mb/s").arg(public_info.card_array[0].card_txspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[0].card_txspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card1 tx Speed = %1 Kb/s").arg(public_info.card_array[0].card_txspeed)<<endl;
#endif
                    }
                }

                //Card2
                if(tmp_obj.contains("lte2_rx_speed")) {
                    QString str_speed = tmp_obj.value("lte2_rx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[1].card_rxspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card2 rx Speed = %1 Kb/s").arg(public_info.card_array[1].card_rxspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[1].card_rxspeed = str_list.at(0).toDouble()*(1024);
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card2 rx Speed = %1 Mb/s").arg(public_info.card_array[1].card_rxspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[1].card_rxspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card2 rx Speed = %1 Kb/s").arg(public_info.card_array[1].card_rxspeed)<<endl;
#endif
                    }
                }
                if(tmp_obj.contains("lte2_tx_speed")) {
                    QString str_speed = tmp_obj.value("lte2_tx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[1].card_txspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card2 tx Speed = %1 Kb/s").arg(public_info.card_array[1].card_txspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[1].card_txspeed = str_list.at(0).toDouble()*(1024);
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card2 tx Speed = %1 Mb/s").arg(public_info.card_array[1].card_txspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[1].card_txspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card2 tx Speed = %1 Kb/s").arg(public_info.card_array[1].card_txspeed)<<endl;
#endif
                    }
                }

                //Card3
                if(tmp_obj.contains("lte3_rx_speed")) {
                    QString str_speed = tmp_obj.value("lte3_rx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[2].card_rxspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card3 rx Speed = %1 Kb/s").arg(public_info.card_array[2].card_rxspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[2].card_rxspeed = str_list.at(0).toDouble()*(1024);
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card3 rx Speed = %1 Mb/s").arg(public_info.card_array[2].card_rxspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[2].card_rxspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card3 rx Speed = %1 Kb/s").arg(public_info.card_array[2].card_rxspeed)<<endl;
#endif
                    }
                }
                if(tmp_obj.contains("lte3_tx_speed")) {
                    QString str_speed = tmp_obj.value("lte3_tx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[2].card_txspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card3 tx Speed = %1 Kb/s").arg(public_info.card_array[2].card_txspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[2].card_txspeed = str_list.at(0).toDouble()*(1024);
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card3 tx Speed = %1 Mb/s").arg(public_info.card_array[2].card_txspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[2].card_txspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card3 tx Speed = %1 Kb/s").arg(public_info.card_array[2].card_txspeed)<<endl;
#endif
                    }
                }

                //Card4
                if(tmp_obj.contains("lte4_rx_speed")) {
                    QString str_speed = tmp_obj.value("lte4_rx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[3].card_rxspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card4 rx Speed = %1 Kb/s").arg(public_info.card_array[3].card_rxspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[3].card_rxspeed = str_list.at(0).toDouble()*(1024);
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card4 rx Speed = %1 Mb/s").arg(public_info.card_array[3].card_rxspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[3].card_rxspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card4 rx Speed = %1 Kb/s").arg(public_info.card_array[3].card_rxspeed)<<endl;
#endif
                    }
                }
                if(tmp_obj.contains("lte4_tx_speed")) {
                    QString str_speed = tmp_obj.value("lte4_tx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.card_array[3].card_txspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card4 tx Speed = %1 Kb/s").arg(public_info.card_array[3].card_txspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.card_array[3].card_txspeed = str_list.at(0).toDouble()*1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card4 tx Speed = %1 Mb/s").arg(public_info.card_array[3].card_txspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.card_array[3].card_txspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Card4 tx Speed = %1 Kb/s").arg(public_info.card_array[3].card_txspeed)<<endl;
#endif
                    }
                }
                //Total
                if(tmp_obj.contains("total_rx_speed")) {
                    QString str_speed = tmp_obj.value("total_rx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.total_rxspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Total rx Speed = %1 Kb/s").arg(public_info.total_rxspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.total_rxspeed = str_list.at(0).toDouble()*1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Total rx Speed = %1 Mb/s").arg(public_info.total_rxspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.total_rxspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Total rx Speed = %1 Kb/s").arg(public_info.total_rxspeed)<<endl;
#endif
                    }
                }
                if(tmp_obj.contains("total_tx_speed")) {
                    QString str_speed = tmp_obj.value("total_tx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.total_txspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Total tx Speed = %1 Kb/s").arg(public_info.total_txspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.total_txspeed = str_list.at(0).toDouble()*1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Total tx Speed = %1 Mb/s").arg(public_info.total_txspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.total_txspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>Total tx Speed = %1 b/s").arg(public_info.total_txspeed)<<endl;
#endif
                    }
                }
                //WAN信息
                if(tmp_obj.contains("wan_rx_speed")) {
                    QString str_speed = tmp_obj.value("wan_rx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.wan_rxspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>WAN rx Speed = %1 Kbs").arg(public_info.wan_rxspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.wan_rxspeed = str_list.at(0).toDouble()*1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>WAN rx Speed = %1 Mbs").arg(public_info.wan_rxspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.wan_rxspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>WAN rx Speed = %1 Kbs").arg(public_info.wan_rxspeed)<<endl;
#endif
                    }
                }
                if(tmp_obj.contains("wan_tx_speed")) {
                    QString str_speed = tmp_obj.value("wan_tx_speed").toString();
                    if(str_speed.contains("Kb/s")) {
                        QStringList str_list = str_speed.split("Kb/s");
                        public_info.wan_txspeed = str_list.at(0).toDouble();
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>WAN tx Speed = %1 Kbs").arg(public_info.wan_txspeed)<<endl;
#endif
                    } else if(str_speed.contains("Mb/s")){
                        QStringList str_list = str_speed.split("Mb/s");
                        public_info.wan_txspeed = str_list.at(0).toDouble()*1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>WAN tx Speed = %1 Mbs").arg(public_info.wan_txspeed/(1024))<<endl;
#endif
                    } else if(str_speed.contains("b/s")) {
                        QStringList str_list = str_speed.split("b/s");
                        public_info.wan_txspeed = str_list.at(0).toInt()/1024;
#ifdef DEBUG_EN
                        qDebug()<<tr("-->>WAN tx Speed = %1 bs").arg(public_info.wan_txspeed)<<endl;
#endif
                    }
                }
                emit sn_getspeed();
            } else {
                this->m_rwlock.unlock();
                return;
            }
            this->m_rwlock.unlock();
        }
    }
}
void Net_Widget::mousePressEvent(QMouseEvent *event)
{
#if 1
    if(!dialog_dispFlag) {
//        tim_getNetinfo->stop();
        dialog_dispFlag = true;
//        net_dialog *net_dlog = new net_dialog(this);
//        net_dlog->setAttribute(Qt::WA_DeleteOnClose);
//        net_dlog->show();
    //    net_dlog->exec();
        m_cardInfo = new Card_Dialog(this);
        Update_NetInfo(m_cardInfo);
        m_cardInfo->show();
        this->setVisible(false);
        connect(m_cardInfo, &Card_Dialog::sig_closeDialog, this, &Net_Widget::slot_dialogRet);
        this->setVisible(true);
//
//        connect(net_dlog, &net_dialog::sign_quitFlag, this, &Net_Widget::slot_dialogRet);

//
    }
#endif

}
void Net_Widget::slot_dialogRet(int ret)
{
    if(ret == 1) {

//        tim_getNetinfo->start(100);
        dialog_dispFlag = false;
        this->db_1->En_repaint = true;
        this->db_2->En_repaint = true;
        this->db_3->En_repaint = true;
        this->setVisible(false);
    }else {
        qDebug()<<tr("netDialog Return Error!")<<endl;
//        tim_getNetinfo->start(100);
        dialog_dispFlag = false;
        this->db_1->En_repaint = true;
        this->db_2->En_repaint = true;
        this->db_3->En_repaint = true;
    }
    this->setVisible(true);
}

void Net_Widget::Update_NetInfo(Card_Dialog *card_dialog)
{
    //Add Card1 Information
    card_dialog->m_card1Info = "----Model1 Information----\n";
    //public_info.model_array[0].model_imei = "235689789569222";
    card_dialog->m_card1Info.append("IMEI:" + public_info.model_array[0].model_imei + "\n");
    //public_info.model_array[0].model_number = "0123456789ABCDEF";
    card_dialog->m_card1Info.append("NUMBER:" + public_info.model_array[0].model_number  + "\n");
    card_dialog->m_card1Info.append("NETTYPE:" + public_info.model_array[0].net_type  + "\n");
    card_dialog->m_card1Info.append("IP:" + public_info.model_array[0].ip_addr + "\n\n");

    card_dialog->m_card1Info.append("----SIM Card1 Information----\n");
    card_dialog->m_card1Info.append("OPERATOR:" + public_info.model_array[0].operator_info + "\n");
//    public_info.card_array[0].card_imsi = "123456789012345";
    card_dialog->m_card1Info.append("IMSI:" + public_info.card_array[0].card_imsi + "\n");
    if(public_info.card_array[0].card_stat) {
    card_dialog->m_card1Info.append("STAT:ready\n");
    } else {
        card_dialog->m_card1Info.append("STAT:absent\n");
    }
    card_dialog->m_card1Info.append("RSSI:" + QString::number(public_info.card_array[0].card_rssi) + "\n");

    //Add Card2 Information
    card_dialog->m_card2Info = "----Model2 Information----\n";
    card_dialog->m_card2Info.append("IMEI:" + public_info.model_array[1].model_imei + "\n");
    card_dialog->m_card2Info.append("NUMBER:" + public_info.model_array[1].model_number  + "\n");
    card_dialog->m_card2Info.append("NETTYPE:" + public_info.model_array[1].net_type  + "\n");
    card_dialog->m_card2Info.append("IP:" + public_info.model_array[1].ip_addr + "\n\n");

    card_dialog->m_card2Info.append("----SIM Card2 Information----\n");
    card_dialog->m_card2Info.append("OPERATOR:" + public_info.model_array[1].operator_info + "\n");
    card_dialog->m_card2Info.append("IMSI:" + public_info.card_array[1].card_imsi + "\n");
    if(public_info.card_array[1].card_stat) {
    card_dialog->m_card2Info.append("STAT:ready\n");
    } else {
        card_dialog->m_card2Info.append("STAT:absent\n");
    }
    card_dialog->m_card2Info.append("RSSI:" + QString::number(public_info.card_array[1].card_rssi) + "\n");

    //Add Card3 Information
    card_dialog->m_card3Info = "----Model3 Information----\n";
    card_dialog->m_card3Info.append("IMEI:" + public_info.model_array[2].model_imei + "\n");
    card_dialog->m_card3Info.append("NUMBER:" + public_info.model_array[2].model_number  + "\n");
    card_dialog->m_card3Info.append("NETTYPE:" + public_info.model_array[2].net_type  + "\n");
    card_dialog->m_card3Info.append("IP:" + public_info.model_array[2].ip_addr + "\n\n");

    card_dialog->m_card3Info.append("----SIM Card3 Information----\n");
    card_dialog->m_card3Info.append("OPERATOR:" + public_info.model_array[2].operator_info + "\n");
    card_dialog->m_card3Info.append("IMSI:" + public_info.card_array[2].card_imsi + "\n");
    if(public_info.card_array[2].card_stat) {
    card_dialog->m_card3Info.append("STAT:ready\n");
    } else {
        card_dialog->m_card3Info.append("STAT:absent\n");
    }
    card_dialog->m_card3Info.append("RSSI:" + QString::number(public_info.card_array[2].card_rssi) + "\n");
}
