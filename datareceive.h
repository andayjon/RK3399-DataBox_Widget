/**
 * @breaf 用于创建websocket连接
 * @note  该类实现了client端基本的websocket创建，自动重连。
 * @author  LittleMushroom
 * date   2018年5月15日
 */

#ifndef DATARECEIVE_H
#define DATARECEIVE_H

#include <QtCore/QObject>
#include <QtWebSockets/QWebSocket>
#include <QWebSocket>

#include <QObject>
#include <QString>
#include <QTimer>
#include <QTime>

//#define URL "ws://112.126.101.172:7001/websocket/bg/"      //
//#define URL "ws://192.168.50.206:7001/websocket/bg/123456789"     //本地服务
class DataReceive : public QObject
{
    Q_OBJECT
public:
    DataReceive(QObject *parent=0);
    ~DataReceive();
    QString URL;
    QString sever_ip;
    QString dev_id;
    bool connectStatus;         /*-<websocket连接状态，连接成功：true；断开：false */
public slots:
    void createDataRecvWS();    /*-<创建websocket连接 */
    void closeConnected();              /*关闭连接*/
    void sendTextmsg(QString msg);     /*发送文本消息*/
    void sendBinaryy(QByteArray arr);
    void reconnect();                  /*-<周期重连函数 */
private:
    QWebSocket *dataRecvWS;     /*-<websocket类 */
    QTimer *m_timer;            /*-<周期重连Timer */
    QString url;                /*Url 连接*/

signals:
    void recv_Msg_pro(QByteArray recv_arr);

private slots:
    void onConnected();                 /*-<socket建立成功后，触发该函数 */
    void onDisConnected();              /*-<socket连接断开后，触发该函数 */
    void onTextReceived(QString msg);   /*-<收到Sev端的数据时，触发该函数 */
};

#endif // DATARECEIVE_H
