#include "bridge.h"
#include <QMessageBox>
#include <QJSEngine>
#include <QDebug>

//#define DEBUG_EN

bridge::bridge()
{
}

void bridge::getYourName()
{
    emit sigYourName("hello, world!");
}

void bridge::getCoordinate(QString lon, QString lat)
{
#ifdef DEBUG_EN
    qDebug()<<lon<<lat;
#endif
}
