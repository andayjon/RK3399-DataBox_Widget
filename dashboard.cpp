#include "dashboard.h"
//#define DEBUG_EN
dashBoard::dashBoard(QWidget *parent) : QWidget(parent)
{
    m_pointer.load(":/Icon/dashboard1_260.png");
    m_disc.load(":/Icon/dashboard2_260.png");

    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setAttribute(Qt::WA_NoSystemBackground);
    m_title = "";
    card_stat = 0;
    m_precision = 0.1;
    m_netspeed = 0;
    m_units = "Mbs";
    curr_angle = 0;
    count_angle = 0;
    m_foreground = Qt::white;
    m_cardground = Qt::red;


    m_tim = new QTimer(this);
    connect(m_tim, SIGNAL(timeout()), this, SLOT(slot_UpdateAngle()));

#ifdef DEBUG_EN
    qDebug()<<tr("DashBoard x = %1, y = %2").arg(this->x()).arg(this->y())<<endl;
#endif
    m_tim->start(100);
}
dashBoard::~dashBoard()
{
    delete this;
}

void dashBoard::paintEvent(QPaintEvent *event)
{
    QImage *newImage = new QImage(m_disc);      //表盘
    QImage *mask = new QImage(m_pointer);       //表针
    QPainter painter;

    painter.begin(mask);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
    painter.translate(this->width()/2, this->height()/2);
    painter.rotate(curr_angle);
    painter.drawImage(-newImage->width()/2, -newImage->height()/2, *newImage);
//    painter.drawImage(-newImage->height()/2, -newImage->width()/2, *newImage);
//    painter.drawImage(0, 0, *newImage);
    painter.end();
    painter.begin(this);
    painter.drawImage(event->rect(), *mask);
    drawTitle(&painter);
    drawSimCardStat(&painter);
    drawNumericValue(&painter);
    drawRSSI(&painter);
    painter.end();

//    if(m_value < 250) {
//        m_value += 15;
//        angle = 360 - m_value;
//    }
    delete mask;
    delete newImage;
}
//更新角度
void dashBoard::slot_UpdateAngle()
{
    m_tim->stop();
    //检测到SIM卡和网速变化
    if(card_stat == 1) {
        //当前网速值在合理范围内
        if(m_netspeed > 0.0 &&m_netspeed < 65535.0) {
#ifdef DEBUG_EN
            qDebug()<<tr("-->>Dash Board m_netspeed = %1.").arg(m_netspeed)<<endl;
#endif
            if(m_netspeed > 1024.0) {
                m_value = m_netspeed/1024.0;
                m_units = "Mbs";
            } else {
                m_value = m_netspeed;
                m_units = "Kbs";
            }
            if(m_netspeed >0.0 && m_netspeed <=128.0) {
                target_angle = 360 - m_netspeed/23;
            }else if(m_netspeed > 128.0 && m_netspeed <= 256.0) {
                target_angle = 360 - (23+(m_netspeed - 128.0)/(128/23));
            }else if(m_netspeed > 256.0 && m_netspeed <= 512.0) {
                target_angle = 360 - (23*2+(m_netspeed - 256.0)/(256/23));
            }else if(m_netspeed > 512.0 && m_netspeed <= 1024.0) {
                target_angle = 360 - (23*3+(m_netspeed - 512.0)/(512/23));
            }else if(m_netspeed > 1024.0 && m_netspeed <= 2048.0) {
                target_angle = 360 - (23*4+(m_netspeed - 1024.0)/(1024/23));
            }else if(m_netspeed > 2048.0 && m_netspeed <= 5120.0) {
                target_angle = 360 - (23*5+(m_netspeed - 2048.0)/(3072/23));
            }else if(m_netspeed > 5120.0 && m_netspeed <= 10240.0) {
                target_angle = 360 - (23*6+(m_netspeed - 5120.0)/(5120/23));
            }else if(m_netspeed >10240.0 && m_netspeed <= 20480.0) {
                target_angle = 360 - (23*7+(m_netspeed - 10240.0)/(10240/23));
            }else if(m_netspeed > 20480.0 && m_netspeed <= 51200.0) {
                target_angle = 360 - (23*8+(m_netspeed - 20480.0)/(30720/23));
            }else if(m_netspeed > 51200.0) {
                target_angle = 360 - (116+24*4+(m_netspeed-51200.0)/(51200/23));
            }else {
                last_netspeed = 65535.0;
                target_angle = 270;
            }
//            target_angle = 360 - 255;
            //按照旋转方向步进1°
            if(target_angle != curr_angle) {
                //识别表盘正反转
                if(target_angle > curr_angle) {
                    m_dir = false;
                } else {
                    m_dir = true;
                }
                if(m_dir) {
                    count_angle += delta_angle;
                    curr_angle = 360 - count_angle;
                } else {
                    count_angle -= delta_angle;
                    curr_angle = 360 - count_angle;
                }
#ifdef DEBUG_EN
                qDebug()<<tr("DashBoard Update curr_angle =  %1, target_angle = %2").arg(curr_angle).arg(target_angle)<<endl;
#endif
            }
        } else {
#ifdef DEBUG_EN
            qDebug()<<tr("The Current Network Speed is 0")<<endl;
#endif
        }
    } else {
#ifdef DEBUG_EN
        qDebug()<<tr("No Check SIM CARD!")<<endl;
#endif
    }
    if(En_repaint)
        repaint();
    m_tim->start(100);
}
//绘制标题
void dashBoard::drawTitle(QPainter *painter)
{
//    quint8 bw = 6;
    painter->save();
    painter->setPen(m_foreground);
    //painter->setBrush(m_foreground);
    QString str(m_title);   //显示仪表的功能
    QFontMetricsF fm(this->font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    double h = fm.size(Qt::TextSingleLine,str).height();
    painter->drawText(this->width()/2-w/2+3, this->height()/2-35, str);
    painter->restore();
}
//SIM卡状态
void dashBoard::drawSimCardStat(QPainter *painter)
{
    painter->save();
    painter->setPen(m_cardground);
    //painter->setBrush(m_foreground);
    QString str; //显示SIM卡状态
    if(card_stat == 1) {
        m_cardground = Qt::green;
        str = QString(tr("Connect Ok"));
    } else if(card_stat == -1){
        m_cardground = Qt::red;
        str = QString(tr("Connect Error"));
    } else {
        m_cardground = Qt::white;
        str = QString(tr("Search Network"));
    }
    QFontMetricsF fm(this->font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    double h = fm.size(Qt::TextSingleLine,str).height();
    painter->drawText(this->width()/2-w/2, this->height()/2+h*3+16, str);
    painter->restore();

}
//显示的单位与数值
void dashBoard::drawNumericValue(QPainter *painter)
{

    QString str_pre;
    if(m_netspeed > 1024.0)
        str_pre = QString::number(m_netspeed/1024,'f',1);
    else
        str_pre = QString::number(m_netspeed,'f',1);
    QString str_un = m_units;
    QFontMetricsF fm(font());
//    double w = fm.size(Qt::TextSingleLine,str_un).width();
    double h = fm.size(Qt::TextSingleLine,str_un).height();
    painter->setPen(m_foreground);
    painter->drawText(this->width()/2+20, this->height()/2+h/2, str_pre);
    painter->drawText(this->width()/2+20, this->height()/2+h+h/2, str_un);
}
//显示信号强度
void dashBoard::drawRSSI(QPainter *painter)
{
    QString str = QString("%1%2").arg("RSSI:").arg(rssi);
    QFontMetricsF fm(font());
    double w = fm.size(Qt::TextSingleLine,str).width();
    double h = fm.size(Qt::TextSingleLine,str).height();
    painter->setPen(m_foreground);
    painter->drawText(this->width()/2-w/2, this->height()/2+h*2+16, str);
}
//获取当前网速槽函数
void dashBoard::slot_GetnetSpeed(double speed)
{
    last_netspeed = m_netspeed;
    m_netspeed = speed;
}
