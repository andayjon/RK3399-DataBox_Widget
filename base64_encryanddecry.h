#ifndef BASE64_ENCRYANDDECRY_H
#define BASE64_ENCRYANDDECRY_H

#include <QObject>
#include <QString>
#include <QByteArray>

class base64_EncryAndDecry:public QObject
{
    Q_OBJECT
public:
    explicit base64_EncryAndDecry(QObject *parent = nullptr);
    ~base64_EncryAndDecry();
    //Base64加密
    QByteArray toBase64Encrypt(const QByteArray &);
    QByteArray toBase64Uncrypt(const QByteArray &);
    //大小写加密
    QByteArray toUppLow(QByteArray &);
    QString toUpperAndLower( QString src);
    QByteArray toUppLowEncrypt(const QByteArray &);
    QByteArray toUppLowUncrypt(const QByteArray &);
    //异或加解密
    QByteArray toXOREncUnc(QByteArray &, const QChar);
    QString toXOREncryptUncrypt(QString src, const QChar);
    QByteArray toXOREncrypt(const QByteArray &, const QChar);
    QByteArray toXORUncrypt(const QByteArray &, const QChar);
};

#endif // BASE64_ENCRYANDDECRY_H
