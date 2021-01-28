#ifndef WAVEDETECTOR_CD_H
#define WAVEDETECTOR_CD_H

#include "zeromqcli.h"
#include <QObject>
#include <QString>
#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QByteArray>
#include <QVector>
#include <QMap>
#include <QTimer>
#include <QtMath>
#include <QtGlobal>

class markSignal : public QObject
{
    Q_OBJECT
public:
    quint32 ch_id;  //信道ID
    quint64 id;     //信号ID
    QString mark;   //safe--标记为安全信号 unsafe--标记为非安全信号
};

class waveDevparam : public QObject
{
    Q_OBJECT
public:
    explicit waveDevparam(QObject *parent = nullptr){};
    QString cmd;    //startup--     shutdown--      reboot--
    qint32 interval;    //上报间隔默认3000ms
    bool train;         //true--开启录制环境  false--关闭录制环境
    markSignal *m_marksig;  //信号标记
    double gain;    //接收增益设置，默认100
    bool clear;     //true--清空已记录的环境
};
typedef struct {
    double lon;         //经度
    double lat;         //纬度
    double alt;         //高度
    qint32 type;        //0--gps,1--北斗,2--多模混合
    qint32 status;      //0--未锁定，其他值--锁定
    qint32 state;       //卫星数
}DevLocat_st;
typedef struct {
    quint64 id;         //信号ID
    QString name;       //无人机类型名称
    quint32 ch_id;      //信道ID
    double freq;        //频率
    double conf_d;      //可信度
    QString type;       //无人机类型
    bool dji_flg;       //是否为大疆机型
    uint utc;           //UTC
}UavInfo_st;
struct Loc_st {
    double lon;
    double lat;
};

class reportInfo :public QObject
{
    Q_OBJECT
public:
    quint64 dev_id;     //设备编号
    QString dev_stat;   //normal--运行正常  stopped--已停止    error--错误
    DevLocat_st dev_locat;  //设备位置信息
    QVector<UavInfo_st> uav_vec;    //无人机信息
    bool dev_train;         //是否正在学习环境
    bool ant_switch;        //天线开关状态

};

class waveDetector_cd : public QObject
{
    Q_OBJECT
public:
    explicit waveDetector_cd(QObject *parent = nullptr);
    ~waveDetector_cd();
    zeromqCli *m_zeroMqcli;
    QString databox_id;
    QJsonObject js_obj;
    reportInfo dev_repinfo;
    bool online_stat = false;   //设备在线状态 false-不在线 true-在线
    bool location_flag = false;
    quint8 subdev_type = 0;     //子设备类型
    double dev_lat = 0.0;       //纬度
    double dev_lon = 0.0;       //经度
    void dev_Init();
    void dev_close();
    void dev_start();
signals:
    void sig_addUavInfo();
    void sig_addUpInfo();
    void sig_clearUavInfo();
    void sig_clearUpInfo();
    void sig_sendUpInfo();
    void sig_confDevCmd(QByteArray);
public slots:
    void slot_addUavInfo();
    void slot_addUpInfo();
    void slot_clearUavInfo();
    void slot_clearUpInfo();
    void slot_recvMsg(QByteArray);
    void slot_sendTimertimout();
    void slot_confDev(QByteArray);
private:
    QJsonArray js_arr;
    QTimer *m_sendTim;
    waveDevparam dev_param;
    float Get_RandNumber();

protected:
    const QString dev_name = "CD12632001";
    void dev_sendCmd(QByteArray);
    void analysisJsonPack(QByteArray);
    Loc_st ConvertDistanceToLogLat(Loc_st, int, double);
};

#endif // WAVEDETECTOR_CD_H
