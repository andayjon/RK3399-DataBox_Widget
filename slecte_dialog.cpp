#include "slecte_dialog.h"


Slecte_Dialog::Slecte_Dialog(QWidget *parent) : QDialog(parent)
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
    lab_Info.setText("请选择一种设备类型.");
    lab_Info.resize(300,100);
    QFont bt_font;
    font.setPointSize(25);
    Btn_Enter.setText("确定");
    Btn_Enter.resize(100,100);
    Btn_Enter.setFont(bt_font);
    Btn_Enter.setStyleSheet("QPushButton{border-style: none; background-color: silver; font:bold 30px;}"
                       "QPushButton:hover{background-color:gray;color: white;}"
                       "QPushButton:pressed{background-color:slategray; border-style: inset; }");

    Btn_Cencel.setText("取消");
    Btn_Cencel.resize(100,100);
    Btn_Cencel.setFont(bt_font);
    Btn_Cencel.setStyleSheet("QPushButton{border-style: none; background-color: silver; font:bold 30px;}"
                       "QPushButton:hover{background-color:gray;color: white;}"
                       "QPushButton:pressed{background-color:slategray; border-style: inset; }");

    this->resize(400,300);
    this->setWindowTitle("选择设备");

    QHBoxLayout *m_hlayout = new QHBoxLayout;
    m_hlayout->addWidget(&Btn_Cencel);
    m_hlayout->addWidget(&Btn_Enter);

    QHBoxLayout *m_rblayout = new QHBoxLayout;
    Btn_group = new QButtonGroup(this);
    rBtn_New.resize(100,100);
    rBtn_Old.resize(100,100);
    QString rb_style = "QRadioButton::indicator{width:35px;height:35px;}";  //修改单选按钮样式
    rBtn_New.setStyleSheet(rb_style);
    rBtn_Old.setStyleSheet(rb_style);
    rBtn_New.setFont(font);
    rBtn_Old.setFont(font);
    rBtn_New.setText("2.0版本");
    rBtn_Old.setText("1.0版本");
    m_rblayout->addWidget(&rBtn_New);
    m_rblayout->addWidget(&rBtn_Old);
    rBtn_New.setChecked(true);

    QVBoxLayout *m_vlayout = new QVBoxLayout;
    m_vlayout->addWidget(&lab_Info);
    m_vlayout->addStretch(1);
    m_vlayout->addLayout(m_rblayout);
    m_vlayout->addStretch(2);
    m_vlayout->addLayout(m_hlayout);

    this->setLayout(m_vlayout);

    connect(&Btn_Enter, &QPushButton::clicked, this, &Slecte_Dialog::Btn_Enter_clicked );
    connect(&Btn_Cencel, &QPushButton::clicked, this, &Slecte_Dialog::Btn_Cencel_clicked);
}
Slecte_Dialog::~Slecte_Dialog()
{
    done(ret);
}

void Slecte_Dialog::Btn_Enter_clicked()
{
    qDebug()<<tr("Press the OK key.")<<endl;
    if(rBtn_New.isChecked()) {
        ret = 2;
    }else if(rBtn_Old.isChecked()) {
        ret = 1;
    }
    done(Accepted);
    emit sig_isok(ret);
}

void Slecte_Dialog::Btn_Cencel_clicked()
{
    qDebug()<<tr("Press the Cencel key.")<<endl;
    ret = 0;
    done(Rejected);
    emit sig_isCencel(ret);
}
