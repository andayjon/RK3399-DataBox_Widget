#ifndef THREAD_H
#define THREAD_H
#include<QThread>
#include<QString>

#define _TTY_ARMV4_
class QTcpSocket;
class QTextCodec;

class Thread : public QThread
{
    Q_OBJECT
public:
    Thread(QObject *parent);
    ~Thread();
    void startThread(const QString &ip, int port);
    void stopThread();

    QString m_QStrSocketIP;
    int m_nSockPort;
protected:
    virtual void run();
    //void run_start();
    bool ping_Ip(QString ip);
Q_SIGNALS:
    void send_toserver(QByteArray);
    void recv_formserver(QByteArray);
    void offline_server();
signals:

public slots:

protected slots:
    void onConnect();
    void onDisConnect();
    void onReadMSG();
    void onSendMSG(QByteArray);
private:
    QTcpSocket* m_TcpSocket;
    bool m_isThreaStopped;
    bool m_isOkConnect;
    QByteArray m_datagram;
};

#endif // THREAD_H
