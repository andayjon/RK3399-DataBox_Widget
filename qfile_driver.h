#ifndef QFILE_DRIVER_H
#define QFILE_DRIVER_H

#include <QObject>
#include <QFile>
#include <QFileDevice>
#include <QIODevice>
#include <QTextStream>
#include <QByteArray>
#include <QDebug>
#include <QString>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

class QFile_driver : public QObject
{
    Q_OBJECT
public:
    explicit QFile_driver(QObject *parent = nullptr);
    ~QFile_driver();
    bool ClearwriteFile(QString, QByteArray &);
    bool AppendwriteFile(QString, QByteArray &);
    QByteArray readFile(QString);
    bool DeleteFile(QString);
protected:

signals:
    void sig_openFile(QString);
    void sig_readFile(QString);
public slots:
//    void slot_readFile(QString);
//    void slot_writeFile(QTextStream);

private:

};

#endif // QFILE_DRIVER_H
