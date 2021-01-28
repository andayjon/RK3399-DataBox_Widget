#include "serialportlist.h"

//默认构造
SerialPortList::SerialPortList(QWidget *parent)
{
    timer = new QTimer;
    scanCycleMs = 2000;
    oldPortStringList.clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

//自定义扫描周期构造
SerialPortList::SerialPortList(quint16 CycleMs)
{
    timer = new QTimer;
    scanCycleMs = CycleMs;
    oldPortStringList.clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}


//
void SerialPortList::SerialPort_Read(QSerialPort comm, QByteArray recv_buf)
{
    //从接收缓冲区中读取数据
    recv_buf = comm.readAll();
}

SerialPortList::~SerialPortList()
{
    delete timer;
}

//开始扫描
void SerialPortList::ScanStart()
{
    timer->stop();
    timer->start(scanCycleMs);
}

//停止扫描
void SerialPortList::ScanStop(){
    timer->stop();
}

//周期扫描服务
void SerialPortList::onTimeOut(){

    QStringList newPortStringList;
    //搜索串口
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
      #if DEBUG_INFOR_EN
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
      #endif
      newPortStringList += info.portName();
    }

    //更新旧的串口列表
    if(newPortStringList.size() != oldPortStringList.size())
    {
        oldPortStringList = newPortStringList;
        emit onNewSerialPort(oldPortStringList);
    }
    //this->ScanStop();
}

void SerialPortList::setcomm_name(QString comm_name, QString name)
{
    comm_name = name;
}
