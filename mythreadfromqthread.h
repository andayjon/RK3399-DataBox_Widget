#ifndef MYTHREADFROMQTHREAD_H
#define MYTHREADFROMQTHREAD_H

#include <QThread>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QNetworkConfigurationManager>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QHostInfo>
#include <QDebug>
class MyThreadFromQThread:public QThread
{
    Q_OBJECT
public:
    MyThreadFromQThread(QObject* parent);
    ~MyThreadFromQThread();
    void run();
    void setSignal();
    void resetSignal();
    bool isNetWorkOnline();
    void checkNetWorkOnline();
    QString ipAddress(QString network);
    QString macAddress(QString network);
    void GetNetInfo(QString network);
    void getHost();
signals:
    void disconNet(bool stat);
    void sigLookUpHostResult(bool);
public slots:
    void onLookupHost(QHostInfo);
private:
    bool signal;
};

#endif // MYTHREADFROMQTHREAD_H
