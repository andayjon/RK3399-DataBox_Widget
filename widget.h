#ifndef WIDGET_H
#define WIDGET_H


#include "netcan_dev.h"
#include "cloud_deck.h"
#include "cloud_whistle.h"
#include "wavedetector_cd.h"
#include "wavedetector_dev.h"
#include "http_client.h"
#include "mythreadfromqthread.h"
#include "serialportlist.h"
#include "websocket_qjon.h"
#include "datareceive.h"
#include "serialport.h"

#include <QWidget>
#include <QProcess>
#include <QDir>
#include <QTextEdit>
#include <QGroupBox>
#include <QCloseEvent>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QLabel>
#include <QTimer>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonParseError>
#include <QMutex>
#include <QComboBox>
#include <QPushButton>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QVBoxLayout>
#include <QHBoxLayout>


#define COMM_GPS_BAUD    9600
#define COMM_DEV_BAUD   115200
#define MAX_DISP_COUNT   512
#define SDR_COURSE_NAME     "dump1090"
#define QX_SDK_COURSE_NAME  "NSDK"

//电池箱结构 -- 2020年RK3399
typedef struct
{
    quint8 work_volt = 0;       //工作电压
    quint8 work_current = 0;    //工作电流
    quint8 temp = 0;            //温度
    quint8 surplus_cap = 0;     //剩余容量
    quint8 trouble_code = 0;    //故障码

    QString charge_num = 0;     //充电次数
    QString discharge_num = 0;  //放电次数
}Batt_Info_st;
//角度姿态模块结构 -- 2020年RK3399
typedef struct
{
    quint16 dir_angle = 0;      //方向角
    qint16 pit_angle = 0;       //俯仰角
    qint16 roll_angle = 0;      //横滚角
    bool att_stat = false;      //姿态模块状态
}Att_Info_st;
//功放结构 -- 2020年RK3399
typedef struct {

    qint8 alarm_stat = 0;       //告警状态
    quint8 swtich_stat = 0;     //开关
    quint8 f_power = 0;         //正向功率
    quint8 b_power = 0;         //反向功率
    quint8 temp = 0;            //温度
    quint8 volt = 0;            //电压
    quint8 current = 0;         //电流
    quint8 swr = 0;             //驻波比
    quint16 freq = 0;           //频率
    quint16 band = 0;           //带宽
    quint8 res;                 //预留

    quint8 pa_id = 0xF0;        //功放ID
    quint16 pa_freq = 0;        //频率
    quint8 pa_swtich = 0;       //开关

}Pa_Info_st;
//信道结构 -- 2020年RK3399
typedef struct
{
    quint8 ch_id = 0xF0;        //信道ID
    quint16 ch_freq = 0;        //信道频率
    quint8 ch_stat =0;          //信道状态
    quint16 ch_current = 0;     //信道电流
    quint8 ch_power_gear = 0;   //信道功率档位
    quint8 ch_swtich = 0;       //信道开关
    double ch_swr = 0.0;        //驻波比0.1
    quint8 pa_id = 0xF0;        //功放ID
}Ch_Info_st;
//屏蔽器结构 -- 2020年RK3399
typedef struct {
    quint8 shield_id = 0xF0;    //ID
    quint8 shield_sw_stat = 0;  //开关状态
    quint8 shield_temp = 0;     //温度
    quint8 shield_mode = 0;     //模式
    quint32 shield_cfg_exple;   //躯离模式配置
    quint32 shield_cfg_land;    //迫降模式配置

    Ch_Info_st ch_info[32];     //信道信息
    quint8 ch_count = 0;        //信道数量
    QList<quint8> ch_list;      //信道列表

    quint8 pa_count = 0;        //功放数量
    Pa_Info_st pa_info[32];     //功放信息
    quint8 sn[16];              //设备序列号
}Shield_Info_st;
//设备信息结构 -- 2020年RK3399
typedef struct {
    QString comm_pro_ver="";        //协议版本
    QString hardware_ver = "";      //
    quint8 group_num = 0;           //屏蔽器数量
    QString dev_id = "";            //设备ID
    quint8 dev_type = 0;            //设备类型
    quint8 dev_subtype = 0;         //子设备类型
    quint16 workradius = 0;         //工作半径
    quint8 subdev_num = 0;          //子设备数量
    bool angle_sw = false;          //角度补偿开关
    quint8 pit_compangle = 0;       //俯仰补偿角
    quint8 hor_compangle = 0;       //水平补偿角
    Att_Info_st att_info;           //角度姿态模块
    Batt_Info_st batt_info;         //电池箱
    Shield_Info_st shield_arry[16];  //屏蔽器数组
    quint8 work_pattern;            //工作模式
    QList<quint8> shield_IDlist;    //屏蔽器列表
    bool stat;
}Dev_Info_st;
typedef struct {
    double lon = 0.0;         //经度
    double lat = 0.0;         //纬度
    QString lon_dir;        //经度方向
    QString lat_dir;        //纬度方向
    double m_speed = 0.0;     //对地速度(英里)
    double n_speed = 0.0;     //对地速度(公里)
    double curr_cog;        //对地航向
}Gps_Info_st;               //2020-5-17
//扩展ID结构
typedef struct {
    quint8 shield_id;   //屏蔽器ID
    quint8 ch_id;       //通道ID
    quint8 index;       //信息序号
    quint8 len;         //数据长度
    quint8 dat[8];      //数据
}Exid_st;
typedef struct {
    bool connServerStat;
    bool connDevStat;
    QString devIP;
    QString devID;
}StatInfo_st;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent=0);
    ~Widget();
    void closeEvent(QCloseEvent *event);
    void closeWidget();
    void reboot();
    void wd_Init(quint8);
    QByteArray recv_buff_gps;   //GPS信息
    QByteArray barry_shield;    //屏蔽器
    quint8 device_id = 0;
    Gps_Info_st gps_info;
    StatInfo_st stat_info;
    Dev_Info_st dev_info;
    bool sleep_ms(int msec);
    //QVector<Shield_Info_st *> vector_Shield; //屏蔽器
