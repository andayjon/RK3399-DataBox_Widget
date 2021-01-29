#ifndef LOCAL_CMDINTERFACE_H
#define LOCAL_CMDINTERFACE_H

#include "zmqdealer.h"
#include "base64_encryanddecry.h"
#include "qfile_driver.h"
#include <QObject>
#include <QtDebug>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIODevice>

class Local_CmdInterface : public QObject
{
    Q_OBJECT
public:
    explicit Local_CmdInterface(QObject *parent = nullptr);
    ~Local_CmdInterface();
    ZmqDealer *m_zmqdealer;
signals:

public slots:
    void slot_recvCmdMsg(const QByteArray &);
private:

    base64_EncryAndDecry *m_EncAndDec;
    QFile_driver *m_Filedriver;

    QString f_name = "./dev_info";
    void start_Thread();
    void stop_Thread();
    void analysis_Cmd(int, QByteArray);
};

#endif // LOCAL_CMDINTERFACE_H
