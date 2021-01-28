#ifndef SLIDERBUTTON_H
#define SLIDERBUTTON_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QColor>
#include <QTimer>
#include <QDebug>
#include <QLabel>

namespace Ui {
class SliderButton;
}

class SliderButton : public QWidget
{
    Q_OBJECT

public:
    explicit SliderButton(QWidget *parent = nullptr);
    ~SliderButton();

    void set_button_size(const int &w, const int &h);
    void set_button_color(const QColor & , const QColor & ,const QColor & );
    void set_button_text(QLabel *, const int &x,  const int &y,QString);     //2020-5-3
    void set_button_pos(const int &x, const int &y);            //2020-5-3

    int m_button_pos;       //按键位置
    int m_move_distance;    //移动距离
    bool m_button_status;   //按键当前状态

    int m_circle_width;     //小圆宽度
    int m_button_pos_x;     //按键位置x
    int m_button_pos_y;     //按键位置y

    int m_button_index;
    bool m_resp_mouse_event;    //是否响应鼠标事件

    QPainter *painter(QWidget *parent);
    QLabel *label_on;       //on标签
    QLabel *label_off;      //off标签
    QColor m_backcolor_on;      //on背景色
    QColor m_backcolor_off;     //off背景色
    QColor m_circle_color;      //小圆颜色
    QTimer *m_timer;            //定时器
 signals:
    void signal_button_on();
    void signal_button_off();
    void signal_button_click(QMouseEvent *);
    void signal_drawdesigns();
    void signal_update();
protected:
//    virtual void mousePressEvent(QMouseEvent *event);
//    virtual void paintEvent(QPaintEvent *event);
public slots:
    void slot_update();
    void slot_MousePressEvent(QMouseEvent *);
    void slot_drawDesigns();
private:

};

#endif // SLIDERBUTTON_H