private:
    Cloud_Deck *cloud_deck;             //云台
    WaveDetector_dev *wavedetec_dev;    //微波侦测TCP
    Http_Client *http_cli;              //微波侦测HTTP
    waveDetector_cd *zmq_wavedetec_dev; //微波侦测ZMQ
    Netcan_dev *netcan_dev;             //网络CAN设备
    QTimer *send_timer;         //定时发送
    QTimer *md_send_timer;      //定时发送
    QString send_msg;
    QString record_gpsInfo;
    QJsonObject *send_json;
    quint8 location_flag;
    quint16 tmp_hor_angle = 0;  //2020-4-5
    quint16 tmp_pit_angle = 0;  //2020-4-5
    //2020-4-6
    quint8 netcan_stat = 0;         //网络转CAN状态
    quint8 wavedetec_stat = 0;      //微波侦测状态
    quint8 wavedetec_num = 0;   	//微波侦测数量
    quint8 clouddeck_num = 0;   	//云台数量
    quint8 cloudwhistle_num = 0;    //云哨数量
    quint8 phototrack_num = 0;      //光电追踪数量
    quint8 radar_num = 0;           //雷达数量
    quint8 subdev_num = 0;      	//子设备数量
    qint8 discNet_count = 0;        //重连次数
    quint8 dev_index;
    bool avi_prostat = false;
    bool qxgps_prostat = false;
    bool get_mac_flag = false;
    bool ret_serial_flag;
    int shield_count;
    int wid_x1;
    int wid_y1;
    int recv_count_dev = 0;
    int recv_count_gps = 0;
    int recv_count_avi = 0;
    int recv_count_shield = 0;

    QLabel *lab_logo;
    QLabel *lab_time;
    QLabel *lab_sys_name;
    QLabel *lab_netstat;
    QLabel *label_shieldid;
    QLabel *label_paid;
/*************************************************/
    QGroupBox *grpbox_gps;
    QGroupBox *grpbox_avi;
    QGroupBox *grpbox_dev;

    QTextEdit *tedit_gps;
    QTextEdit *tedit_avi;
    QTextEdit *tedit_dev;

    QLabel *label_gif;
    QMovie *movie;
    QPixmap imag_netstat;
    QPixmap net_imag;

    QComboBox *commbox_shieldid;
    QComboBox *commbox_paid;
    QPushButton *pbutt_1;
    QPushButton *pbutt_2;
    QProcess *process;
    QProcess *my_pro_avi;
    QProcess *my_qx_gps;
    QProcess my_pro_gps;
    QProcess my_pro_dev;
    QByteArray my_pro_avi_recv;
    QByteArray my_pro_gps_recv;
    QByteArray my_pro_dev_recv;

    //Pa_Module *pa_info;     	//PA模块
    QTimer timer_out_md;    	//侦测
    QTimer time_out_dev;
    QTimer time_out_gps;
    QByteArray recv_buff;
/**********************************************/
    SerialPortList *port_list;  //串口列表
    SerialPort *Dev_serial;     //设备串口
    SerialPort *Gps_serial;     //GPS串口
    SerialPort *Test_serial;    //测试串口
    QString str_port;
    QSerialPort comm_gps;
    QSerialPort comm_dev;
    QSerialPort comm;
    QByteArray comm_gps_rxbuf;
    QByteArray comm_dev_rxbuf;
    QByteArray comm_gps_txbuf;
    QByteArray comm_dev_txbuf;

