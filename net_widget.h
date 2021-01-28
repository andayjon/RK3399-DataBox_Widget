#ifndef NET_WIDGET_H
#define NET_WIDGET_H
#include "card_dialog.h"
#include "speed.h"
#include "dashboard.h"
#include "net_dialog.h"
#include <QMouseEvent>
#include <QMutex>
#include <QReadWriteLock>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>
#include <QtNetwork>
#include <QtNetwork/QNetworkAccessManager>
#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QSlider>
#include <QTextEdit>
#include <QGroupBox>
#include <QRadioButton>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QImage>
#include <QPalette>
#include <QFont>
#include <QDebug>
#include <QTime>
#include <QTimer>


typedef struct{
    bool card_stat = false;
    int card_rssi = 0;
    QString card_nettype = "";
    QString card_imsi = "";
    QString card_iccid = "";
    double card_rxspeed = 0;
    double card_txspeed = 0;
}Card_Info_st;

typedef struct {
    QString operator_info = "";
    QString model_number = "";
    QString model_imei = "";
    QString net_type = "";
    QString ip_addr = "";
}Model_Info_st;

typedef struct{
    bool bond_stat = false;
    QString bond_ip = "";
    bool wan_stat = false;
    QString wan_ip = "";
    double total_txspeed = 0;
    double total_rxspeed = 0;
    double wan_txspeed = 0;
    double wan_rxspeed = 0;
    Model_Info_st model_array[4];
    Card_Info_st card_array[4];
}Public_Info_st;

class Net_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Net_Widget(QWidget *parent = nullptr);
    ~Net_Widget();
    Card_Dialog *m_cardInfo;
protected:
    void mousePressEvent(QMouseEvent *event);
private:
    int wid_x1;
    int wid_y1;
    QLabel *label1;
    QLabel *label2;

    Speed *sp_dial1;
    Speed *sp_dial2;
    Speed *sp_dial3;
    QSlider *sliderh;

    dashBoard *db_1;
    dashBoard *db_2;
    dashBoard *db_3;

    bool net_creat_flag = false;
    bool dialog_dispFlag = false;
    Public_Info_st public_info;
    quint8 query_index = 0;
    QTimer *tim_setNetspeed;
    QTimer *tim_getNetinfo;
    QNetworkAccessManager *manager;
    QMutex mutex_md;            //操作锁
    QReadWriteLock m_rwlock;    //读写锁

    quint32 GetrandNum();
    void analysisJson(QByteArray);
    void Update_NetInfo(Card_Dialog *);
//    virtual void mousePressEvent(QMouseEvent);
signals:
    void sn_setNetspeed1(double);
    void sn_setNetspeed2(double);
    void sn_setNetspeed3(double);
    void sn_getspeed();
public slots:
    void sliderhValue(int);
    void slot_timOut();
    void slot_timgetOut();
    void replyFinished(QNetworkReply *);
    void slot_dialogRet(int);
};

#endif // NET_WIDGET_H
