#ifndef RADAR_H
#define RADAR_H

#include "udp_socket.h"
#include <QObject>
#include <QString>
#include <QDebug>
#include <QVector>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtEndian>
#include <QDateTime>

struct Target_stc {
    quint16 flag = 0;       //起批标志
    quint16 number = 0;     //目标编号
    quint16 target_count = 0;   //发现目标次数(航迹个数)
    quint16 C_angle = 0;    //航向角--0.01°
    qint16 P_angle = 0;     //仰角--0.01°
    quint32 dist = 0;       //距离--m
    quint16 speed = 0;      //航速--0.1m/s
    qint16 rv_speed = 0;    //径向速度--0.1m/s
    qint16 pnp = 0;         //偏北角--0.01°
    double lat = 0.0;       //纬度--1°
    double lon = 0.0;       //经度--1°
    float altitude = 0.0;   //高度--m
};

struct Radar_stc{
    quint16 id = 0;         //设备ID
    qint64 utc = 0;         //时间戳--ms
    quint8 pattern = 0;     //发送航迹模式
    QVector<Target_stc> m_vct;
    Target_stc *target_arr;
};

class Radar : public QObject
{
    Q_OBJECT
public:
    explicit Radar(QObject *parent = nullptr);
    ~Radar();
    int port;
    QJsonObject md_upMQ_info;   //RADAR上传消息
    quint8 md_dev_stat;         //RADAR设备状态
    QString md_dev_model;       //RADAR设备型号
    int md_dev_id;              //厂家设备ID
    QString conn_ip;            //设备IP
    QString databox_id;         //数据盒ID
    bool online_stat = false;   //设备在线状态 false-不在线 true-在线
    bool location_flag = false;
    quint8 subdev_type = 0;     //子设备类型
signals:
    void sig_sendMsg(QByteArray, int);
    void sig_sendAccomplish();
    void sig_recvMsg(QByteArray);
    void sig_start();
    void sig_addJsData();
    void sig_addUavData();
    void sig_clearJsData();
    void sig_clearUavData();
    void sig_sendJsData();
public slots:

private slots:
    void slot_sendMsg(QByteArray, int);
    void slot_udpRecvMsg(QByteArray);
    void slot_sendAccomplish();
    void slot_start();
    void slot_addUavInfo();
    void slot_addJsData();
    void slot_clearJsData();
    void slot_clearUavInfo();
    void slot_timerOut();
private:
    Radar_stc m_radar;
    udp_Socket *m_udpSocket;
    QTimer *m_timer;

    QJsonObject js_devGps;
    QJsonArray js_devData;
    QJsonObject js_ObjData;
    QJsonArray js_ArrData;

    bool dev_stat;
    void addDev_Info();
    void analysRadarData(QByteArray);
    void littTobig(uchar *, quint8, quint64);
};

#endif // RADAR_H