/***********************************************/
    //Web_qjon *web_json;
    DataReceive *w_socket;          //websocket
    DataReceive *md_socket;         //websocket本地无人机侦测接口 2020-3-31
    DataReceive *md_socket2;        //websocket2本地无人机侦测接口
    Cloud_whistle *cloud_whistle;   //云哨设备
    Shield_Info_st shield_info[16]; //下行数据状态

    QJsonObject JS_md_info_obj;     //本地微波侦测信息 2020-3-31
    QJsonDocument JS_document;
    QJsonObject JS_dev_info_obj;            //设备
    QJsonObject Js_shield_obj;              //屏蔽器
    QJsonObject JS_pa_info_obj;             //功放
    QJsonObject JS_shield_info_obj[16];     //屏蔽器
    QJsonObject JS_radar_info_obj;          //雷达
    QJsonObject JS_batt_info_obj;           //电池
    QJsonObject JS_avia_info_obj;           //航空
    QJsonObject JS_gps_info_obj;            //gps
    QJsonObject JS_send_msg_obj;            //发送消息
    QJsonObject JS_send_head_obj;

    QJsonArray js_shield_arry;      //屏蔽器
    QJsonArray JS_pa_info_arry;     //功放
    QJsonArray JS_avia_info_arry;   //航空
    QByteArray byte;

    QString local_ipaddr;           //本地IP
    QString local_macaddr;          //本地MAC
    QString dev_spemod;             //设备规格型号

/***********************************************/

    int Comm_Init(QString sub1_str, QString sub2_str, QString sub3_str);
    int Dev_prot_anal(QByteArray byte_arry, int len);
    QString Gps_prot_anal(QByteArray byte_arry, int len);
    QStringList Avi_prot_anal(QByteArray byte_arry, int len);
    int Avi_info_pro(QStringList list);
    int Sever_prot_anal(QJsonObject json_obj);

    void Get_DevMac();
    void disp_name(QString name);
    void disp_sys_name(QString name);
    void killApp_process(const QString &appName);
    bool checkAppRunstat(const QString &appName);

    void send_Devcmd();
    void send_Shield_cmd(Exid_st, quint8 *);

    //void socket_sendMsg();
    void disp_Dev_Info();           //显示设备信息
    void test_SendShield_Cmd();     //测试发送屏蔽器指令
    void get_stat_Fun();
signals:
    void startConnect();
    void closeConnect();
    void analy_gps_info();
    void sendMsg(QString);
    void sendmdMsg(QString);
    void sendmdHeartbeat(QString);
    void serialsend(QByteArray);
    void add_Msgdata(QJsonArray);
    void send_byteMsg(QByteArray);
    void send_reset4g();
    void exit();
public slots:
    void send_Compass_cmd(quint8 cmd_code);
private slots:
    void rec_md_inf();                  //微波侦测信息
    void slot_sendmdInfo();             //发送微波侦测信息
    void slot_Netcan_pro(QByteArray);   //处理网络CAN数据
    void output_avi();                  //输出航空信息
    void output_gps();                  //输出GPS信息
    void output_dev();                  //输出Device信息
    void showErr_avi_pro();
    void showstat_avi_pro(QProcess::ProcessState state);
    void showFini_avi_pro(int exitCode, QProcess::ExitStatus exitStatus);

    void showErr_gps_pro();
    void showstat_gps_pro(QProcess::ProcessState state);
    void showFini_gps_pro(int exitCode, QProcess::ExitStatus exitStatus);
    void disp_port_list(QStringList str_list);
    void comm_gps_readyRead();
    void comm_dev_readyRead();
    void comm_gps_delay();
    void comm_dev_delay();
    void Serial_gps_Read();
    void Serial_dev_Read();
    void autoScrollTextEdit(QTextEdit *textEdit,QString &text,QString &add_text);
    //多线程串口接收
    void on_receive_dev(QByteArray tmpdata);
    void on_receive_gps(QByteArray tmpdata);
    void on_receive_avi(QByteArray tmpdata);
    void on_receive_test(QByteArray tmpdata);
    void recvDowninfo_parser(QByteArray bytearr);

    void on_rd_btn_clicked(bool checked);

    void timeout_send();
    void socket_sendMsg();
    void timerUpdate();
    void timer_sendHeartBeatMsg();
    void pbutt_start_event();
    void pbutt_stop_event();

    void add_sendMsg();    //添加发送消息
    void on_discNetProcess(bool);   //断开网络处理
    void send_DevcmdReset4G();      //发送重启4G
};

#endif // WIDGET_H
