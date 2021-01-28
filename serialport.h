#include <QObject>
#include <QCoreApplication>
#include <QEventLoop>
#include <QSerialPort>
#include <QString>
#include <QByteArray>
#include <QObject>
#include <QDebug>
#include <QObject>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QMutex>
class SerialPort : public QObject
{
  Q_OBJECT
public:
  //explicit SerialPort(QObject *parent = NULL, QString comm_name, int baud);
    explicit SerialPort(QString comm_name, int baud);
    ~SerialPort();
    QTimer timer_out;
    void init_port(QString comm_name, int baud);  //初始化串口
    QByteArray buff;
public slots:
    void handle_data();  //处理接收到的数据
    void write_data(QByteArray arr);   //发送数据
    bool sleep_ms(int msec);
signals:
    //接收数据
    void receive_data(QByteArray tmp);

private:
    QThread *my_thread;
    QSerialPort *port;
};
