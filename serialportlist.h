#ifndef SERIALPORTLIST_H
#define SERIALPORTLIST_H

#include <QTimer>
#include <QDebug>
#include <QSerialPort>
#include <QStringList>
#include <QSerialPortInfo>
#include <QWidget>
#include <QString>
#include <QObject>
/* @������
 * worldsing.cnblogs.com
 *
 * @���ܣ�
 * SerialPortList ��ʵ�ֵ�ǰ���õĴ��ڽ���ʵʱɨ�裬���������µĴ���
 * �������д�����ʧʱ��SerialPortList�ཫ����һ��QStringList���͵�
 * �ź�onNewSerialPort��QStringList���Ͷ����д�ŵ�ǰ���õĴ����豸��
 *
 * @ʹ�÷�����
 * 1��Qt�汾��Ҫ����5.1���ϣ�
 *
 * 2�����½�Qt�����ļ���ʱ��Ҫ����QSerialPortģ�飻������xxx.pro�ļ�
 *    �м��� Qt += serialport
 *
 * 3����Ӧ�ó����ж���һ���ܣ�������ӦSerialPortList�źţ���ȡ�����б�
 *    void onNewPortList(QStringList portName){
 *     ui->comboBoxComList->clear();
 *     ui->comboBoxComList->addItems(portName);
 *    }
 *
 * 4����Ӧ�ó����й���SerialProt��������onNewSerialPort�źţ�
 *   portList = new SerialPortList(200);
 *   connect(portList, SIGNAL(onNewSerialPort(QStringList)),\
 *           this,     SLOT(onNewPortList(QStringList)));
 *
 * 5������ɨ��
 *   portList->ScanStart();
 */

#define  DEBUG_INFOR_EN   0


class SerialPortList : public QWidget
{
    Q_OBJECT

public:
    SerialPortList(QWidget *parent);
    SerialPortList(quint16);
    ~SerialPortList();

    quint16 scanCycleMs;
    QTimer *timer;
    QStringList oldPortStringList;
//    QSerialPort comm_qx;       //serialport class
//    QString comm1_name;      //serialport name
//    quint32 comm1_baud;       //serialport baud
//    QSerialPort comm_dev;
//    QString comm2_name;
//    quint32 comm2_baud;
    void SerialPort_Read(QSerialPort comm, QByteArray recv_buf);
    void SerialPort_Send(QSerialPort comm, QByteArray send_buf);
    void ScanStart();
    void ScanStop();
    void setcomm_name(QString comm_name, QString name);

private:


public slots:
    void onTimeOut();

signals:
    void onNewSerialPort(QStringList);

};

#endif // SERIALPORTLIST_H
