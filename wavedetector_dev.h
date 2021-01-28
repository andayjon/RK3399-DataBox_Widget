#ifndef WAVEDETECTOR_DEV_H
#define WAVEDETECTOR_DEV_H

#include "tcp_socket.h"
#include <QObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtMath>

typedef struct
{
    double lon; //jingdu
    double lat; //weidu
}LOC_st;

class WaveDetector_dev:public QObject
{
    Q_OBJECT
public:
    explicit WaveDetector_dev(QObject *parent);
    ~WaveDetector_dev();

    QJsonObject md_upMQ_info;   //微波侦测上传消息
    quint8 md_dev_stat;         //微波侦测设备状态
    QString localdevid;         //本地微波侦测设备型号
    int md_dev_id;              //厂家设备ID
    QString databox_id;         //数据盒ID
    tcp_socket *tcp_cli;        //socket客户端
    bool online_stat = false;   //设备在线状态 false-不在线 true-在线
    bool location_flag = false;
    quint8 subdev_type = 0;     //子设备类型

    LOC_st ConvertDistanceToLogLat(LOC_st, int, double);
    LOC_st ConvertDistanceToLogLat2(LOC_st, int, double);
    LOC_st ConvertDistanceToLogLatGD(LOC_st , double, double);
    LOC_st dev_Location;
    LOC_st ConvertGPSToGcj02(LOC_st);
private:
    QJsonObject js_devGps;
    QJsonArray js_devData;
    QJsonObject js_ObjData;
    QJsonArray js_ArrData;

    QTimer *tim_send;
//    double pi = 3.1415926535897932384626;
    double x_pi = 3.14159265358979324 * 3000.0 / 180.0;
    double a = 6378245.0;
    double ee = 0.00669342162296594323;

    float Get_RandNumber(void);     //获取随机数
    void analysisJson(QByteArray);  //解析侦测数据
    void analysisJsonPack(QByteArray);
    void add_uavtoData();
    void add_AngleData();
    bool outof_China(double, double);
    double translate_lon(double, double);
    double translate_lat(double, double);
    double TransformLat(double, double);
    double TransformLon(double, double);
    LOC_st transformwgs(double, double);
signals:
    void clear_allData();
    void clear_uavData();
    void add_allData();
    void send_data(QByteArray);
    void sign_sendData();
private slots:
    void slot_msgFormServer(QByteArray);    //接收自服务器消息
    void slot_clear_allData();              //清除所有数据
    void slot_clear_uavData();              //清除无人机数据
    void slot_add_allData();                //添加所有数据
    void slot_tcpDisConnect();
    void slot_tcpConnect();
};

#endif // WAVEDETECTOR_DEV_H
