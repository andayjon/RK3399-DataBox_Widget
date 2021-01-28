#ifndef WEBSOCKET_QJON_H
#define WEBSOCKET_QJON_H

#include "datareceive.h"
#include <QApplication>
#include <QTime>
//#include "widget.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QtWebSockets/QWebSocket>
#include <QObject>
#include <QJsonObject>
//#include <QWebSocket>
//#include <QString>

class Web_qjon:public QObject
{
    Q_OBJECT
public:
    Web_qjon();

    DataReceive *w_socket;  //websocket

    QJsonDocument JS_document;
    QJsonObject JS_dev_info_obj;            //设备
    QJsonObject Js_shield_obj;              //屏蔽器
    QJsonObject JS_pa_info_obj;             //功放
    QJsonObject JS_shield_info_obj[10];     //屏蔽器
    QJsonObject JS_radar_info_obj;          //雷达
    QJsonObject JS_batt_info_obj;           //电池
    QJsonObject JS_avia_info_obj[50];       //航空
    QJsonObject JS_gps_info_obj;            //gps
    QJsonObject JS_send_msg_obj;            //发送消息
    QJsonObject JS_send_head_obj;

    QJsonArray js_shield_arry;      //屏蔽器
    QJsonArray JS_pa_info_arry;     //功放
    QJsonArray JS_avia_info_arry;   //航空
    QByteArray byte;

    void sleep(unsigned int msec);
signals:
    void startConnect(QString url);
    void closeConnect();
    void sendMsg(QString);

public slots:
    void readDevInfo();     //读取设备信息

};

#endif // WEBSOCKET_QJON_H
