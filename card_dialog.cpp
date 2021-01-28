#include "card_dialog.h"
#include "ui_card_dialog.h"
#include <QFont>
#include <QColor>
#include <QPalette>
#include <QDebug>
Card_Dialog::Card_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Card_Dialog)
{

    this->setStyleSheet("QPushButton{"
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

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->resize(800, 380);
    this->setWindowTitle(tr("Sim Card Information"));
    this->setStyleSheet("background-color:white; font:bold 16px;");
//    this->setWindowFlag(Qt::WindowSystemMenuHint);
//    this->setWindowFlags(Qt::WindowCloseButtonHint);
    m_stat = true;
    QColor lab_color;
    lab_color.setBlue(100);

    QPalette lab_pe;
    lab_pe.setColor(QPalette::WindowText, Qt::red);
    QFont lab_font;
    lab_font.setPointSize(16);
    lab_font.setFamily("SimHei");

    ui->setupUi(this);
    ui->lab_card1Info->setFont(lab_font);
    ui->lab_card1Info->setPalette(lab_pe);
    //设置label样式表
//    ui->lab_card1Info->setStyleSheet("color:red;");

    ui->lab_card2Info->setFont(lab_font);
    ui->lab_card2Info->setPalette(lab_pe);
    ui->lab_card3Info->setFont(lab_font);
    ui->lab_card3Info->setPalette(lab_pe);

    m_timer = new QTimer(this);

    connect(m_timer, &QTimer::timeout, this, &Card_Dialog::slot_timerTimeout);
    m_timer->setInterval(500);
    m_timer->start();
}

Card_Dialog::~Card_Dialog()
{
    delete ui;
}

void Card_Dialog::on_pbtn_enter_clicked()
{
    if(m_stat) {
        emit sig_closeDialog(1);
        close();
    }
}

void Card_Dialog::slot_timerTimeout()
{
    ui->lab_card1Info->setText(m_card1Info);
    ui->lab_card2Info->setText(m_card2Info);
    ui->lab_card3Info->setText(m_card3Info);

//    qDebug()<<"Card1 Information:" + m_card1Info<<endl;
//    qDebug()<<"Card2 Information:" + m_card2Info<<endl;
//    qDebug()<<"Card3 Information:" + m_card3Info<<endl;
}

void Card_Dialog::on_tabWidget_currentChanged(int index)
{
    switch (index) {
    case 0:
        ui->lab_card1Info->setText(m_card1Info);
        break;
    case 1:
        ui->lab_card2Info->setText(m_card2Info);
        break;
    case 2:
        ui->lab_card3Info->setText(m_card3Info);
        break;
    default:
        break;
    }
}
