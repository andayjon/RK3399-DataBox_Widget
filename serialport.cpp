#include "serialport.h"

SerialPort::SerialPort(QString comm_name, int baud)
{
    my_thread = new QThread();
    buff.resize(1024);
    port = new QSerialPort();
    init_port(comm_name, baud);
    this->moveToThread(my_thread);
    port->moveToThread(my_thread);
    my_thread->start();  //启动线程

}

SerialPort::~SerialPort()
{
    port->close();
    port->deleteLater();
    my_thread->quit();
    my_thread->wait();
    my_thread->deleteLater();
}
//初始化串口
void SerialPort::init_port(QString comm_name, int baud)
{
    port->setPortName(comm_name);                      //串口名 windows下写作COM1
    port->setBaudRate(baud);                           //波特率
    port->setDataBits(QSerialPort::Data8);             //数据位
    port->setStopBits(QSerialPort::OneStop);           //停止位
    port->setParity(QSerialPort::NoParity);            //奇偶校验
    port->setFlowControl(QSerialPort::NoFlowControl);  //流控制
    if (port->open(QIODevice::ReadWrite))
    {
        qDebug()<<comm_name<<" Port have been opened";
    }
    else
    {
        qDebug()<<comm_name<<" open it failed";
    }
    connect(port, SIGNAL(readyRead()), this, SLOT(handle_data()), Qt::QueuedConnection); //Qt::DirectConnection
}
//接收数据处理
void SerialPort::handle_data()
{
    //QTime rx_time = QTime::currentTime();
    QByteArray data;
    data.resize(1024*4);
    //sleep_ms(10);
    //while(port->waitForReadyRead(100))
        data = port->readAll();

    //qDebug()<<port->portName()<<":Receive time--"<<rx_time.toString();
    //buff.append(data);
    //qDebug() << QStringLiteral("data received(收到的数据):")<<data;
    //qDebug() << "handing thread is:" << QThread::currentThreadId();
    emit receive_data(data);
}
//串口写入函数
void SerialPort::write_data(QByteArray arr)
{
    QByteArray tmp_arr;
    tmp_arr.resize(3);
    tmp_arr[0] = 0x30;
    tmp_arr[1] = 0x31;
    tmp_arr[2] = 0x32;
    qDebug() << "write_id is:" << QThread::currentThreadId();
    qDebug()<<"dev_SerialPort = "<<arr.toHex()<<endl;
    port->write(arr, arr.size());   //发送数据
}
//延时函数
bool SerialPort::sleep_ms(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    return true;
}
