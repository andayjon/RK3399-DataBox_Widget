#include "local_cmdinterface.h"

Local_CmdInterface::Local_CmdInterface(QObject *parent) : QObject(parent)
{
    m_zmqdealer = new ZmqDealer(this);
    m_EncAndDec = new base64_EncryAndDecry(this);
    m_Filedriver = new QFile_driver();
    connect(m_zmqdealer, &ZmqDealer::sigRecv, this, &Local_CmdInterface::slot_recvCmdMsg);
    m_zmqdealer->start();
}
Local_CmdInterface::~Local_CmdInterface()
{

}

void Local_CmdInterface::slot_recvCmdMsg(const QByteArray &msg)
{
    QByteArray hh = msg;
    QByteArray ba = m_EncAndDec->toXOREncUnc(hh, 'h');
    const QByteArray tmp_ba = m_EncAndDec->toUppLow(ba);
    const QByteArray decfy_ba = tmp_ba.fromBase64(tmp_ba);
    int ba_size = decfy_ba.size();
    if(decfy_ba.at(0) == '\xAA' && decfy_ba.at(1) == '\x55' && decfy_ba.at(ba_size-2) == '\x55' && decfy_ba.at(ba_size-1) == '\xAA') {
        int cmdCode = quint8(decfy_ba.at(3))*256 + quint8(decfy_ba.at(2));
        char checksum = decfy_ba.at(ba_size-3);
        QByteArray check_ba = decfy_ba.mid(2, ba_size-5);
        if(m_zmqdealer->m_zmqcmd->compChecksum(check_ba, checksum)) {
            QByteArray data = decfy_ba.mid(6, ba_size-9);
            analysis_Cmd(cmdCode, data);
        }
    }
}

void Local_CmdInterface::analysis_Cmd(int cmdCode, QByteArray msg)
{
    Zmqcmd_stc command;
    command.head[0] = '\xAA';
    command.head[1] = '\x55';
    command.cmdCode = 0;
    command.len = 0;
    command.checksum = 0;
    command.end[0] = '\x55';
    command.end[1] = '\xAA';

    QByteArray read_ba, base_ba, encry_ulba, encry_xorba;
    QByteArray cmd_ba;
    qDebug()<< QString("-->>Receive a command from the host computer.") <<endl;
    switch (cmdCode) {
    case 1001:  //read config
        if(msg.isEmpty()) {
            read_ba = m_Filedriver->readFile(f_name);
            command.cmdCode = 2001;
            if(!read_ba.isEmpty()) {
                command.len = read_ba.size();
            }else {
                command.cmdCode = 2001;
                command.len = 1;
                read_ba.clear();
                read_ba.append('\x0');
            }
        }
        break;
    case 1002:  //write config
        command.cmdCode = 2002;
        command.len = 1;
        if(m_Filedriver->ClearwriteFile(f_name, msg)) {
            read_ba.clear();
            read_ba.append('\x1');
        }else {
            read_ba.clear();
            read_ba.append('\x0');
        }
        break;
    default:
        break;
    }
    cmd_ba.prepend((char *)&command.cmdCode, 4);
    cmd_ba.append(read_ba, command.len);
    command.checksum = m_zmqdealer->m_zmqcmd->calChecksum(cmd_ba);
    cmd_ba.append(command.checksum);
    cmd_ba.prepend((char*)&command.head, 2);
    cmd_ba.append((char *)&command.end, 2);
    base_ba = cmd_ba.toBase64();
    encry_ulba = m_EncAndDec->toUppLow(base_ba);
    encry_xorba = m_EncAndDec->toXOREncUnc(encry_ulba, 'h');
    m_zmqdealer->sendData(encry_xorba);

}
