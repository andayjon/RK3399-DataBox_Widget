#include "qfile_driver.h"

QFile_driver::QFile_driver(QObject *parent) : QObject(parent)
{

}
QFile_driver::~QFile_driver()
{

}
////追加写入
bool QFile_driver::AppendwriteFile(QString f_name, QByteArray &ba)
{
    QFile m_file(f_name);

   if(!m_file.exists()) {
       qDebug()<< tr("文件不存在！") <<endl;
       return false;
   }
   if(!m_file.open(QIODevice::ReadWrite | QIODevice::Append)) {
       qDebug()<< tr("文件打开失败！") <<endl;
      return false;
   }
   QTextStream out(&m_file);
   out<<ba<<endl;
   m_file.close();
   return true;
}
//清除写入
bool QFile_driver::ClearwriteFile(QString f_name, QByteArray &ba)
{
    QFile m_file(f_name);

    if(!m_file.exists()) {
        //qDebug()<< tr("文件不存在！开始创建文件") <<endl;
        m_file.open(QIODevice::WriteOnly);
        m_file.close();
    }
    if(!m_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        //qDebug()<< tr("文件打开失败！") <<endl;
        return false;
    }
    m_file.write(ba);
    m_file.close();
    return true;
}
//读取文件
QByteArray QFile_driver::readFile(QString f_name)
{
    QFile m_file(f_name);

    if(!m_file.exists(f_name)) {
        //qDebug()<< tr("读取文件不存在.")<<endl;
        return 0;
    }
    if(!m_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //qDebug()<< tr("文件打开失败！") <<endl;
        return 0;
    }
    QByteArray ret_ba;
    QTextStream in(&m_file);

    while (!in.atEnd()) {
        ret_ba += in.readLine();
        ret_ba.append('\n');
    }
     m_file.close();
    return ret_ba;
}
////删除文件
bool QFile_driver::DeleteFile(QString f_name)
{
    QFile m_file(f_name);

    if(!m_file.exists()) {
        qDebug()<< tr("Delete File is not exists.")<<endl;
        return false;
    }
    bool ret = m_file.remove(m_file.fileName());
    return ret;
}
