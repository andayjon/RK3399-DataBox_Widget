#include "net_dialog.h"

net_dialog::net_dialog(QWidget *parent) : QDialog(parent), Btn_Close(this)
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
    QFont font;
    font.setPointSize(16);
    font.setFamily("SimHei");

    this->resize(700, 380);
    this->setWindowTitle(tr("Sim Card Information"));
//    this->setWindowFlag(Qt::WindowSystemMenuHint);
//    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->setStyleSheet("background-color:white; font:bold 16px;");
//    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    //this->showFullScreen();
    tab_Card1widget = new QWidget(this);
    tab_Card2widget = new QWidget(this);
    tab_Card3widget = new QWidget(this);

    pt_Edit1 = new QPlainTextEdit(this);
    pt_Edit2 = new QPlainTextEdit(this);
    pt_Edit3 = new QPlainTextEdit(this);


    tab_widget = new QTabWidget();
    setTabWidgetStyleSheet(tab_widget);
    tab_widget->setTabPosition(QTabWidget::North);  //


    Btn_Close.setText(tr("Close"));
    Btn_Close.setFont(font);
    Btn_Close.resize(100,30);

    QVBoxLayout *vbox_layout1 = new QVBoxLayout(this);
    QHBoxLayout *hbox_layout1 = new QHBoxLayout(this);
    hbox_layout1->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    hbox_layout1->addWidget(&Btn_Close);
    hbox_layout1->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding));
    vbox_layout1->addWidget(tab_widget);
    vbox_layout1->addLayout(hbox_layout1);

    this->setLayout(vbox_layout1);

    connect(&Btn_Close, &QPushButton::clicked, this, &net_dialog::Btn_Close_Clicked);
}
net_dialog::~net_dialog()
{
    emit sign_quitFlag(1);
    done(1);
}

void net_dialog::disp_card_info(QByteArray *bytes)
{

}

void net_dialog::Btn_Close_Clicked()
{
    done(Rejected);
    emit sign_quitFlag(1);
}

void net_dialog::setTabWidgetStyleSheet(QTabWidget* obj)
{
    obj->setStyleSheet(
                    "QTabWidget{"
                    "background-color:transparent;"
                    "}"
                    "QTabWidget::pane{"
                    "    border:2px;"
                    "}"
                    "QTabWidget::tab-bar{"
                    "        alignment:left;"
                    "}"
                    "QTabBar::tab{"
                    "    background:rgb(14, 106, 175);"
                    "    color:white;"
                    "    min-width:35ex;"
                    "    min-height:10ex;"
                    "}"
                    "QTabBar::tab:hover{"
                    "    background:rgb(255, 255, 255, 100);"
                    "color:black;"
                    "}"
                    "QTabBar::tab:selected{"
                    "    border-color: black;"
                    "    background:red;"
                    "    color:white;"
                    "}"
                    );

}
