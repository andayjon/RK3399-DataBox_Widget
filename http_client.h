#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSsl>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>

#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

#include <QtGlobal>
#include <QUrl>
#include <QByteArray>
#include <QString>
#include <QObject>
#include <QEventLoop>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QTime>


//#define _DEBUG_
class Http_Client: public QObject
{
    Q_OBJECT
public:
    explicit Http_Client(QObject *parent);
    ~Http_Client();
    void Http_DetResults(QString);
    int Http_login(QString);
    int Http_logout(QString);
    void AnalysisJson(QByteArray *);
    float Get_RandNumber(void);
    QByteArray m_rcv_data;
    QString m_token;
    QVariant variant;
    QJsonObject json_MQ;
    QString def_User = "operator";
    QString def_Passwd = "UserMon1";
    QString md_User = "";
    QString md_Passwd = "";
    QString md_superUser = "super";
    QString md_superPasswd = "Superpower1";
    QString url_ip;
    QString url_login;  //"https://192.168.11.100:5000/api/v3/login";
    QString url_logout; //"https://192.168.11.100:5000/api/v3/logout";
    QString url_getsys; //"https://192.168.11.100:5000/api/v3/system";
    qint8 stat_code = 0;    //状态码
    bool log_stat = 0;      //登录状态
    QString databox_id;     //数据盒ID
    quint8 up_data_type;    //上传数据类型
    QString local_devid;    //本地设备ID
    bool online_stat;       //在线状态
    quint8 work_stat;       //工作状态 0-正常 1-警告 2-错误
private:
    QNetworkAccessManager *m_aManager;
    QNetworkRequest m_Request;
    QAuthenticator m_Auth;
    QUrl baseUrl;

    QByteArray mp_data;

    //QString url_login = "https://115.28.131.3:5000/api/v3/login";
    //QString url_logout = "https://115.28.131.3:5000/api/v3/logout";
    //QString url_getsys = "https://115.28.131.3:5000/api/v3/system";

    QNetworkCookie cookies;
    QTimer *tim_get;
    qint8 err_code = 0; 
    qint8 quest_code =0;
public slots:
    void replyFinished(QNetworkReply*);
    void AuthRequiredReply(QNetworkReply*,QAuthenticator*);
    void slot_clear_allData();
    void slot_questData();
    void rec_md_finished();
signals:
    void Signal_ReadRecvFinished();
    void Signal_AnalysisFinished();
    void clear_allData();
};

#endif // HTTP_CLIENT_H
