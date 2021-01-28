#include "slider_button.h"

SliderButton::SliderButton(QWidget *parent) :
    QWidget (parent),
    m_button_status(false),
    m_circle_width(30),         //圆直径
    m_button_pos(0),            //按键位置
    m_move_distance(50),       //移动距离
    m_backcolor_on(Qt::red),    //on背景图层
    m_backcolor_off(Qt::blue),  //off背景图层
    m_circle_color(Qt::black)   //圆图层
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slot_update()));
    connect(this, SIGNAL(signal_drawdesigns()), this, SLOT(slot_drawDesigns()));
    connect(this, &SliderButton::signal_button_click, this, &SliderButton::slot_MousePressEvent);
    label_on = new QLabel(this);
    label_off = new QLabel(this);

}

SliderButton::~SliderButton()
{
}
//设置按键大小
void SliderButton::set_button_size(const int & width, const int &heigh)
{
    m_move_distance = width;    //滑动距离
    m_circle_width = heigh;     //圆直径
}
//设置按键颜色
void SliderButton::set_button_color(const QColor &on_color, const QColor &off_color, const QColor &button_color)
{
    QPalette palette;

    m_backcolor_on = on_color;      //on图层
    m_backcolor_off = off_color;    //off图层
    m_circle_color = button_color;  //圆图层

    palette.setColor(QPalette::WindowText,Qt::black);
    label_off->setPalette(palette);
    //label_on->setFont(font());

    palette.setColor(QPalette::WindowText,Qt::black);
    label_on->setPalette(palette);
}
//设置按钮位置
void SliderButton::set_button_pos(const int &x, const int &y)
{
    m_button_pos_x = x;
    m_button_pos_y = y;
    m_button_pos = m_button_pos_x;
}
//设置按钮文字
void SliderButton::set_button_text(QLabel *label, const int &x, const int &y, QString str)
{
    QFont txt_Font;
    txt_Font.setFamily("SimHei");
    txt_Font.setBold(true);
    txt_Font.setPointSize(m_circle_width/2);
    label->setFont(txt_Font);
    QPalette pal;
    pal.setColor(QPalette::WindowText,Qt::white);
    label->setPalette(pal);
    label->setGeometry(x, y, m_circle_width+m_move_distance/2, m_circle_width);
    label->setText(str);
    label->setVisible(true);
}
void SliderButton::slot_MousePressEvent(QMouseEvent *event)
{
    if(m_resp_mouse_event) {
        if((event->pos().x() > m_button_pos_x) && (event->pos().x() < (m_button_pos_x + m_move_distance + m_circle_width))
                && (event->pos().y() > m_button_pos_y) && (event->pos().y() < m_button_pos_y+m_circle_width)) {
            qDebug()<<tr("slider_button %1 mousePressEvent!").arg(m_button_index)<<endl;
            if (false == m_button_status)
            {
                m_button_status = true;
                emit signal_button_off();

            }
            else
            {
                m_button_status = false;
                emit signal_button_on();
            }
            m_timer->start(1);
        }
    }
}
#if 0
//响应鼠标事件
void SliderButton::mousePressEvent(QMouseEvent *event)
{

    qDebug()<<tr("slider_button mousePressEvent!")<<endl;
    if((event->pos().x() > m_button_pos_x) && (event->pos().x() < (m_button_pos_x+m_move_distance+m_circle_width))
            && (event->pos().y() > m_button_pos_y) && (event->pos().y() < m_button_pos_y+m_circle_width)) {
        if (false == m_button_status)
        {
            m_button_status = true;
            emit signal_button_off();
        }
        else
        {
            m_button_status = false;
            emit signal_button_on();
        }
        m_timer->start(1);
    }
}
#endif
//画图形
void SliderButton::slot_drawDesigns()
{

    QPainter painter(this);
    QPainterPath path;
    QFont font;

    painter.setRenderHint(QPainter::Antialiasing, true);


    if (m_button_status == true)
    {
        painter.setBrush(m_backcolor_off);
        label_on->setVisible(false);
        set_button_text(label_off, m_button_pos_x+m_circle_width/2, m_button_pos_y, "On");
    }
    else
    {
        painter.setBrush(m_backcolor_on);
        label_off->setVisible(false);
        set_button_text(label_on, m_button_pos_x+m_move_distance+m_circle_width/5, m_button_pos_y, "Off");
    }
    //画背景滑动条
    QRect rect (m_button_pos_x, m_button_pos_y, m_circle_width, m_circle_width);
    int startX = rect.left()  + rect.width()  /  2;
    int startY = rect.top();
    path.moveTo(startX,startY);
    path.arcTo(QRect(rect.left(), rect.top(), rect.width(), rect.height()),90,180);
    path.lineTo((rect.left()  + m_move_distance ),  rect.bottom()  + 1);   // the graph not connet , neet 1 pixcel
    path.arcTo(QRect((startX + m_move_distance),rect.top(),rect.width(),rect.height()),270,180);
    path.lineTo(startX,startY);
    painter.drawPath(path);

    // draw small circle 画小圆
    painter.setBrush(m_circle_color);
    painter.drawEllipse(m_button_pos ,m_button_pos_y,m_circle_width,m_circle_width);
}
#if 0
//画图事件
void SliderButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QPainterPath path;
    QFont font;

    painter.setRenderHint(QPainter::Antialiasing, true);


    if (m_button_status == true)
    {
        painter.setBrush(m_backcolor_off);
        label_on->setVisible(false);
        set_button_text(label_off, m_button_pos_x+m_circle_width/2, m_button_pos_y, "ON");
    }
    else
    {
        painter.setBrush(m_backcolor_on);
        label_off->setVisible(false);
        set_button_text(label_on, m_button_pos_x+m_move_distance+m_circle_width/4, m_button_pos_y, "OFF");
    }
    //画背景滑动条
    QRect rect (m_button_pos_x, m_button_pos_y, m_circle_width, m_circle_width);
    int startX = rect.left()  + rect.width()  /  2;
    int startY = rect.top();
    path.moveTo(startX,startY);
    path.arcTo(QRect(rect.left(), rect.top(), rect.width(), rect.height()),90,180);
    path.lineTo((rect.left()  + m_move_distance ),  rect.bottom()  + 1);   // the graph not connet , neet 1 pixcel
    path.arcTo(QRect((startX + m_move_distance),rect.top(),rect.width(),rect.height()),270,180);
    path.lineTo(startX,startY);
    painter.drawPath(path);

    // draw small circle 画小圆
    painter.setBrush(m_circle_color);
    painter.drawEllipse(m_button_pos ,m_button_pos_y,m_circle_width,m_circle_width);


}
#endif
//更新槽函数
void SliderButton::slot_update()
{
    if (m_button_status == true)
    {
        m_button_pos += 1;
        if (m_button_pos == m_button_pos_x + m_move_distance + m_circle_width / 2)
        {
            m_timer->stop();
        }
    }
    else if(m_button_status == false)
    {
        m_button_pos -= 1;
        if (m_button_pos == m_button_pos_x)
        {
            m_timer->stop();
        }
    }
    update();
}
