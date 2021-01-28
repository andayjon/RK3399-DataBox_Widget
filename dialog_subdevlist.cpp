#include "dialog_subdevlist.h"

dialog_Subdevlist::dialog_Subdevlist(QWidget *parent) : QDialog(parent)
{
    setStyleSheet("QPushButton{"
                  "background-color:rgba(100,225,100,30);"//背景色（也可以设置图片）
                  "border-style:outset;"                  //边框样式（inset/outset）
                  "border-width:4px;"                     //边框宽度像素
                  "border-radius:10px;"                   //边框圆角半径像素
                  "border-color:rgba(255,255,255,30);"    //边框颜色
                  "font:bold 24px;"                       //字体，字体大小
                  "color:Qt::black;"                      //字体颜色
                  "padding:6px;"                          //填衬
                  "}"
                  //鼠标按下样式
                  "QPushButton:pressed{"
                  "background-color:rgba(100,255,100,200);"
                  "border-color:rgba(255,255,255,30);"
                  "border-style:inset;"
                  "color:rgba(0,0,0,100);"
                  "}"
                  //鼠标悬停样式
                  "QPushButton:hover{"
                  "background-color:rgba(100,255,100,100);"
                  "border-color:rgba(255,255,255,200);"
                  "color:rgba(0,0,0,200);"
                  "}"
                  );                     //正常状态样式
    QFont font;
    font.setPointSize(16);
    lab_Info.setFont(font);
    lab_Info.setText("请选择设备支持的协议类型.");
    lab_Info.resize(300,100);
    QFont bt_font;
    font.setPointSize(25);
    btn_Enter.setText("确定");
    btn_Enter.resize(100,100);
    btn_Enter.setFont(bt_font);
    btn_Enter.setStyleSheet("QPushButton{border-style: none; background-color: silver; font:bold 30px;}"
                       "QPushButton:hover{background-color:gray;color: white;}"
                       "QPushButton:pressed{background-color:slategray; border-style: inset; }");

    btn_Cencel.setText("取消");
    btn_Cencel.resize(100,100);
    btn_Cencel.setFont(bt_font);
    btn_Cencel.setStyleSheet("QPushButton{border-style: none; background-color: silver; font:bold 30px;}"
                       "QPushButton:hover{background-color:gray;color: white;}"
                       "QPushButton:pressed{background-color:slategray; border-style: inset; }");

    this->resize(400,300);
    this->setWindowTitle("选择协议类型");

    QHBoxLayout *m_hlayout = new QHBoxLayout;
    m_hlayout->addWidget(&btn_Cencel);
    m_hlayout->addWidget(&btn_Enter);

    QVBoxLayout *m_rblayout = new QVBoxLayout;
    btn_group = new QButtonGroup(this);
    rbtn_tcp.resize(100,100);
    rbtn_http.resize(100,100);
    rbtn_zmq.resize(100,100);
    QString rb_style = "QRadioButton::indicator{width:35px;height:35px;}";  //修改单选按钮样式
    rbtn_tcp.setStyleSheet(rb_style);
    rbtn_http.setStyleSheet(rb_style);
    rbtn_zmq.setStyleSheet(rb_style);
    rbtn_tcp.setFont(font);
    rbtn_http.setFont(font);
    rbtn_zmq.setFont(font);
    rbtn_tcp.setText("TCP协议");
    rbtn_http.setText("HTTP协议");
    rbtn_zmq.setText("ZMQ协议");
    m_rblayout->addWidget(&rbtn_tcp);
    m_rblayout->addWidget(&rbtn_http);
    m_rblayout->addWidget(&rbtn_zmq);
    rbtn_tcp.setChecked(true);

    QVBoxLayout *m_vlayout = new QVBoxLayout;
    m_vlayout->addWidget(&lab_Info);
    m_vlayout->addStretch(1);
    m_vlayout->addLayout(m_rblayout);
    m_vlayout->addStretch(2);
    m_vlayout->addLayout(m_hlayout);

    this->setLayout(m_vlayout);

    connect(&btn_Enter, &QPushButton::clicked, this, &dialog_Subdevlist::Btn_Enter_clicked);
}

dialog_Subdevlist::~dialog_Subdevlist()
{
    done(ret);
}

void dialog_Subdevlist::Btn_Enter_clicked()
{
    qDebug()<<tr("Press the OK key.")<<endl;
    if(rbtn_tcp.isChecked()) {
        ret = 1;
    }else if(rbtn_http.isChecked()) {
        ret = 2;
    }else if(rbtn_zmq.isChecked()) {
        ret = 3;
    }
    done(Accepted);
    emit sig_isOk(ret);
}

void dialog_Subdevlist::Btn_Cencel_clicked()
{
    qDebug()<<tr("Press the Cencel key.")<<endl;
    ret = 0;
    done(Rejected);
    emit sig_isCencel(ret);
}
