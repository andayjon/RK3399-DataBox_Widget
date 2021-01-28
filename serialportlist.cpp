#include "serialportlist.h"

//Ĭ�Ϲ���
SerialPortList::SerialPortList(QWidget *parent)
{
    timer = new QTimer;
    scanCycleMs = 2000;
    oldPortStringList.clear();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

//�Զ���ɨ�����ڹ���
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
    //�ӽ��ջ������ж�ȡ����
    recv_buf = comm.readAll();
}

SerialPortList::~SerialPortList()
{
    delete timer;
}

//��ʼɨ��
void SerialPortList::ScanStart()
{
    timer->stop();
    timer->start(scanCycleMs);
}

//ֹͣɨ��
void SerialPortList::ScanStop(){
    timer->stop();
}

//����ɨ�����
void SerialPortList::onTimeOut(){

    QStringList newPortStringList;
    //��������
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
      #if DEBUG_INFOR_EN
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
      #endif
      newPortStringList += info.portName();
    }

    //���¾ɵĴ����б�
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
