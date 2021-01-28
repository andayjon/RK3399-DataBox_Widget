#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H
#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonValue>

class tcp_client:public QObject
{
    Q_OBJECT
public:
    explicit tcp_client(QObject *parent);
    ~tcp_client();

    bool connectServer();
    void writeMsgToServer(QString str);
    QString server_ip;
    quint16 server_port;
    QJsonObject md_upMQ_info;   //微波侦测上传消息
    quint8 md_dev_stat;         //微波侦测设备状态
    QString localdevid;         //本地微波侦测设备型号
    int md_dev_id;              //厂家设备ID
    QString databox_id;         //数据盒ID

private:
    QTcpSocket *tcp_socket;
    bool m_isOkConect;

    QTimer *recon_timer;
    quint16 send_num = 0;
    QByteArray recMsg;

    QJsonObject js_devGps;
    QJsonArray js_devData;
    QJsonObject js_ObjData;
    QJsonArray js_ArrData;


    float Get_RandNumber(void);
    void analysisJson(QByteArray);
signals:
    void clear_allData();
    void clear_uavData();
    void add_allData();
private slots:
    void slot_onDisConnect();
    void slot_onConnect();
    void slot_readMsgFromServer();
    void slot_clear_allData();
    void slot_clear_uavData();
    void slot_add_allData();
};

#endif // TCP_CLIENT_H
