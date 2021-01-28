#include "cloud_deck.h"
//#define DEBUG_EN
Cloud_Deck::Cloud_Deck(QObject *parent):QObject (parent)
{
    tcpsocket_cli = new tcp_socket(this);

    connect(this, &Cloud_Deck::send_Command, tcpsocket_cli, &tcp_socket::send_toserver);
    connect(tcpsocket_cli, &tcp_socket::recv_formserver, this, &Cloud_Deck::slot_recvFormDev);
    connect(tcpsocket_cli, &tcp_socket::conn_toServerOk, this, &Cloud_Deck::slot_connToServer);
    connect(tcpsocket_cli, &tcp_socket::onDisConnect, this, &Cloud_Deck::slot_onDisConnect);
}
Cloud_Deck::~Cloud_Deck()
{

}

//设置移动速度
void Cloud_Deck::set_Comm_Speed(quint8 speed, quint8 dir)
{
    char cmd[7];
    command.clear();
    if(speed > 0 && dev_addr != 0) {
        cmd[0] = 0xFF;
        cmd[1] = dev_addr;
        switch (dir) {
            case 0:     //设置水平右速度
                cmd[2] = 0x00;
                cmd[3] = 0x02;
                cmd[4] = speed;
                cmd[5] = 0x00;
                break;
            case 1:     //设置水平左速度
                cmd[2] = 0x00;
                cmd[3] = 0x04;
                cmd[4] = speed;
                cmd[5] = 0x00;
                break;
            case 2:     //设置俯仰上速度
                cmd[2] = 0x00;
                cmd[3] = 0x08;
                cmd[4] = 0x00;
                cmd[5] = speed;
                break;
            case 3:     //设置俯仰下速度
                cmd[2] = 0x00;
                cmd[3] = 0x10;
                cmd[4] = 0x00;
                cmd[5] = speed;
                break;
            default:
#ifdef DEBUG_EN
                qDebug()<<tr(">>>速度控制参数错误<<< -- 方向速度")<<endl;
#endif
                break;
        }

        cmd[6] = cal_checkSum(cmd+1, 5);
    }
    command.append(cmd, 7);
#ifdef DEBUG_EN
    qDebug()<<tr("Cloud Deck Command:")<<command.toHex()<<endl;
#endif
    emit send_Command(command);
}
//设置实时返回
void Cloud_Deck::set_Comm_realTimeBack(quint8 addr)
{
    if(addr > 0) {
        char cmd[7];
        command.clear();
        cmd[0] = 0xFF;
        cmd[1] = addr;
        cmd[2] = 0x00;
        cmd[3] = 0x09;
        cmd[4] = 0x00;
        cmd[5] = 0x05;
        cmd[6] = cal_checkSum(cmd+1, 5);
        command.append(cmd, 7);
#ifdef DEBUG_EN
        qDebug()<<tr("Cloud Deck Command:")<<command.toHex()<<endl;
#endif
    }

    emit send_Command(command);
}

//查询当前水平角度
void Cloud_Deck::query_Comm_horAngle(quint8 addr)
{
    if(addr > 0) {
        char cmd[7];
        command.clear();
        cmd[0] = 0xFF;
        cmd[1] = addr;
        cmd[2] = 0x00;
        cmd[3] = 0x51;
        cmd[4] = 0x00;
        cmd[5] = 0x00;
        cmd[6] = cal_checkSum(cmd+1, 5);
        command.append(cmd, 7);
#ifdef DEBUG_EN
        qDebug()<<tr("Cloud Deck Command:")<<command.toHex()<<endl;
#endif
    }
    emit send_Command(command);

}

//查询当前俯仰角度
void Cloud_Deck::query_Comm_pitAngle(quint8 addr)
{
    if(addr > 0) {
        char cmd[7];
        command.clear();
        cmd[0] = 0xFF;
        cmd[1] = addr;
        cmd[2] = 0x00;
        cmd[3] = 0x53;
        cmd[4] = 0x00;
        cmd[5] = 0x00;
        cmd[6] = cal_checkSum(cmd+1, 5);
        command.append(cmd, 7);
#ifdef DEBUG_EN
        qDebug()<<tr("Cloud Deck Command:")<<command.toHex()<<endl;
#endif
    }
    emit send_Command(command);
}

