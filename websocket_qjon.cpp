#include "websocket_qjon.h"
#include <QDebug>

Web_qjon::Web_qjon()
{
    DataReceive *w_socket = new DataReceive(this);
    QString URL = "ws://112.126.101.172:7001/websocket/bg/10001";
    QObject::connect(this,SIGNAL(startConnect(QString)),w_socket,SLOT(createDataRecvWS(QString)));
    QObject::connect(this,SIGNAL(closeConnect()),w_socket,SLOT(closeConnected()));
    QObject::connect(this,SIGNAL(sendMsg(QString)),w_socket,SLOT(sendTextmsg(QString)));
    emit this->startConnect(URL);
    qDebug()<<"Connect URL = "<<URL;
}
void Web_qjon::readDevInfo()
{

}
void Web_qjon::sleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
