#include "defense_widget.h"
#define DEBUG_EN
//防御设备信息界面
defense_Widget::defense_Widget(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(0, 75, 800, 280);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:white");
    wid_x1 = this->x();
    wid_y1 = this->y();
#ifdef DEBUG_EN
    qDebug()<<tr("Defense Widget x = %1, y = %2").arg(wid_x1).arg(wid_y1)<<endl;
#endif
    //字体
    QFont fn_lab;
    fn_lab.setPointSize(32);
    fn_lab.setBold(true);
    fn_lab.setFamily("SimHei");

    QFont fn2_lab;
    fn2_lab.setPointSize(16);
    fn2_lab.setBold(true);
    fn2_lab.setFamily("SimHei");
    QString rb_style = "QRadioButton::indicator{width:65px;height:65px;}";  //修改单选按钮样式
    //整体布局
    QVBoxLayout *vbLayout = new QVBoxLayout(this);
    //第一行内容
    QHBoxLayout *hbLayout1 = new QHBoxLayout(this);
    //名称
    l_name1 = new QLabel(this);
    l_name1->setText(tr("定向设备"));
    l_name1->setFont(fn_lab);
    l_name1->setAlignment(Qt::AlignCenter);
    l_name1->resize(200,65);
    //lab_name1->setMinimumSize(120,70);
    //lab_name1->setGeometry(0,75,120,155);
    //设备图标
    l_icon1 = new QLabel();
    QImage *img1 = new QImage();
    if(!(img1->load(":/Icon/device/DS.png"))) {
        QMessageBox::information(this,"Error","open image DS.png error!");
        delete img1;
        return;
    }
    QImage img_after1 = img1->scaled(l_icon1->size(),Qt::KeepAspectRatio);//重新调整图像大小以适应窗口
    l_icon1->setScaledContents(true);
    l_icon1->setPixmap(QPixmap::fromImage(img_after1));
    l_icon1->resize(65,65);
    //lab_icon1->setMinimumSize(70,70);
    //lab_icon1->resize(img1->width(),img1->height());
    l_icon1->setAlignment(Qt::AlignCenter);
    //lab_icon1->setGeometry(120,75,200,155);
    l_icon1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //单选按钮
    rbut_1 = new QRadioButton();
    rbut_1->resize(65,65);
    //rbut1->setGeometry(220,75,300,155);
    //rbut1->setMinimumSize(70,70);
    rbut_1->setStyleSheet(rb_style);
    //连接图标
    l_conn_icon1 = new QLabel();
    l_conn_icon1->resize(100,65);
    QImage *img_detect1 = new QImage();
    if(!(img_detect1->load(":/Icon/conn1.png"))) {
        QMessageBox::information(this,"Error","open image conn1.png error!");
        delete img_detect1;
        return;
    }
    QImage img_after2 = img_detect1->scaled(l_conn_icon1->size(),Qt::KeepAspectRatio);
    l_conn_icon1->setScaledContents(true);
    l_conn_icon1->setPixmap(QPixmap::fromImage(img_after2));
    l_conn_icon1->resize(100,65);
    l_conn_icon1->setAlignment(Qt::AlignCenter);
    l_conn_icon1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    l_conn_icon1->setEnabled(false);
    //网络连接信息
    l_conn1 = new QLabel();
    l_conn1->resize(200,65);
    l_conn1->setMinimumSize(200,65);
    l_conn1->setWordWrap(true);
    l_conn1->setAlignment(Qt::AlignCenter);
    l_conn1->setFont(fn2_lab);
    //l_conn1->setText("ID=");
    l_conn1->setEnabled(false);
    //上下行标识
    l_datadir1 = new QLabel();
    l_datadir1->resize(65,65);
    QImage *img_detect4 = new QImage();
    if(!(img_detect4->load(":/Icon/up.png"))){
        QMessageBox::information(this,"Error","open image up.png error!");
        delete img_detect4;
        return;
    }
    QImage img_after7 = img_detect4->scaled(l_datadir1->size(),Qt::KeepAspectRatio);
    l_datadir1->setScaledContents(true);
    l_datadir1->setPixmap(QPixmap::fromImage(img_after7));
    l_datadir1->resize(65,65);
    l_datadir1->setEnabled(false);

    hbLayout1->addStretch();
    hbLayout1->addWidget(rbut_1);
    hbLayout1->addWidget(l_name1);
    hbLayout1->addWidget(l_icon1);
    hbLayout1->addWidget(l_datadir1);
    hbLayout1->addWidget(l_conn_icon1);
    hbLayout1->addWidget(l_conn1);
    hbLayout1->addStretch();

    //第二行内容
    QHBoxLayout *hbLayout2 = new QHBoxLayout(this);
    //名称
    l_name2 = new QLabel();
    l_name2->setText(tr("全向设备"));
    l_name2->setFont(fn_lab);
    l_name2->setAlignment(Qt::AlignCenter);
    l_name2->resize(200,65);
    //lab_name2->setMinimumSize(120,70);
    //lab_name2->setGeometry(0,75,100,155);
    //设备图标
    l_icon2 = new QLabel();
    QImage *img2 = new QImage();
    if(!(img2->load(":/Icon/device/OS.png"))) {
        QMessageBox::information(this,"Error","open image OS.png error!");
        delete img2;
        return;
    }
    QImage img_after3 = img2->scaled(l_icon2->size(),Qt::KeepAspectRatio);
    l_icon2->setScaledContents(true);
    l_icon2->setPixmap(QPixmap::fromImage(img_after3));
    //lab_icon2->setMinimumSize(70,70);
    //lab_icon2->resize(img1->width(),img1->height());
    l_icon2->setAlignment(Qt::AlignCenter);
    l_icon2->resize(65,65);
    //lab_icon2->setGeometry(120,75,200,155);
    l_icon2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //单选按钮
    rbut_2 = new QRadioButton();
    //rbut2->setGeometry(220,75,300,155);
    //rbut2->setMinimumSize(70,70);
    rbut_2->resize(65,65);
    rbut_2->setStyleSheet(rb_style);
    //连接图标
    l_conn_icon2 = new QLabel();
    l_conn_icon2->resize(120,65);
    QImage *img_detect2 = new QImage();
    if(!(img_detect2->load(":/Icon/conn1.png"))) {
        QMessageBox::information(this,"Error","open image conn1.png error!");
        delete img_detect2;
        return;
    }
    QImage img_after4 = img_detect2->scaled(l_conn_icon2->size(),Qt::KeepAspectRatio);
    l_conn_icon2->setScaledContents(true);
    l_conn_icon2->setPixmap(QPixmap::fromImage(img_after4));
    l_conn_icon2->resize(120,65);
    l_conn_icon2->setAlignment(Qt::AlignCenter);
    l_conn_icon2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    l_conn_icon2->setEnabled(false);
    //网络连接信息
    l_conn2 = new QLabel();
    l_conn2->resize(200,65);
    l_conn2->setMinimumSize(200,65);
    l_conn2->setWordWrap(true);
    l_conn2->setAlignment(Qt::AlignCenter);
    l_conn2->setFont(fn2_lab);
    //l_conn2->setText("ID=");
    l_conn2->setEnabled(false);
    //上下行标识
    l_datadir2 = new QLabel();
    l_datadir2->resize(120,65);
    QImage *img_detect5 = new QImage();
    if(!(img_detect5->load(":/Icon/up.png"))){
        QMessageBox::information(this,"Error","open image up.png error!");
        delete img_detect5;
        return;
    }
    QImage img_after8 = img_detect5->scaled(l_datadir2->size(),Qt::KeepAspectRatio);
    l_datadir2->setScaledContents(true);
    l_datadir2->setPixmap(QPixmap::fromImage(img_after8));
    l_datadir2->resize(120,65);
    l_datadir2->setEnabled(false);

    hbLayout2->addStretch();
    hbLayout2->addWidget(rbut_2);
    hbLayout2->addWidget(l_name2);
    hbLayout2->addWidget(l_icon2);
    hbLayout2->addWidget(l_datadir2);
    hbLayout2->addWidget(l_conn_icon2);
    hbLayout2->addWidget(l_conn2);
    hbLayout2->addStretch();

    //第三行内容
    QHBoxLayout *hbLayout3 = new QHBoxLayout(this);
    //名称
    l_name3 = new QLabel();
    l_name3->setText(tr("船载设备"));
    l_name3->setFont(fn_lab);
    l_name3->setAlignment(Qt::AlignCenter);
    l_name3->resize(200,65);
    //设备图标
    l_icon3 = new QLabel();
    QImage *img3 = new QImage();
    if(!(img3->load(":/Icon/device/SB.png"))) {
        QMessageBox::information(this,"Error","open image AW.png error!");
        delete img3;
        return;
    }
    QImage img_after5 = img3->scaled(l_icon3->size(),Qt::KeepAspectRatio);//重新调整图像大小以适应窗口
    l_icon3->setScaledContents(true);
    l_icon3->setPixmap(QPixmap::fromImage(img_after5));
    //lab_icon3->setMinimumSize(70,70);
    //lab_icon3->resize(img1->width(),img1->height());
    l_icon3->setAlignment(Qt::AlignCenter);
    l_icon3->resize(65,65);
    //lab_icon3->setGeometry(120,75,200,155);
    l_icon3->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //单选按钮
    rbut_3 = new QRadioButton();
    //rbut3->setGeometry(220,75,300,155);
    //rbut3->setMinimumSize(70,70);
    rbut_3->resize(65,65);
    rbut_3->setStyleSheet(rb_style);
    //连接图标
    l_conn_icon3 = new QLabel();
    l_conn_icon3->resize(120,65);
    QImage *img_detect3 = new QImage();
    if(!(img_detect3->load(":/Icon/conn1.png"))) {
        QMessageBox::information(this,"Error","open image conn1.png error!");
        delete img_detect3;
        return;
    }
    QImage img_after6 = img_detect3->scaled(l_conn_icon3->size(),Qt::KeepAspectRatio);
    l_conn_icon3->setScaledContents(true);
    l_conn_icon3->setPixmap(QPixmap::fromImage(img_after6));
    l_conn_icon3->resize(120,65);
    l_conn_icon3->setAlignment(Qt::AlignCenter);
    l_conn_icon3->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    l_conn_icon3->setEnabled(false);
    //网络连接信息
    l_conn3 = new QLabel();
    l_conn3->resize(200,65);
    l_conn3->setMinimumSize(200,65);
    l_conn3->setWordWrap(true);
    l_conn3->setAlignment(Qt::AlignCenter);
    l_conn3->setFont(fn2_lab);
    //l_conn3->setText("ID=");
    l_conn3->setEnabled(false);
    //上下行标识
    l_datadir3 = new QLabel();
    l_datadir3->resize(120,65);
    QImage *img_detect6 = new QImage();
    if(!(img_detect6->load(":/Icon/up.png"))){
        QMessageBox::information(this,"Error","open image down.png error!");
        delete img_detect6;
        return;
    }
    QImage img_after9 = img_detect6->scaled(l_datadir3->size(),Qt::KeepAspectRatio);
    l_datadir3->setScaledContents(true);
    l_datadir3->setPixmap(QPixmap::fromImage(img_after9));
    l_datadir3->resize(120,65);
    l_datadir3->setEnabled(false);
    //lab_datadir3->setFont(fn_lab);
    //lab_datadir3->setText("200Kbps");

    hbLayout3->addStretch();
    hbLayout3->addWidget(rbut_3);
    hbLayout3->addWidget(l_name3);
    hbLayout3->addWidget(l_icon3);
    hbLayout3->addWidget(l_datadir3);
    hbLayout3->addWidget(l_conn_icon3);
    hbLayout3->addWidget(l_conn3);
    hbLayout3->addStretch();

    bt_group = new QButtonGroup(this);
    bt_group->addButton(rbut_1);
    bt_group->addButton(rbut_2);
    bt_group->addButton(rbut_3);
    bt_group->setExclusive(true);
    connect(bt_group, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
    //第四行内容
    QHBoxLayout *hbLayout4 = new QHBoxLayout(this);
    pbt_1 = new QPushButton();
    pbt_2 = new QPushButton();
    QFont ft_pbt;
    ft_pbt.setBold(true);
    ft_pbt.setPointSize(20);
//    pbt_1->resize(200,60);
//    pbt_2->resize(200,60);
    pbt_1->setMinimumSize(250,70);
    pbt_2->setMinimumSize(250,70);
    pbt_1->setText(tr("退出"));
    pbt_2->setText(tr("确定"));
    pbt_1->setFont(ft_pbt);
    pbt_2->setFont(ft_pbt);

    pbt_1->setEnabled(false);
    pbt_2->setEnabled(false);
    connect(pbt_1, &QPushButton::clicked, this, &defense_Widget::on_pbt_1_Clicked);
    connect(pbt_2, &QPushButton::clicked, this, &defense_Widget::on_pbt_2_Clicked);

    hbLayout4->addStretch();
    hbLayout4->addWidget(pbt_1);
    hbLayout4->addStretch();
    hbLayout4->addWidget(pbt_2);
    hbLayout4->addStretch();

    vbLayout->addLayout(hbLayout1);
    vbLayout->addLayout(hbLayout2);
    vbLayout->addLayout(hbLayout3);
    vbLayout->addLayout(hbLayout4);

    this->setLayout(vbLayout);

    tim_upid = new QTimer(this);
    connect(tim_upid, &QTimer::timeout, this, &defense_Widget::slot_updateid);
    tim_upid->start(2000);
}

defense_Widget::~defense_Widget()
{
    delete this;
}

void defense_Widget::defense_Init()
{
    rbut_1->setEnabled(true);
    rbut_2->setEnabled(true);
    rbut_3->setEnabled(true);

    rbut_1->setChecked(false);
    rbut_2->setChecked(false);
    rbut_3->setChecked(false);

    l_conn1->clear();
    l_conn2->clear();
    l_conn3->clear();

    l_datadir1->setEnabled(false);
    l_datadir2->setEnabled(false);
    l_datadir3->setEnabled(false);

    l_conn_icon1->setEnabled(false);
    l_conn_icon2->setEnabled(false);
    l_conn_icon3->setEnabled(false);
}
//取消键按下
void defense_Widget::on_pbt_1_Clicked()
{
    emit sn_dwCencleSelect(2);
}
//确定键按下
void defense_Widget::on_pbt_2_Clicked()
{
    if(rbut_1->isChecked()) {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Defense Widget:Checked Orienteering Shield!")<<endl;
#endif
        pbt_1->setEnabled(true);
        pbt_2->setEnabled(false);
        rbut_1->setEnabled(false);
        rbut_2->setEnabled(false);
        rbut_3->setEnabled(false);
        id = 4;
        Slecte_Dialog *m_sdialog = new Slecte_Dialog(this);
        m_sdialog->setAttribute(Qt::WA_DeleteOnClose);
        m_sdialog->show();
        //add signal and slot relevance
        connect(m_sdialog, &Slecte_Dialog::sig_isok, this, &defense_Widget::slot_getversion);
        connect(m_sdialog, &Slecte_Dialog::sig_isCencel, this, &defense_Widget::slot_getversion);

    }else if(rbut_2->isChecked()) {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Defense Widget:Checked Omnibearing Shield!")<<endl;
#endif
        pbt_1->setEnabled(true);
        pbt_2->setEnabled(false);
        rbut_1->setEnabled(false);
        rbut_2->setEnabled(false);
        rbut_3->setEnabled(false);
        id = 5;
        emit sn_dwSelectDevice(id);
    }else if(rbut_3->isChecked()) {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Defense Widget:Checked EW System!")<<endl;
#endif
        pbt_1->setEnabled(true);
        pbt_2->setEnabled(false);
        rbut_1->setEnabled(false);
        rbut_2->setEnabled(false);
        rbut_3->setEnabled(false);
    }else {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Defense Widget: Unchecked Device.")<<endl;
#endif
        pbt_1->setEnabled(false);
        pbt_2->setEnabled(true);
        rbut_1->setEnabled(true);
        rbut_2->setEnabled(true);
        rbut_3->setEnabled(true);
        id = 6;
        emit sn_dwSelectDevice(id);
    }
}
//更新显示设备ID信息
void defense_Widget::slot_updateid()
{
    QString tmp;
    //tmp.append("ID:");
    tim_upid->stop();
    switch (id) {
    case 4:
        l_conn1->setText(dev_id1);
        l_conn1->setEnabled(true);
        break;
    case 5:
        l_conn2->setText(dev_id2);
        l_conn2->setEnabled(true);
        break;
    case 6:
        l_conn3->setText(dev_id3);
        l_conn3->setEnabled(true);
        break;
    default:
        break;
    }
    tim_upid->start(2000);
}
//获取用户选择硬件版本
void defense_Widget::slot_getversion(int ret)
{
    if(ret > 0) {
        switch (ret) {
        case 1:
            dev_ver = "V1.0";
            qDebug()<<tr("User Select V1.0 Version Device.")<<endl;
            break;
        case 2:
            dev_ver = "V2.0";
            qDebug()<<tr("User Select V2.0 Version Device.")<<endl;
            break;
        }
    } else {
        dev_ver = "";
        qDebug()<<tr("The user selects an unspecified version.")<<endl;
    }
    emit sn_dwSelectDevice(id);
}

void defense_Widget::onButtonClicked(QAbstractButton *button)
{
    pbt_2->setEnabled(true);
}