//设置目标水平角度
void Cloud_Deck::set_Comm_tarHorAngle(quint16 angle, quint8 addr)
{
    if(addr > 0) {
        char cmd[7];
        command.clear();
        cmd[0] = 0xFF;
        cmd[1] = addr;
        cmd[2] = 0x00;
        cmd[3] = 0x4B;
        cmd[4] = angle>>8&0xFF;
        cmd[5] = angle&0xFF;
        cmd[6] = cal_checkSum(cmd+1, 5);
        command.append(cmd, 7);
#ifdef DEBUG_EN
        qDebug()<<tr("Cloud Deck Command:")<<command.toHex()<<endl;
#endif
    }
    emit send_Command(command);
}
//设置目标俯仰角度
void Cloud_Deck::set_Comm_tarPitAngle(quint16 angle, quint8 addr)
{
    if(addr > 0) {
        char cmd[7];
        command.clear();
        cmd[0] = 0xFF;
        cmd[1] = addr;
        cmd[2] = 0x00;
        cmd[3] = 0x4D;
        cmd[4] = angle>>8&0xFF;
        cmd[5] = angle&0xFF;
        cmd[6] = cal_checkSum(cmd+1, 5);
        command.append(cmd, 7);
#ifdef DEBUG_EN
        qDebug()<<tr("Cloud Deck Command:")<<command.toHex()<<endl;
#endif
    }
    emit send_Command(command);
}
//计算校验和
quint8 Cloud_Deck::cal_checkSum(char *bytes, quint8 len)
{
    quint8 check_sum = 0;
    quint8 val;
    if(len == 0) {
        return check_sum;
    } else {
        check_sum = *bytes;
        for(val = 1; val < len; val++ ) {
            check_sum = ((*(bytes+val))+check_sum)&0xFF;
        }
    }

    return check_sum;
}
//连接到云台设备
void Cloud_Deck::slot_connToServer()
{
    //query_Comm_horAngle(1);
}
//接收到设备返回数据
void Cloud_Deck::slot_recvFormDev(QByteArray bytes)
{
#ifdef DEBUG_EN
    qDebug()<<tr("Recv Cloud Deck Data:")<<bytes.toHex()<<endl;
#endif
    char *tmp;
    tmp = bytes.data();
    quint8 check_sum = cal_checkSum(tmp+1, 5);

    if(check_sum != bytes.at(6)) {
#ifdef DEBUG_EN
        qDebug()<<tr("Cloud Deck Return Check sum Error!")<<endl;
#endif
        return;
    }
    //有效的数据
    if(bytes.at(0) == 0xFF && bytes.at(1) == dev_addr) {
        online_stat = true;
        //水平
        if(bytes.at(2) == 0x00 && bytes.at(3) == 0x59) {
            hor_angle = (bytes.at(4)*256+bytes.at(5))/100;
#ifdef DEBUG_EN
            qDebug()<<tr("Current Hor Angel:")<<hor_angle<<endl;
#endif
        }
        //俯仰
        if(bytes.at(2) == 0x00 && bytes.at(3) == 0x5B) {
            quint16 tmp_angle = (bytes.at(4)*256+bytes.at(5))/100;
            if(tmp_angle > 315 && tmp_angle <= 360) {
                pit_angle = qint16(360-tmp_angle);
            } else {
                pit_angle = qint16(0-tmp_angle);
            }
#ifdef DEBUG_EN
            qDebug()<<tr("Current Pit Angle:")<<pit_angle<<endl;
#endif
        }
    } else {
        online_stat = false;
    }
}

void Cloud_Deck::slot_sendCommand(QByteArray bytes)
{

}
//tcp socket断开连接
void Cloud_Deck::slot_onDisConnect()
{
    online_stat = false;
}
