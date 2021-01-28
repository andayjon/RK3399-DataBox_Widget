#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QImage>
#include <QString>
#include <QFont>
#include <QColor>
#include <QMessageBox>
#include <QPaintEvent>
#include <QDebug>
#include <QPushButton>

typedef struct
{
    int x;
    int y;
    int center_x;
    int center_y;
}Img_loc_stc;

class dashBoard : public QWidget
{
    Q_OBJECT
public:
    explicit dashBoard(QWidget *parent = nullptr);
    ~dashBoard();
    double m_value;             //数值
    double m_netspeed;          //网速--bps
    double m_precision;         //精度
    int rssi;                   //信号强度
    quint16 delta_angle;        //角度增量
    QString m_title;            //标题
    int card_stat;              //卡状态
    Img_loc_stc img_location;   //图片位置
    bool En_repaint = true;

protected:
    void drawTitle(QPainter *);
    void drawSimCardStat(QPainter *);
    void drawNumericValue(QPainter *);
    void drawRSSI(QPainter *);
    void paintEvent(QPaintEvent *event);
private:
    QColor m_foreground;    //网速值显示颜色
    QColor m_cardground;    //卡状态显示颜色

    QImage m_disc;          //表盘
    QImage m_pointer;       //指针
    QTimer *m_tim;          //定时器
    quint16 curr_angle;     //当前角度
    quint16 target_angle;   //目标角度
    double last_netspeed;   //上次网速值
    bool m_dir;             //旋转方向
    double m_angle_res;     //角度分辨率
    quint16 count_angle;    //累加角度
    QString m_units;        //显示单位
    QFont fn_txt;
signals:

public slots:
    void slot_UpdateAngle();
    void slot_GetnetSpeed(double);
};

#endif // DASHBOARD_H
