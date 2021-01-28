#include "detect_widget.h"
#define DEBUG_EN
//侦测设备信息界面
Detect_Widget::Detect_Widget(QWidget *parent) : QWidget(parent)
{ 
    this->setGeometry(0,75,800,280);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:white");
    wid_x1 = this->x();
    wid_y1 = this->y();
#ifdef DEBUG_EN
    qDebug()<<tr("Get Detect Widget Point x = %1, y = %2").arg(this->x()).arg(this->y())<<endl;
#endif
    //字体
    QFont fn_lab;
    fn_lab.setPointSize(32);
    fn_lab.setBold(true);
    fn_lab.setFamily("SimHei");

    //字体
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
    lab_name1 = new QLabel(this);
    lab_name1->setText(tr("云哨侦测"));
    lab_name1->setFont(fn_lab);
    lab_name1->setAlignment(Qt::AlignCenter);
    lab_name1->resize(200,65);
    //lab_name1->setMinimumSize(120,70);
    //lab_name1->setGeometry(0,75,120,155);
    //设备图标
    lab_icon1 = new QLabel();
    QImage *img1 = new QImage();
    if(!(img1->load(":/Icon/device/CLOUD.png"))) {
        QMessageBox::information(this,"Error","open image CLOUD.png error!");
        delete img1;
        return;
    }
    QImage img_after1 = img1->scaled(lab_icon1->size(),Qt::KeepAspectRatio);//重新调整图像大小以适应窗口
    lab_icon1->setScaledContents(true);
    lab_icon1->setPixmap(QPixmap::fromImage(img_after1));
    //lab_icon1->setMinimumSize(70,70);
    //lab_icon1->resize(img1->width(),img1->height());
    lab_icon1->setAlignment(Qt::AlignCenter);
    lab_icon1->resize(65,65);
    //lab_icon1->setGeometry(120,75,200,155);
    lab_icon1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //单选按钮
    rbut1 = new QRadioButton();   
    //rbut1->setGeometry(220,75,300,155);
    //rbut1->setMinimumSize(70,70);
    rbut1->resize(65,65);
    rbut1->setStyleSheet(rb_style);
    //连接图标
    lab_conn_icon1 = new QLabel();
    lab_conn_icon1->resize(100,65);
    QImage *img_detect1 = new QImage();
    if(!(img_detect1->load(":/Icon/conn1.png"))) {
        QMessageBox::information(this,"Error","open image conn1.png error!");
        delete img_detect1;
        return;
    }
    QImage img_after2 = img_detect1->scaled(lab_conn_icon1->size(),Qt::KeepAspectRatio);
    lab_conn_icon1->setScaledContents(true);
    lab_conn_icon1->setPixmap(QPixmap::fromImage(img_after2));
    lab_conn_icon1->resize(100,65);
    lab_conn_icon1->setAlignment(Qt::AlignCenter);
    lab_conn_icon1->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    lab_conn_icon1->setEnabled(false);
    //网络连接信息
    lab_conn1 = new QLabel();
    lab_conn1->resize(200,65);
    lab_conn1->setMinimumSize(200,65);
    lab_conn1->setFont(fn2_lab);
    lab_conn1->setWordWrap(true);
    lab_conn1->setAlignment(Qt::AlignCenter);
    //lab_conn1->setText("192.168.1.11");
    lab_conn1->setEnabled(false);
    //上下行标识
    lab_datadir1 = new QLabel();
    lab_datadir1->resize(65,65);
    QImage *img_detect4 = new QImage();
    if(!(img_detect4->load(":/Icon/up.png"))){
        QMessageBox::information(this,"Error","open image up.png error!");
        delete img_detect4;
        return;
    }
    QImage img_after7 = img_detect4->scaled(lab_datadir1->size(),Qt::KeepAspectRatio);
    lab_datadir1->setScaledContents(true);
    lab_datadir1->setPixmap(QPixmap::fromImage(img_after7));
    lab_datadir1->resize(65,65);
    lab_datadir1->setEnabled(false);

    hbLayout1->addStretch();
    hbLayout1->addWidget(rbut1);
    hbLayout1->addWidget(lab_name1);
    hbLayout1->addWidget(lab_icon1);
    hbLayout1->addWidget(lab_datadir1);
    hbLayout1->addWidget(lab_conn_icon1);   
    hbLayout1->addWidget(lab_conn1);
    hbLayout1->addStretch();

    //第二行内容
    QHBoxLayout *hbLayout2 = new QHBoxLayout(this);
    //名称
    lab_name2 = new QLabel();
    lab_name2->setText(tr("微波侦测"));
    lab_name2->setFont(fn_lab);
    lab_name2->setAlignment(Qt::AlignCenter);
    lab_name2->resize(200,65);
    //lab_name2->setMinimumSize(120,70);
    //lab_name2->setGeometry(0,75,100,155);
    //设备图标
    lab_icon2 = new QLabel();
    QImage *img2 = new QImage();
    if(!(img2->load(":/Icon/device/WAVE.png"))) {
        QMessageBox::information(this,"Error","open image WAVE.png error!");
        delete img2;
        return;
    }
    QImage img_after3 = img2->scaled(lab_icon2->size(),Qt::KeepAspectRatio);
    lab_icon2->setScaledContents(true);
    lab_icon2->setPixmap(QPixmap::fromImage(img_after3));
    //lab_icon2->setMinimumSize(70,70);
    //lab_icon2->resize(img1->width(),img1->height());
    lab_icon2->setAlignment(Qt::AlignCenter);
    lab_icon2->resize(65,65);
    //lab_icon2->setGeometry(120,75,200,155);
    lab_icon2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //单选按钮
    rbut2 = new QRadioButton();
    //rbut2->setGeometry(220,75,300,155);
    //rbut2->setMinimumSize(70,70);
    rbut2->resize(65,65);
    rbut2->setStyleSheet(rb_style);
    //连接图标
    lab_conn_icon2 = new QLabel();
    lab_conn_icon2->resize(120,65);
    QImage *img_detect2 = new QImage();
    if(!(img_detect2->load(":/Icon/conn1.png"))) {
        QMessageBox::information(this,"Error","open image conn1.png error!");
        delete img_detect2;
        return;
    }
    QImage img_after4 = img_detect2->scaled(lab_conn_icon2->size(),Qt::KeepAspectRatio);
    lab_conn_icon2->setScaledContents(true);
    lab_conn_icon2->setPixmap(QPixmap::fromImage(img_after4));
    lab_conn_icon2->resize(120,65);
    lab_conn_icon2->setAlignment(Qt::AlignCenter);
    lab_conn_icon2->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    lab_conn_icon2->setEnabled(false);
    //连接提示信息
    lab_conn2 = new QLabel();
    lab_conn2->resize(200,65);
    lab_conn2->setMinimumSize(200,65);
    lab_conn2->setFont(fn2_lab);
    lab_conn2->setWordWrap(true);
    lab_conn2->setAlignment(Qt::AlignCenter);
    //lab_conn2->setText("192.168.1.12");
    lab_conn2->setEnabled(false);
    //上下行标识
    lab_datadir2 = new QLabel();
    lab_datadir2->resize(120,65);
    QImage *img_detect5 = new QImage();
    if(!(img_detect5->load(":/Icon/up.png"))){
        QMessageBox::information(this,"Error","open image up.png error!");
        delete img_detect5;
        return;
    }
    QImage img_after8 = img_detect5->scaled(lab_datadir2->size(),Qt::KeepAspectRatio);
    lab_datadir2->setScaledContents(true);
    lab_datadir2->setPixmap(QPixmap::fromImage(img_after8));
    lab_datadir2->resize(120,65);
    lab_datadir2->setEnabled(false);

    hbLayout2->addStretch();
    hbLayout2->addWidget(rbut2);
    hbLayout2->addWidget(lab_name2);
    hbLayout2->addWidget(lab_icon2);
    hbLayout2->addWidget(lab_datadir2);
    hbLayout2->addWidget(lab_conn_icon2);
    hbLayout2->addWidget(lab_conn2);
    hbLayout2->addStretch();

    //第三行内容
    QHBoxLayout *hbLayout3 = new QHBoxLayout(this);
    //名称
    lab_name3 = new QLabel();
    lab_name3->setText(tr("雷达侦测"));
    lab_name3->setFont(fn_lab);
    lab_name3->setAlignment(Qt::AlignCenter);
    lab_name3->resize(200,65);
    //设备图标
    lab_icon3 = new QLabel();
    QImage *img3 = new QImage();
    if(!(img3->load(":/Icon/device/RADAR.png"))) {
        QMessageBox::information(this,"Error","open image RADAR.png error!");
        delete img3;
        return;
    }
    QImage img_after5 = img3->scaled(lab_icon3->size(),Qt::KeepAspectRatio);//重新调整图像大小以适应窗口
    lab_icon3->setScaledContents(true);
    lab_icon3->setPixmap(QPixmap::fromImage(img_after5));
    //lab_icon3->setMinimumSize(70,70);
    //lab_icon3->resize(img1->width(),img1->height());
    lab_icon3->setAlignment(Qt::AlignCenter);
    lab_icon3->resize(65,65);
    //lab_icon3->setGeometry(120,75,200,155);
    lab_icon3->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    //单选按钮
    rbut3 = new QRadioButton();
    //rbut3->setGeometry(220,75,300,155);
    //rbut3->setMinimumSize(70,70);
    rbut3->resize(65,65);
    rbut3->setStyleSheet(rb_style);
    //连接图标
    lab_conn_icon3 = new QLabel();
    lab_conn_icon3->resize(120,65);
    QImage *img_detect3 = new QImage();
    if(!(img_detect3->load(":/Icon/conn1.png"))) {
        QMessageBox::information(this,"Error","open image conn1.png error!");
        delete img_detect3;
        return;
    }
    QImage img_after6 = img_detect3->scaled(lab_conn_icon3->size(),Qt::KeepAspectRatio);
    lab_conn_icon3->setScaledContents(true);
    lab_conn_icon3->setPixmap(QPixmap::fromImage(img_after6));
    lab_conn_icon3->resize(120,65);
    lab_conn_icon3->setAlignment(Qt::AlignCenter);
    lab_conn_icon3->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    lab_conn_icon3->setEnabled(false);
    //网络连接信息
    lab_conn3 = new QLabel();
    lab_conn3->resize(200,65);
    lab_conn3->setMinimumSize(200,65);
    lab_conn3->setFont(fn2_lab);
    lab_conn3->setWordWrap(true);
    lab_conn3->setAlignment(Qt::AlignCenter);
    //lab_conn3->setText("192.168.1.33");
    lab_conn3->setEnabled(false);
    //上下行标识
    lab_datadir3 = new QLabel();
    lab_datadir3->resize(120,65);
    QImage *img_detect6 = new QImage();
    if(!(img_detect6->load(":/Icon/up.png"))){
        QMessageBox::information(this,"Error","open image down.png error!");
        delete img_detect6;
        return;
    }
    QImage img_after9 = img_detect6->scaled(lab_datadir3->size(),Qt::KeepAspectRatio);
    lab_datadir3->setScaledContents(true);
    lab_datadir3->setPixmap(QPixmap::fromImage(img_after9));
    lab_datadir3->resize(120,65);
    lab_datadir3->setEnabled(false);
    //lab_datadir3->setFont(fn_lab);
    //lab_datadir3->setText("200Kbps");

    hbLayout3->addStretch();
    hbLayout3->addWidget(rbut3);
    hbLayout3->addWidget(lab_name3);
    hbLayout3->addWidget(lab_icon3);
    hbLayout3->addWidget(lab_datadir3);
    hbLayout3->addWidget(lab_conn_icon3);
    hbLayout3->addWidget(lab_conn3);
    hbLayout3->addStretch();

    bt_group = new QButtonGroup(this);
    bt_group->addButton(rbut1);
    bt_group->addButton(rbut2);
    bt_group->addButton(rbut3);
    bt_group->setExclusive(true);
    connect(bt_group, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
    //第四行内容
    QHBoxLayout *hbLayout4 = new QHBoxLayout(this);
    pbt1 = new QPushButton();
    pbt2 = new QPushButton();
    QFont ft_pbt;
    ft_pbt.setBold(true);
    ft_pbt.setPointSize(20);
    pbt1->setMinimumSize(250,70);
    pbt2->setMinimumSize(250,70);
    pbt1->setText(tr("退出"));
    pbt2->setText(tr("确定"));
    pbt1->setFont(ft_pbt);
    pbt2->setFont(ft_pbt);

    pbt1->setEnabled(false);
    pbt2->setEnabled(false);
    connect(pbt1, &QPushButton::clicked, this, &Detect_Widget::on_pbt1_Clicked);
    connect(pbt2, &QPushButton::clicked, this, &Detect_Widget::on_pbt2_Clicked);

    hbLayout4->addStretch();
    hbLayout4->addWidget(pbt1);
    hbLayout4->addStretch();
    hbLayout4->addWidget(pbt2);
    hbLayout4->addStretch();

    vbLayout->addLayout(hbLayout1);
    vbLayout->addLayout(hbLayout2);
    vbLayout->addLayout(hbLayout3);
    vbLayout->addLayout(hbLayout4);

    this->setLayout(vbLayout);
    tim_upip = new QTimer(this);
    connect(tim_upip, &QTimer::timeout, this, &Detect_Widget::slot_updateip);
    tim_upip->start(2000);

}
Detect_Widget::~Detect_Widget()
{
    delete this;
}

void Detect_Widget::detect_Init()
{
    rbut1->setEnabled(true);
    rbut2->setEnabled(true);
    rbut3->setEnabled(true);

    rbut1->setChecked(false);
    rbut2->setChecked(false);
    rbut3->setChecked(false);

    lab_conn1->clear();
    lab_conn2->clear();
    lab_conn3->clear();

    lab_datadir1->setEnabled(false);
    lab_datadir2->setEnabled(false);
    lab_datadir3->setEnabled(false);

    lab_conn_icon1->setEnabled(false);
    lab_conn_icon2->setEnabled(false);
    lab_conn_icon3->setEnabled(false);

}
void Detect_Widget::on_pbt1_Clicked()
{
    emit sn_dwCencleSelect(1);
}

void Detect_Widget::on_pbt2_Clicked()
{

    if(rbut1->isChecked()) {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Detect Widget: Checked Cloud Whistle!")<<endl;
#endif
        pbt1->setEnabled(true);
        pbt2->setEnabled(false);
        rbut1->setEnabled(false);
        rbut2->setEnabled(false);
        rbut3->setEnabled(false);
        id = 1;
        emit sn_dwSelectDevice(id);
    }else if(rbut2->isChecked()) {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Detect Widget: Checked Wave Detector!")<<endl;
#endif
        pbt1->setEnabled(true);
        pbt2->setEnabled(false);
        rbut1->setEnabled(false);
        rbut2->setEnabled(false);
        rbut3->setEnabled(false);
        id = 2;
        //emit sn_dwSelectDevice(id);
        //2021-1-4
        dialog_Subdevlist *m_devlist = new dialog_Subdevlist(this);
        m_devlist->setAttribute(Qt::WA_DeleteOnClose);
        m_devlist->show();
        connect(m_devlist, &dialog_Subdevlist::sig_isOk, this, &Detect_Widget::slot_getProver);
        connect(m_devlist, &dialog_Subdevlist::sig_isCencel, this, &Detect_Widget::slot_getProver);
        //2021-1-4
    }else if(rbut3->isChecked()) {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Detect Widget: Checked Photoelectric Radar Detector!")<<endl;
#endif
        pbt1->setEnabled(true);
        pbt2->setEnabled(false);
        rbut1->setEnabled(false);
        rbut2->setEnabled(false);
        rbut3->setEnabled(false);
        id = 3;
        emit sn_dwSelectDevice(id);
    }else {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Detect Widget: Unchecked Device.")<<endl;
#endif
        pbt1->setEnabled(false);
        pbt2->setEnabled(true);
        rbut1->setEnabled(true);
        rbut2->setEnabled(true);
        rbut3->setEnabled(true);
    }

}

void Detect_Widget::onButtonClicked(QAbstractButton *button)
{
    pbt2->setEnabled(true);
}

void Detect_Widget::slot_updateip()
{
    QString tmp;
    //tmp.append("ID:");
    tim_upip->stop();
    switch (id) {
    case 1:
        tmp.append(dev_ip1);
        lab_conn1->setText(tmp);
        lab_conn1->setEnabled(true);
        break;
    case 2:
        tmp.append(dev_ip2);
        lab_conn2->setText(tmp);
        lab_conn2->setEnabled(true);
        break;
    case 3:
        tmp.append(dev_ip3);
        lab_conn3->setText(tmp);
        lab_conn3->setEnabled(true);
        break;
    default:
        break;
    }
    tim_upip->start(2000);
}
//2021-1-4
void Detect_Widget::slot_getProver(int ret)
{
    if(ret > 0) {
        switch (ret) {
        case 1:
            pro_ver = "TCP";
            qDebug()<<tr("User Select TCP Protocol Version Device.")<<endl;
            break;

        case 2:
            pro_ver = "HTTP";
            qDebug()<<tr("User Select HTTP Protocol Version Device.")<<endl;
            break;

        case 3:
            pro_ver = "ZMQ";
            qDebug()<<tr("User Select ZMQ Protocol Version Device.")<<endl;
            break;

        default:
            break;
        }
    } else {
        pro_ver = "";
        qDebug()<<tr("The user selects an unspecified protocol version.")<<endl;
    }
    emit sn_dwSelectDevice(id);
}
