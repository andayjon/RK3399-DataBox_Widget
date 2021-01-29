#include "base64_encryanddecry.h"

base64_EncryAndDecry::base64_EncryAndDecry(QObject *parent) : QObject(parent)
{

}
base64_EncryAndDecry::~base64_EncryAndDecry()
{

}
//加密Base64数据
QByteArray base64_EncryAndDecry::toBase64Encrypt(const QByteArray &ba)
{
    QByteArray ret_ba;
    if(!ba.isEmpty()) {
        ret_ba = ba.toBase64();
        return ret_ba;
    }
    return ba;
}
//解密Base64数据
QByteArray base64_EncryAndDecry::toBase64Uncrypt(const QByteArray &ba)
{
    QByteArray ret_ba;
    if(!ba.isEmpty()) {
        ret_ba = ba.fromBase64(ba);
        return ret_ba;
    }
    return ba;
}

// 大小写加解密
QString base64_EncryAndDecry::toUpperAndLower(QString src)
{
    for(int i = 0; i < src.count(); i++) {
        QChar curC = src.at(i);
        if(curC.isUpper()) {
            curC = curC.toLower();
        } else if(curC.isLower()){
            curC = curC.toUpper();
        }
        src[i] = curC;
    }
    return src;
}
QByteArray base64_EncryAndDecry::toUppLow(QByteArray &ba)
{
    for(int i = 0; i < ba.size(); i++) {
        QChar curC = ba.at(i);
        if(curC.isUpper()) {
            curC = curC.toLower();
        }else if(curC.isLower()) {
            curC = curC.toUpper();
        }
        ba[i] = curC.toLatin1();
    }
    return ba;
}
//大小写加密
QByteArray base64_EncryAndDecry::toUppLowEncrypt(const QByteArray &ba)
{
    QByteArray base = ba.toBase64();
    QByteArray ret_ba = toUpperAndLower(QString(base)).toUtf8();
    return ret_ba;
}

//大小写解密
QByteArray base64_EncryAndDecry::toUppLowUncrypt(const QByteArray &ba)
{
    QByteArray base = toUpperAndLower(QString(ba)).toUtf8();
    QByteArray ret_ba = ba.fromBase64(base);
    return ret_ba;
}
// 异或加解密
QString base64_EncryAndDecry::toXOREncryptUncrypt(QString src, const QChar key)
{
    for(int i = 0; i < src.size(); i++) {
        src[i] = src.at(i).toLatin1() ^ key.toLatin1();
    }
    return src;
}

//异或加密
QByteArray base64_EncryAndDecry::toXOREncrypt(const QByteArray &ba, const QChar key)
{
    QByteArray base = ba.toBase64();
    QByteArray ret_ba = toXOREncryptUncrypt(QString(base), key).toUtf8();
    return ret_ba;
}
QByteArray base64_EncryAndDecry::toXOREncUnc(QByteArray &ba, const QChar key)
{
    for(int i = 0; i < ba.size(); i++) {
        ba[i] = ba.at(i) ^ key.toLatin1();
    }
    return ba;
}
//异或解密
QByteArray base64_EncryAndDecry::toXORUncrypt(const QByteArray &ba, const QChar key)
{
    QByteArray base = ba.fromBase64(ba);
    QByteArray ret_ba = toXOREncryptUncrypt(QString(base), key).toUtf8();
    return ret_ba;
}
