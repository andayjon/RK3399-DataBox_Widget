#ifndef CLOUD_DECK_H
#define CLOUD_DECK_H
#include "tcp_socket.h"
#include <QObject>
#include <QByteArray>

//云台设备类
class Cloud_Deck:public QObject
{
    Q_OBJECT
public:
    explicit Cloud_Deck(QObject *parent);
    ~Cloud_Deck();

    void set_Comm_Speed(quint8,quint8);             //设置移动速度
    void set_Comm_tarHorAngle(quint16,quint8);      //设置水平目标角度
    void set_Comm_tarPitAngle(quint16,quint8);      //设置俯仰目标角度
    void set_Comm_realTimeBack(quint8);     //设置实时回传
    void query_Comm_Speed(quint8);          //查询移动速度
    void query_Comm_horAngle(quint8);       //查询水平角
    void query_Comm_pitAngle(quint8);       //查询俯仰角
    quint8 cal_checkSum(char*,quint8);      //计算校验和
    tcp_socket *tcpsocket_cli;
    QByteArray command;         //指令
    quint8 dev_addr = 0;        //设备地址
    quint16 hor_angle = 0;      //当前水平角度
    qint16 pit_angle = 0;       //当前俯仰角度
    quint16 targ_h_angle = 0;   //目标水平角度
    qint16 targ_p_angle = 0;    //目标俯仰角度

    quint8 move_speed = 0;      //当前移动速度
    quint8 set_speed = 0;       //设置移动速度
    bool online_stat = false;   //在线状态
    bool ping_enable = false;
private:


signals:
    void send_Command(QByteArray);  //发送指令
    void recvFormDev(QByteArray);   //接收来自设备数据

private slots:
    void slot_recvFormDev(QByteArray);
    void slot_sendCommand(QByteArray);
    void slot_connToServer();
    void slot_onDisConnect();
};

#endif // CLOUD_DECK_H
