#include "mythreadfromqthread.h"

MyThreadFromQThread::MyThreadFromQThread(QObject* parent):QThread(parent)
{

}
MyThreadFromQThread::~MyThreadFromQThread()
{
    qDebug()<<"Close MyThreadFromQThread."<<endl;
}
//线程运行
void MyThreadFromQThread::run()
{
    int count = 0;
    QString str_macaddr;
    QString str_ipaddr;
    qDebug()<<"Start run MyThread."<<endl;
    while(signal) {
        //qDebug()<<"Thread :"<<count++<<endl;
#if 0
        if(this->isNetWorkOnline()) {
            qDebug()<<"Curr NetWork status:"<<"connect."<<endl;
        }
        else {
            qDebug()<<"Curr NetWord status:"<<"no connect."<<endl;
        }
#endif
        checkNetWorkOnline();
        //GetNetInfo("eth1");
        sleep(3);
    }
    signal = false;
}
//设置信号
void MyThreadFromQThread::setSignal()
{
    qDebug()<<"Set MyThread signal is True."<<endl;
    this->signal = true;
}
//清除信号
void MyThreadFromQThread::resetSignal()
{
    qDebug()<<"Clear MyThread signal is false."<<endl;
    this->signal = false;
}
//检测网络信号
bool MyThreadFromQThread::isNetWorkOnline()
{
    QNetworkConfigurationManager mgr;
    return mgr.isOnline();
}
//检测网络连接
void MyThreadFromQThread::checkNetWorkOnline()
{
    QHostInfo::lookupHost("www.baidu.com",this,SLOT(onLookupHost(QHostInfo)));
}

void MyThreadFromQThread::onLookupHost(QHostInfo host)
{
    if(host.error() != QHostInfo::NoError) {
        qDebug()<<"Lookup failed:"<<host.errorString()<<endl;
        emit sigLookUpHostResult(false);
    }
    else {
        qDebug()<<"Lookup successful."<<endl;
        emit sigLookUpHostResult(true);
    }
}

//
QString MyThreadFromQThread::ipAddress(QString network)
{
    QList<QNetworkAddressEntry> list;
    QString localIPaddress;

    QNetworkInterface interface = QNetworkInterface::interfaceFromName(network);
    list = interface.addressEntries();
    if (!list.isEmpty())
    {
        localIPaddress = list.at(0).ip().toString();
    }

    return localIPaddress;
}
//
QString MyThreadFromQThread::macAddress(QString network)
{
    QList<QNetworkInterface> netList;
    int netCount;
    int neti;
    QNetworkInterface myNet;
    netList = QNetworkInterface::allInterfaces();
    netCount = netList.count();

    for (neti = 0; neti < netCount; neti ++)
    {
        if (netList.at(neti).isValid() && netList.at(neti).name() == network)
        {
            myNet = netList.at(neti);
            break;
        }
    }
    return (myNet.hardwareAddress());
}
//
void MyThreadFromQThread::GetNetInfo(QString network)
{
    QString ipaddr;
    QString macaddr;

    ipaddr = ipAddress(network);
    macaddr = macAddress(network);
    qDebug()<<"Get IP Address:"<<ipaddr;
    qDebug()<<"Get Mac Address:"<<macaddr;
}
void MyThreadFromQThread::getHost()
{
    //获取主机名
    QString localHostName = QHostInfo::localHostName();
    qDebug() << "localHostName:" + localHostName;

    //获取Ip
    QHostInfo info = QHostInfo::fromName(localHostName);
    QList<QHostAddress> list = info.addresses();
    if(!list.isEmpty())
    {
        qDebug() << "IP:" + list.first().toString();
    }

    //我们还可以获取更多

    //返回在主机上找到的所有网络接口的列表
    QList<QNetworkInterface> listImpl = QNetworkInterface::allInterfaces();
    if(!listImpl.isEmpty())
    {
        for (int i = 0; i < listImpl.length(); ++i) {
            QNetworkInterface interface = listImpl.at(i);
            qDebug() << "Device:" + interface.name();
            qDebug() << "HardwareAddress:" + interface.hardwareAddress();
            //返回该接口所拥有的IP地址列表及其相关的网络掩码和广播地址。
            QList<QNetworkAddressEntry> enrtyList = interface.addressEntries();
            for (int j = 0; j < enrtyList.length(); ++j) {
                QNetworkAddressEntry entry = enrtyList.at(j);
                qDebug() << "IP:" + entry.ip().toString();
                qDebug() << "netmask:" + entry.netmask().toString();
                qDebug() << "broadcast:" + entry.broadcast().toString();
            }
        }
    }
}
