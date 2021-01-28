#include "compass_dialog.h"

Compass_Dialog::Compass_Dialog(QWidget *parent) : QDialog(parent)
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
    m_labInfo.setFont(font);
    m_labInfo.setText("请选择一种校准方式.");
    m_labInfo.resize(300,100);

    QFont bt_font;
    font.setPointSize(25);
    m_pbEnter.setText("确定");
    m_pbEnter.resize(100,100);
    m_pbEnter.setFont(bt_font);
    m_pbEnter.setStyleSheet("QPushButton{border-style: none; background-color: silver; font:bold 30px;}"
                       "QPushButton:hover{background-color:gray;color: white;}"
                       "QPushButton:pressed{background-color:slategray; border-style: inset; }");

    m_pbCencel.setText("取消");
    m_pbCencel.resize(100,100);
    m_pbCencel.setFont(bt_font);
    m_pbCencel.setStyleSheet("QPushButton{border-style: none; background-color: silver; font:bold 30px;}"
                       "QPushButton:hover{background-color:gray;color: white;}"
                       "QPushButton:pressed{background-color:slategray; border-style: inset; }");

    this->resize(400,300);
    this->setWindowTitle("校准方式选择");
    this->setStyleSheet("background-color:white; font:bold 16px;");

    QHBoxLayout *m_hlayout = new QHBoxLayout;
    m_hlayout->addWidget(&m_pbCencel);
    m_hlayout->addWidget(&m_pbEnter);

    QHBoxLayout *m_rblayout = new QHBoxLayout;
    m_pbGroup = new QButtonGroup(this);
    m_rbHorCali.resize(100,100);
    m_rbMulitCali.resize(100,100);
    QString rb_style = "QRadioButton::indicator{width:35px;height:35px;}";  //修改单选按钮样式
    m_rbHorCali.setStyleSheet(rb_style);
    m_rbMulitCali.setStyleSheet(rb_style);
    m_rbHorCali.setFont(font);
    m_rbMulitCali.setFont(font);
    m_rbHorCali.setText("水平面校准");
    m_rbMulitCali.setText("多面校准");
    m_rblayout->addWidget(&m_rbHorCali);
    m_rblayout->addWidget(&m_rbMulitCali);
    m_rbHorCali.setChecked(true);

    QVBoxLayout *m_vlayout = new QVBoxLayout;
    m_vlayout->addWidget(&m_labInfo);
    m_vlayout->addStretch(1);
    m_vlayout->addLayout(m_rblayout);
    m_vlayout->addStretch(2);
    m_vlayout->addLayout(m_hlayout);

    this->setLayout(m_vlayout);

    connect(&m_pbEnter, &QPushButton::clicked, this, &Compass_Dialog::on_pbEnter_clicked );
    connect(&m_pbCencel, &QPushButton::clicked, this, &Compass_Dialog::on_pbCencel_clicked);
}

Compass_Dialog::~Compass_Dialog()
{
    done(ret);
}

void Compass_Dialog::on_pbEnter_clicked()
{
    qDebug()<<tr("-->>Compass Calibration: Press the OK key.")<<endl;
    if(m_rbHorCali.isChecked()) {
        ret = 1;
    }else if(m_rbMulitCali.isChecked()) {
        ret = 2;
    }
    done(Accepted);
    emit signal_CompassCali(ret);
}
void Compass_Dialog::on_pbCencel_clicked()
{
    qDebug()<<tr("-->>Compass Calibration: Press the Cencel key.")<<endl;
    ret = 0;
    done(Rejected);
    emit signal_CompassCali(ret);
}
