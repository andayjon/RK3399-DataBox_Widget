#include "widget.h"
#include "strtohex.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QPlainTextEdit>
#include <QProcess>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPalette>
#include <QPainter>
#include <QFont>
#include <QRect>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QScrollBar>
#include <QTime>
#include <QDateTime>
#include <QTextBlock>
#include <QPixmap>
#include <QLibrary>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include <QMovie>

QMutex mutex_dev;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
//    this->setGeometry(0, 75, 800, 280);
//    this->setWindowFlag(Qt::FramelessWindowHint);
//    this->setStyleSheet("background-color:black");
    this->setGeometry(0, 75, 800, 280);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:white");
    this->setAttribute(Qt::WA_DeleteOnClose);
    wid_x1 = this->x();
    wid_y1 = this->y();
    qDebug()<<tr("Get Widget Point x = %1, y = %2").arg(wid_x1).arg(wid_y1)<<endl;
    qDebug()<<tr("Get Widget Size x = %1, y = %2").arg(this->width()).arg(this->height())<<endl;

//    QPixmap imag_logo = QPixmap(":/Icon/picture/logo_black.jpg");
//    lab_logo = new QLabel(this);
//    lab_logo->setGeometry(0, 75, 200, 100);
    //imag_logo->scaled(lab_logo->size(), Qt::KeepAspectRatio);
//    QPixmap new_imag = imag_logo.scaled(lab_logo->size(),Qt::KeepAspectRatio);
//    lab_logo->setPixmap(new_imag);

//    lab_time = new QLabel(this);
//    lab_time->setGeometry(800, 30, 274, 50);
//    QTimer *sys_time = new QTimer(this);
//    connect(sys_time, SIGNAL(timeout()), this, SLOT(timerUpdate()));
//    sys_time->start(1000);

    //设备串口接收缓存
    my_pro_dev_recv.resize(2048);
    comm_dev_txbuf.resize(10);
    //send_msg = "operation....syncinfo";
/******************************************************************************************/
    QString pss1="QGroupBox { \
    border: 2px solid #0000EE; \
    border-radius: 5px; \
    margin-top: 1ex; \
    font-family:仿宋; \
    font:blod 12px; \
    } \
    QGroupBox::title {\
    subcontrol-origin: margin;\
//   subcontrol-position: top center; \
    subcontrol-position: top left;\
    padding: 0 3px;\
    }";
    grpbox_gps = new QGroupBox(this);
    grpbox_gps->setTitle("GPS信息");
    grpbox_gps->setTitle("GNSS Information");
    QPalette pe_grp;
    pe_grp.setColor(QPalette::WindowText,Qt::red);
    QFont ft_grp;
    ft_grp.setPointSize(12);
    grpbox_gps->setFont(ft_grp);
    grpbox_gps->setPalette(pe_grp);
    grpbox_gps->setStyleSheet(pss1);

    QFont ft_tedit;
    ft_tedit.setPointSize(10);
//    grpbox_gps->resize(350,100);
    //grpbox_gps->setGeometry(this->x(),this->y(),390,200);

    tedit_gps = new QTextEdit(this);
    tedit_gps->setFont(ft_tedit);
    tedit_gps->setTextColor(Qt::green);
    tedit_gps->setTextBackgroundColor(Qt::black);
    tedit_gps->setStyleSheet("background-color:black;");
    tedit_gps->setText("Display GPS Information...");
//    tedit_gps->resize(380,100);
    //tedit_gps->setGeometry(grpbox_gps->x(),grpbox_gps->y()+20,370,180);

    QHBoxLayout *gbox_layout = new QHBoxLayout();
    gbox_layout->addWidget(tedit_gps);
    grpbox_gps->setLayout(gbox_layout);
/***************************************************************************/

/***************************************************************************/
    grpbox_dev = new QGroupBox(this);
    grpbox_dev->setTitle("设备接口信息");
    grpbox_dev->setTitle("Device Information");
    grpbox_dev->setFont(ft_grp);
    grpbox_dev->setPalette(pe_grp);
    grpbox_dev->setStyleSheet(pss1);
//    grpbox_dev->setGeometry(this->x()+400,this->y(),390,200);

    //设备信息显示文本框
    tedit_dev = new QTextEdit(this);
    tedit_dev->setFont(ft_tedit);
    tedit_dev->setTextColor(Qt::green);
    tedit_dev->setTextBackgroundColor(Qt::black);
    tedit_dev->setStyleSheet("background-color:black;");
    tedit_dev->setText("Display Devices Information...");
//    tedit_dev->resize(380,100);
//    tedit_dev->setGeometry(grpbox_dev->x(),grpbox_dev->y()+20,370,180);

    QHBoxLayout *dbox_layout = new QHBoxLayout();
    dbox_layout->addWidget(tedit_dev);
    grpbox_dev->setLayout(dbox_layout);

/***************************************************************************/

/***************************************************************************/
    grpbox_avi = new QGroupBox(this);
    grpbox_avi->setTitle("航空信息");
    grpbox_avi->setTitle("Aviation Information");
    grpbox_avi->setFont(ft_grp);
    grpbox_avi->setPalette(pe_grp);
    grpbox_avi->setStyleSheet(pss1);
//    grpbox_avi->setGeometry(this->x(),this->y()+200,780,120);
    grpbox_avi->hide();
    //航空信息显示文本框
    tedit_avi = new QTextEdit(this);
    tedit_avi->setFont(ft_tedit);
    tedit_avi->setTextColor(Qt::green);
    tedit_avi->setTextBackgroundColor(Qt::black);
    tedit_avi->setStyleSheet("background-color:black;");
    tedit_avi->setText("Display Aviation Information...");
    tedit_avi->hide();
    //tedit_avi->setGeometry(20,400,984,180);


    QHBoxLayout *abox_layout = new QHBoxLayout(this);
    abox_layout->addWidget(tedit_avi);
    grpbox_avi->setLayout(abox_layout);

//    QVBoxLayout *vb_layout = new QVBoxLayout(this);
//    vb_layout->addWidget(grpbox_dev);
//    vb_layout->addWidget(grpbox_gps);
//    vb_layout->addWidget(grpbox_avi);
//    this->setLayout(vb_layout);
    QHBoxLayout *hb_layout = new QHBoxLayout(this);
    hb_layout->addWidget(grpbox_gps);
    hb_layout->addWidget(grpbox_dev);

    this->setLayout(hb_layout);
    //显示雷达动态扫描
//    label_gif = new QLabel(this);
//    movie = new QMovie(":/Icon/picture/scan2_radar.gif");
//    this->label_gif->setGeometry(839,400,180,180);
//    this->label_gif->setMovie(movie);
//    //this->label_gif->setText("this is a label");
//    movie->start();
    //显示网络状态
//    lab_netstat = new QLabel(this);
//    lab_netstat->setGeometry(904,0,40,40);
//    //this->lab_netstat->setText("Netstat");
//    imag_netstat = QPixmap(":/Icon/picture/net_scan1.jpg");
//    net_imag = imag_netstat.scaled(lab_netstat->size(),Qt::KeepAspectRatio);
//    lab_netstat->setPixmap(net_imag);

/*********************************手动发送指令配置*****************************************/
#if 0
    label_shieldid = new QLabel(this);
    label_paid = new QLabel(this);
    QPalette pe_shieldid;
    pe_shieldid.setColor(QPalette::WindowText,Qt::white);
    label_shieldid->setPalette(pe_shieldid);
    label_shieldid->setFont(font());
    label_paid->setPalette(pe_shieldid);
    label_paid->setFont(font());
    label_shieldid->setText("屏蔽器ID:");
    label_shieldid->setVisible(false);
    label_paid->setText("功放ID:");
    label_paid->setVisible(false);

    label_shieldid->setGeometry(240,10,80,30);
    label_paid->setGeometry(240,60,80,30);
    commbox_shieldid = new QComboBox(this);
    commbox_shieldid->addItem("0");
    commbox_shieldid->addItem("1");
    commbox_shieldid->addItem("2");
    commbox_shieldid->addItem("3");
    commbox_shieldid->addItem("4");
    commbox_shieldid->addItem("5");
    commbox_shieldid->addItem("6");
    commbox_shieldid->addItem("7");
    commbox_shieldid->addItem("8");
    commbox_shieldid->addItem("9");
    //commbox_shieldid->addItem("all");
    qDebug()<<"add commbox_shieldid member ok!";
    commbox_shieldid->setGeometry(330,10,80,30);
    commbox_shieldid->setVisible(false);

    commbox_paid = new QComboBox(this);
    commbox_paid->addItem("0");
    commbox_paid->addItem("1");
    commbox_paid->addItem("2");
    commbox_paid->addItem("3");
    commbox_paid->addItem("4");
    commbox_paid->addItem("5");
    commbox_paid->addItem("6");
    commbox_paid->addItem("7");
    commbox_paid->addItem("8");
    commbox_paid->addItem("9");
    commbox_paid->addItem("all");

    qDebug()<<"add commbox_paid member ok!";
    commbox_paid->setGeometry(330,60,80,30);
    commbox_paid->setVisible(false);

    pbutt_1 = new QPushButton(this);
    pbutt_1->setText("Start");
    pbutt_1->setGeometry(450,10,60,30);
    pbutt_1->setVisible(false);

    pbutt_2 = new QPushButton(this);
    pbutt_2->setText("Stop");
    pbutt_2->setGeometry(450,60,60,30);
    pbutt_2->setVisible(false);

    QRadioButton *rd_btn = new QRadioButton("&F", this);
    rd_btn->setStyleSheet("QRadioButton::indicator:checked {background-color:red;}"
                          "QRadioButton::indicator:unchecked {background-color:black;}");
    rd_btn->setGeometry(550,60,20,20);
    QFont ft_tedit_dev;
    ft_tedit_dev.setPointSize(12);
    tedit_avi->setFont(ft_tedit_dev);
    connect(rd_btn, SIGNAL(clicked(bool)), this, SLOT(on_rd_btn_clicked(bool)));
#endif
/***********************************航空信息****************************************/
#if 0
    process = new QProcess(this);
    my_pro_avi = new QProcess(this);
    connect(my_pro_avi, SIGNAL(readyRead()), this, SLOT(output_avi()));
    connect(my_pro_avi, SIGNAL(stateChanged(QProcess::ProcessState)),
            this, SLOT(showstat_avi_pro(QProcess::ProcessState)));
    connect(my_pro_avi, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showErr_avi_pro()));
    connect(my_pro_avi, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(showFini_avi_pro(int, QProcess::ExitStatus)));
    //my_pro_avi = new QProcess(this);  //创建对象，指定父类指针
    //my_pro_avi.start("/home/pi/opt/RTL_SDR/dump_1090/start.sh",QIODevice::ReadWrite);
    my_pro_avi->start("rtl_start.sh",QIODevice::ReadWrite);
    // 等待进程启动
    if (!my_pro_avi->waitForStarted())
    {
        qDebug() << "启动失败\n";
        this->tedit_avi->setText("启动失败！！\n");
        //return;
    }
    my_pro_avi->closeWriteChannel();
#endif
/**********************************GPS*****************************************/
#if 0
    my_qx_gps = new QProcess(this);

    QString qx_gps_path = "/home/pi/opt/NSDK-HL-MC120M-dev/run_qxgps";
    QStringList qx_gps_args;

    //connect(this, SIGNAL(get_gps_info()), this, SLOT(output_gps()));
    connect(my_qx_gps, SIGNAL(readyRead()), this, SLOT(output_gps()));
    //connect(my_qx_gps, SIGNAL(readyReadStandardOutput()), this, SLOT(output_gps()));
    //my_pro_gps.start("run_qxmf.sh",QIODevice::ReadWrite);
    connect(my_qx_gps, SIGNAL(stateChanged(QProcess::ProcessState)),
            this, SLOT(showstat_gps_pro(QProcess::ProcessState state)));
    connect(my_qx_gps, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showErr_gps_pro()));
    connect(my_qx_gps, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(showFini_gps_pro(int, QProcess::ExitStatus)));

#if 0
    //my_qx_gps->start("/home/pi/opt/NSDK-HL-MC120M-dev/demo_main.bin");
    //my_qx_gps->start("/home/pi/opt/NSDK-HL-MC120M-dev/run_qxmf.sh");
    my_qx_gps->start("run_qxmf.sh",QIODevice::ReadWrite);
    // 等待进程启动5
    if (!my_qx_gps->waitForStarted())
    {
        qDebug() << "启动失败\n";
        this->tedit_gps->setText("启动失败！！\n");
        //return;
    }
    my_qx_gps->closeWriteChannel();
#endif
/************************************GPS***************************************/
    connect(&time_out_gps, SIGNAL(timeout()), this, SLOT(Serial_gps_Read()));
    time_out_gps.start(1000);
    //sleep_ms(1000);


#endif
}

//退出提示
void Widget::closeEvent(QCloseEvent *event)
{
   // QMessageBox::StandardButton button;
    int button;
    button = QMessageBox::question(this, tr("退出系统"),
                                   QString(tr("确认退出系统?")),
                                   QMessageBox::No | QMessageBox::Yes);
    if(button == QMessageBox::No) {
        event->ignore();  //忽略退出信号，程序继续运行
    }
    else if(button == QMessageBox::Yes) {
        //检查进程是否存在
//        if(checkAppRunstat(SDR_COURSE_NAME)) {
//            killApp_process(SDR_COURSE_NAME);
//        }
        QString workingDir = QDir::currentPath();
        workingDir.append("restart.sh");
        qDebug()<<tr("-->>Restart System Current Path:%1").arg(workingDir)<<endl;
        system("echo 1 > number.bat");
        qDebug()<<tr("HiHi...")<<endl;
        if(avi_prostat) {
            killApp_process("run_qxgps");
            my_qx_gps->close();
        }
        if(avi_prostat) {
            killApp_process("dump1090");
            my_pro_avi->close();
        }
        emit this->closeConnect();
        sleep_ms(1000);
        //event->accept();  //接受退出信号，程序退出
        system("./restart.sh");
        sleep_ms(1000);
        qApp->closeAllWindows();

        //reboot();
    }
}
//重启应用程序
void Widget::reboot()
{
    QString program = QApplication::applicationFilePath();
    QStringList arguments = QApplication::arguments();
    QString workingDir = QDir::currentPath();
    QProcess::startDetached(program, arguments, workingDir);
    QApplication::exit();
}
void Widget::closeWidget()
{
    this->exit();
}
//
Widget::~Widget()
{
    qDebug()<<tr("OrOr...")<<endl;
    delete this;
}

void Widget::wd_Init(quint8 index)
{
#if 1
    /*******************************获取本地MAC信息开启网络监测线程******************************/
    discNet_count = 20;
    MyThreadFromQThread *thread1 = new MyThreadFromQThread(this);
    local_ipaddr = thread1->ipAddress("eth0");
    local_macaddr = thread1->macAddress("eth0");
    qDebug()<<"local_ipaddr:"<<local_ipaddr;
    qDebug()<<"local_macaddr:"<<local_macaddr;
    connect(thread1,SIGNAL(sigLookUpHostResult(bool)),this,SLOT(on_discNetProcess(bool)));
    connect(this,SIGNAL(send_reset4g),this,SLOT(send_DevcmdReset4G()));
    thread1->setSignal();
    thread1->start();

#endif

#if 1
    /****************************读取配置文件参数信息****************************/
    bool ok;
    get_mac_flag = false;
    dev_index = index;
    QFile file("./dev_info");
    if(index <= 0) {
        qDebug()<<tr("Unrecognized device type!")<<endl;
        return;
    }else {
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug()<<tr("Can't open %1 file!").arg(file.fileName())<<endl;
            return;
        }
        while(!file.atEnd()) {
            //qDebug()<<"open the file successful!"<<endl;
            QByteArray line = file.readLine();
            QString str(line);
            if(str.contains("Dev_Model:")) {
                QStringList str_list = str.split(":");
                for(int i = 0; i < str_list.count(); i++)  {
                    if(true == str_list[i].contains("HS_")) {
                        QString type = str_list[i].trimmed();
                        dev_info.dev_id.prepend(type);
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"Dev_Model:"<<dev_info.dev_id<<endl;
                        qDebug()<<"----------------------"<<endl;

                    }
                }
            }
            if(str.contains("Work_Pattern:")) {
                QStringList str_list = str.split(":");
                for(int i = 1; i < str_list.count(); i++) {
                    if(!str_list[i].isNull()) {
                        //0-工作模式 1-测试模式
                        dev_info.work_pattern = quint8(str_list[i].toInt(&ok,10));
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"Work_Pattern:"<<dev_info.work_pattern<<endl;
                        qDebug()<<"----------------------"<<endl;
                    }
                }
            }
            if(str.contains("Location_Model:")) {
                QStringList str_list = str.split(":");
                for(int i = 1; i < str_list.count(); i++) {
                    if(!str_list[i].isNull()) {
                        //0-工作模式 1-测试模式
                        location_flag = quint8(str_list[i].toInt(&ok, 10));
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"Location Model:"<<QString::number(location_flag)<<endl;
                        qDebug()<<"----------------------"<<endl;
                    }
                }
            }
            if(str.contains("MAC:")) {
                QStringList str2_list = str.split(":");
                dev_info.dev_id.append("_");
                for(int i = 0; i < str2_list.count(); i++)  {
                    if(false == str2_list[i].isNull()) {
                        dev_info.dev_id.append(str2_list[i]);
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"MAC:"<<str2_list[i]<<endl;
                        qDebug()<<"----------------------"<<endl;

                    }
                }
                get_mac_flag = true;
            }
            //设备数量
            if(str.contains("Group_Num:")) {
                qDebug()<<"-------------------"<<endl;
                QStringList str3_list = str.split(":");
                for(int i = 1; i < str3_list.count(); i++) {
                    if(false == str3_list[i].isNull()) {
                        dev_info.group_num = str3_list[i].toInt(&ok,10);
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"Group_Num:"<<dev_info.group_num<<endl;
                        qDebug()<<"----------------------"<<endl;

                    }
                }
            }
/*********************************设备类型*******************************/
/*
            if(str.contains("Dev_Type:")) {
                qDebug()<<"-------------------"<<endl;
                QStringList str4_list = str.split(":");
                for(int i = 1; i < str4_list.count(); i++) {
                    if(false == str4_list[i].isNull()) {
                        dev_info.dev_type = str4_list[i].toInt(&ok,10);
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"Dev_Type:"<<dev_info.dev_type<<endl;
                        qDebug()<<"----------------------"<<endl;
                    }
                }
            }
*/
            //子设备类型
            if((index == 2) && str.contains("Dev_SubType:")) {
                qDebug()<<"-------------------"<<endl;
                QStringList str_list = str.split(":");
                for(int i = 1; i < str_list.count(); i++) {
                    if(false == str_list[i].isNull()) {
                        dev_info.dev_subtype = str_list[i].toInt(&ok,10);
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"Dev_SubType:"<<dev_info.dev_subtype<<endl;
                        qDebug()<<"----------------------"<<endl;
                    }
                }
            }

    /*******************************设备协议版本*****************************/
/*
            //
//            if((index == 4) && (str.contains("Dev_Comm_Pro_Version:"))) {
//                if(str.contains("Dev_Comm_Pro_Version:")) {
//                    qDebug()<<"-------------------"<<endl;
//                    QStringList str_list = str.split(":");
//                    for(int i = 1; i < str_list.count(); i++) {
//                        if(false == str_list[i].isNull()) {
//                            dev_info.comm_pro_ver = str_list.at(i);
//                            dev_info.comm_pro_ver.remove("\n");

//                        }
//                    }
//                }
//                qDebug()<<"----------------------"<<endl;
//                qDebug()<<"Dev_Comm_Pro_Version:"<<dev_info.comm_pro_ver<<endl;
//                qDebug()<<"----------------------"<<endl;

//            }
*/
            //工作半径
            if(str.contains("WorkRadius:")) {
                qDebug()<<"-------------------"<<endl;
                QStringList str5_list = str.split(":");
                for(int i = 1; i < str5_list.count(); i++) {
                    if(false == str5_list[i].isNull()) {
                        dev_info.workradius = str5_list[i].toInt(&ok,10);
                        qDebug()<<"----------------------"<<endl;
                        qDebug()<<"WorkRadius:"<<dev_info.workradius<<endl;
                        qDebug()<<"----------------------"<<endl;
                    }
                }
            }
            /**********************tcp云哨侦测设备信息************************/
            if((index == 1) && (str.contains("Cloud_Whistle:"))) {
                cloudwhistle_num += 1;
                cloud_whistle = new Cloud_whistle(this);
                QStringList tmp_list = str.split(":");
                for(int i = 1; i < tmp_list.count(); i++) {
                    if(false == tmp_list[i].isNull()) {
                        if(i == 1) {
                            //cloud_whistle->tcp_cli->server_ip = tmp_list.at(i);
                            cloud_whistle->tcp_cli->server_ip = tmp_list.at(i);
                            cloud_whistle->server_ip = tmp_list.at(i);
                            stat_info.devIP = tmp_list.at(i);
                        }
                        if(i == 2) {
                            //cloud_whistle->tcp_cli->server_port = quint16(tmp_list.at(i).toInt());
                            cloud_whistle->tcp_cli->server_port = tmp_list.at(i).toInt();
                            cloud_whistle->server_port = tmp_list.at(i).toInt();
                        }
                    }
                }
                qDebug()<<"----------------------"<<endl;
                qDebug()<<"Cloud Whistle Device Number:"<<cloudwhistle_num<<endl;
                qDebug()<<"Cloud Whistle Info:"<<cloud_whistle->server_ip<<":"<<cloud_whistle->server_port<<endl;
                qDebug()<<"----------------------"<<endl;
            }
            /**********************https本地侦测设备信息************************/
            if((index == 2) && (str.contains("Wavedetec_HTTP:")) && (dev_info.comm_pro_ver == "HTTP")) {
                wavedetec_num += 1;
                http_cli = new Http_Client(this);
                QStringList tmp_list = str.split(":");
                for(quint8 i = 1; i < tmp_list.size(); i++) {
                    if(tmp_list[i].contains("https")) {
                        if(!tmp_list[i+1].isNull() && !tmp_list[i+2].isNull()) {

                            http_cli->url_ip = tmp_list[i] + ":" + tmp_list[i+1] + ":" + tmp_list[i+2];
                            stat_info.devIP = tmp_list[i+1].remove(QChar('/'), Qt::CaseInsensitive);
                            if(!tmp_list[i+3].isNull()) {
                                http_cli->md_User = tmp_list[i+3];
                            }
                            if(!tmp_list[i+4].isNull()) {
                                http_cli->md_Passwd = tmp_list[i+4];
                            }
                            wavedetec_stat = 1;
                            qDebug()<<"----------------------"<<endl;
                            qDebug()<<"WaveDetec Device Number:"<<wavedetec_num<<endl;
                            qDebug()<<"WaveDetec HTTP connect Info:"<<http_cli->url_ip<<endl;
                            qDebug()<<"----------------------"<<endl;
                        }
                    }
                }
            }
            /**********************tcp本地侦测设备信息************************/
            //微波设备      2020-4-6
            if((index == 2) && (str.contains("Wavedetec_TCP:")) && (dev_info.comm_pro_ver == "TCP")) {
                wavedetec_num += 1;
                wavedetec_dev = new WaveDetector_dev(this);
                QStringList tmp_list = str.split(":");
                for(int i = 1; i < tmp_list.count(); i++) {
                    //tcp协议设备
                    if(false == tmp_list[i].isNull()) {
                        if(i == 1) {
                            wavedetec_dev->tcp_cli->server_ip = tmp_list.at(i);
                            stat_info.devIP = tmp_list.at(i);
                        }
                        if(i == 2) {
                            wavedetec_dev->tcp_cli->server_port = quint16(tmp_list.at(i).toInt());
                        }
                        if(location_flag > 0)
                            wavedetec_dev->location_flag = true;
                        else
                            wavedetec_dev->location_flag = false;
                        wavedetec_dev->subdev_type = dev_info.dev_subtype;
                    }
                }
                wavedetec_stat = 2;
                qDebug()<<"----------------------"<<endl;
                qDebug()<<"WaveDetec Device Number:"<<wavedetec_num<<endl;
                qDebug()<<"WaveDetec Tcp connect Info:"<<wavedetec_dev->tcp_cli->server_ip<<":"<<wavedetec_dev->tcp_cli->server_port<<endl;
                qDebug()<<"----------------------"<<endl;

            }
            //ZMQ 2021-1-4
            if((index == 2) && (str.contains("Wavedetec_ZMQ:")) && (dev_info.comm_pro_ver == "ZMQ")) {
                wavedetec_num += 1;
                zmq_wavedetec_dev = new waveDetector_cd(this);
                QStringList tmp_list = str.split(":");
                for(int i = 1; i < tmp_list.count(); i++) {
                    if(false == tmp_list[i].isNull()) {
                        if(i == 1) {
                            zmq_wavedetec_dev->m_zeroMqcli->m_mqparam->ip_server = tmp_list.at(i);
                            stat_info.devIP = tmp_list.at(i);
                        }
                    }
                }
                wavedetec_stat = 3;
                qDebug()<<"----------------------"<<endl;
                qDebug()<<"WaveDetec Device Number:"<<wavedetec_num<<endl;
                qDebug()<<"WaveDetec ZMQ Connect Info:"<<zmq_wavedetec_dev->m_zeroMqcli->m_mqparam->ip_server<<endl;
                qDebug()<<"----------------------"<<endl;

            }
            //2020-12-12
            if(str.contains("Radar_UDP:")) {
                if(dev_info.dev_type == 15 && dev_info.dev_subtype == 0) {
                    radar_num += 1;
                    m_radar = new Radar(this);
                    QStringList tmp_list = str.split(":");
                    for(int i = 1; i < tmp_list.count(); i++) {
                        //UDP协议设备
                        if(false == tmp_list[i].isNull()) {
                            if(i == 1) {
                                m_radar->conn_ip = tmp_list.at(i);
                            }
                            if(i == 2) {
                                m_radar->port = quint16(tmp_list.at(i).toInt());
                            }

                            if(location_flag > 0) {
                                m_radar->location_flag = true;
                            }else {
                                m_radar->location_flag = false;
                            }
                            m_radar->subdev_type = dev_info.dev_subtype;

                        }
                    }
                    qDebug()<<"----------------------"<<endl;
                    qDebug()<<"Radar Device Number:"<<radar_num<<endl;
                    qDebug()<<"Radar UDP Info:"<<m_radar->conn_ip<<":"<<m_radar->port<<endl;
                    qDebug()<<"----------------------"<<endl;
                }
            }
            //ZMQ 2021-1-4
//            /**********************本地网络转CAN设备信息**********************/
/*
//            if((index == 7) && (str.contains("NetCan_TCP:"))) {
//                qDebug()<<"----------------------"<<endl;
//                netcan_dev = new Netcan_dev(this);
//                QStringList tmp_list = str.split(":");
//                for(int i = 1; i < tmp_list.count(); i++) {
//                    if(false == tmp_list[i].isNull()) {
//                        if(i == 1) {
//                            netcan_dev->tcp_cli->server_ip = tmp_list.at(i);
//                        }
//                        if(i == 2) {
//                            netcan_dev->tcp_cli->server_port = quint16(tmp_list.at(i).toInt());
//                        }
//                    }
//                }
//                qDebug()<<"----------------------"<<endl;
//                qDebug()<<tr("NetCan Device Info: IP = %1, Port = %2").arg(netcan_dev->tcp_cli->server_ip)
//                          .arg(netcan_dev->tcp_cli->server_port)<<endl;
//                qDebug()<<"----------------------"<<endl;
//                if(!netcan_dev->tcp_cli->server_ip.isEmpty() && (netcan_dev->tcp_cli->server_port != 0)) {
//                    connect(netcan_dev, &Netcan_dev::signal_recvdata, this, &Widget::slot_Netcan_pro);
//                    if(netcan_dev->tcp_cli->connectServer()) {
//                        if(netcan_dev->tcp_cli->m_isOkConect) {
//                            QThread *m_thread_tcp3 = new QThread(this);
//                            netcan_dev->moveToThread(m_thread_tcp3);
//                            m_thread_tcp3->start();
//                        }
//                    }
//                }
//            }
*/
            /**********************tcp云台设备信息************************/
            //云台设备      2020-4-6
            if((index == 7) && (str.contains("Clouddeck:"))) {
                //qDebug()<<"-------------------"<<endl;
                cloud_deck = new Cloud_Deck(this);
                clouddeck_num += 1;
                QStringList tmp_list = str.split(":");
                for(int i = 1; i < tmp_list.count(); i++) {
                    if(false == tmp_list[i].isNull()) {
                        if(i == 1) {
                            cloud_deck->tcpsocket_cli->server_ip = tmp_list.at(i);
                        }
                        if(i == 2) {
                            cloud_deck->tcpsocket_cli->server_port = quint16(tmp_list.at(i).toUInt());
                        }
                        if(i == 3) {
                            cloud_deck->dev_addr = quint8(tmp_list.at(i).toInt());
                        }

                    }
                }
                qDebug()<<"----------------------"<<endl;
                qDebug()<<"CloudDeck Info:"<<cloud_deck->tcpsocket_cli->server_ip<<":"<<cloud_deck->tcpsocket_cli->server_port<<":"<<cloud_deck->dev_addr<<endl;
                qDebug()<<"----------------------"<<endl;

            /**********************tcp云台设备信息************************/
            }
            //角度开关
            if(str.contains("AngleComp_Switch:")) {
                qDebug()<<"-------------------"<<endl;
                QStringList tmp_list = str.split(":");
    //            for(int i = 1; i < tmp_list.count(); i++) {
    //                if(false == tmp_list[i].isNull()) {
    //                    QVariant tmp_val = tmp_list[i];
    //                    dev_info.angle_sw = tmp_val.toBool();
    //                    //dev_info.angle_sw = tmp_list[i].toInt(&ok,10);
    //                    qDebug()<<"----------------------"<<endl;
    //                    qDebug()<<"AngleComp_Switch:"<<dev_info.angle_sw<<endl;
    //                    qDebug()<<"----------------------"<<endl;
    //                }
    //            }
                int tmp_int = tmp_list[1].toInt();
                if(tmp_int == 0) {
                    dev_info.angle_sw = false;
                } else {
                    dev_info.angle_sw = true;
                }
                qDebug()<<"----------------------"<<endl;
                qDebug()<<"AngleComp_Switch:"<<dev_info.angle_sw<<endl;
                qDebug()<<"----------------------"<<endl;
            }
            //俯仰角
            if(str.contains("PitCompAngle:")) {
                if(dev_info.angle_sw == true) {
                    qDebug()<<"--------Read PitCompAngle-----------"<<endl;
                    QStringList tmp_list = str.split(":");
                    dev_info.pit_compangle = tmp_list[1].toInt(&ok,10);
                    qDebug()<<"----------------------"<<endl;
                    qDebug()<<"Pit_Angle:"<<dev_info.pit_compangle<<endl;
                    qDebug()<<"----------------------"<<endl;
                }
                else {
                    qDebug()<<"---Not Read PitCompAngle---"<<endl;
                }
            }
            //水平角
            if(str.contains("HorCompAngle:")) {
                if(dev_info.angle_sw == true) {
                    qDebug()<<"--------Read HorCompAngle-----------"<<endl;
                    QStringList tmp_list = str.split(":");
                    for(int i = 1; i < tmp_list.count(); i++) {
                        if(false == tmp_list[i].isNull()) {
                            dev_info.hor_compangle = tmp_list[i].toInt(&ok,10);
                            qDebug()<<"----------------------"<<endl;
                            qDebug()<<"Hor_Angle:"<<dev_info.hor_compangle<<endl;
                            qDebug()<<"----------------------"<<endl;
                        }
                    }
                } else {
                    qDebug()<<"---Not Read HorCompAngle---"<<endl;
                }
            }
        }
        file.close();
    }
    if(!dev_info.dev_id.isEmpty()) {
        if(index == 4) {
            dev_info.dev_id = "HS_09B90D01";
        }else if(index == 5) {
            dev_info.dev_id = "HS_8QX360B01";
        }else
            dev_info.dev_id = "HS_09B90D01";
    }
    Get_DevMac();

    qDebug()<<tr(">>>Get Data Box ID: %1").arg(dev_info.dev_id)<<endl;
    switch (index) {
    case 1: //云哨设备
        dev_info.dev_type = 13;
        dev_info.workradius = 2000;
        if(cloudwhistle_num > 0) {
            cloud_whistle->databox_id = dev_info.dev_id;
            qDebug()<<tr(">>>Cloud Whistle Device Get Data Box ID: %1").arg(dev_info.dev_id)<<endl;
            qDebug()<<tr("---------Start Connect Cloud Whistle Device---------")<<endl;
            if((!cloud_whistle->tcp_cli->server_ip.isEmpty()) && (cloud_whistle->tcp_cli->server_port != 0)) {
                connect(cloud_whistle, &Cloud_whistle::sign_sendData, this, &Widget::slot_sendmdInfo);
                while(!cloud_whistle->tcp_cli->m_isOkConect) {
                    cloud_whistle->tcp_cli->connectServer();
                    sleep_ms(5000);
                }
                if(cloud_whistle->tcp_cli->m_isOkConect) {
                    QThread *m_thread_tcp1 = new QThread(this);
                    cloud_whistle->moveToThread(m_thread_tcp1);
                    m_thread_tcp1->start();
                }
            }
        }
        break;
    case 2: //侦测设备
        dev_info.dev_type = 14;
        dev_info.workradius = 2000;
        if(wavedetec_num > 0) {
            if(wavedetec_stat == 1) {
                http_cli->databox_id = dev_info.dev_id;
                qDebug()<<tr(">>>HTTP Wavedetec: Databox ID = %1").arg(http_cli->databox_id)<<endl;
                qDebug()<<"--------Start Connect HTTP Wavedetec Device -----------"<<endl;
                while(!http_cli->log_stat) {
                    this->http_cli->Http_login(http_cli->url_ip);
                    sleep_ms(5000);
                    if(http_cli->log_stat) {
                        qDebug()<<tr(">>>Https:登录成功！")<<endl;
                        http_cli->up_data_type = dev_info.dev_type;
                        connect(this->http_cli, &Http_Client::Signal_AnalysisFinished, this, &Widget::slot_sendmdInfo);
                        //timer_out_md.start(1000);
                        QThread *m_thread_http= new QThread(this);
                        http_cli->moveToThread(m_thread_http);
                        m_thread_http->start();
                    }
                    break;
                }

            } else if (wavedetec_stat == 2) {
                wavedetec_dev->databox_id = dev_info.dev_id;
                qDebug()<<tr(">>>TCP Wavedetec: Databox ID = %1").arg(wavedetec_dev->databox_id)<<endl;
                qDebug()<<"--------Start TCP Connect Wavedetec Device -----------"<<endl;
                if(!wavedetec_dev->tcp_cli->server_ip.isEmpty() && (wavedetec_dev->tcp_cli->server_port !=0)) {
                    connect(wavedetec_dev, &WaveDetector_dev::sign_sendData, this, &Widget::slot_sendmdInfo);
                    while(!wavedetec_dev->tcp_cli->m_isOkConect)
                    {
                        wavedetec_dev->tcp_cli->connectServer();
                        sleep_ms(5000);
                    }
                    qDebug()<<tr("Connet to WaveDetec Device Done!")<<endl;

                    if(wavedetec_dev->tcp_cli->m_isOkConect) {
                        QThread *m_thread_tcp2 = new QThread(this);
                        wavedetec_dev->moveToThread(m_thread_tcp2);
                        m_thread_tcp2->start();
                    }
                }
            } else if (wavedetec_stat ==3) {    //2021-1-4
                zmq_wavedetec_dev->databox_id = dev_info.dev_id;
                qDebug()<<tr(">>>ZMQ Wavedetec: Databox ID = %1").arg(zmq_wavedetec_dev->databox_id)<<endl;
                qDebug()<<"--------Start ZMQ Connect Wavedetec Device -----------"<<endl;
                if(!zmq_wavedetec_dev->m_zeroMqcli->m_mqparam->ip_server.isEmpty()) {
                    connect(zmq_wavedetec_dev, &waveDetector_cd::sig_sendUpInfo, this, &Widget::slot_sendmdInfo);
                    zmq_wavedetec_dev->subdev_type = dev_info.dev_subtype;
                    zmq_wavedetec_dev->m_zeroMqcli->start();
                    while(!zmq_wavedetec_dev->m_zeroMqcli->conn_stat) {
                        sleep_ms(3000);
                        if(zmq_wavedetec_dev->m_zeroMqcli->conn_stat) {
                            zmq_wavedetec_dev->dev_Init();
                            QThread *m_thread_zmq = new QThread(this);
                            zmq_wavedetec_dev->moveToThread(m_thread_zmq);
                            m_thread_zmq->start();
                            break;
                        }
                    }
                }
            } else {
                qDebug()<<tr(">>>Not Detection Wave Detec Device!")<<endl;
            }
        }
        break;
    case 3: //雷达设备
        dev_info.dev_type = 15;
        dev_info.workradius = 5000;
        if(radar_num > 0) {
            m_radar->databox_id = dev_info.dev_id;
            qDebug()<<tr(">>>Radar Device Get Data Box ID: %1").arg(dev_info.dev_id)<<endl;
            qDebug()<<tr("---------Start Connect Radar Device---------")<<endl;
            if((!m_radar->conn_ip.isEmpty()) && (m_radar->port != 0)) {
                connect(m_radar, &Radar::sig_sendJsData, this, &Widget::slot_sendmdInfo);
                while (!m_radar->online_stat) {
                    emit m_radar->sig_start();
                    sleep_ms(5000);
                }
                qDebug()<<tr("Create Radar UDP Connect Done!")<<endl;
            }
            if(m_radar->online_stat) {
                radar_stat = 1;
                QThread *m_thread_udp = new QThread(this);
                m_radar->moveToThread(m_thread_udp);
                m_thread_udp->start();
            }
        }
        break;
    case 4: //便携定向
        dev_info.dev_type = 3;
        dev_info.workradius = 1000;
        break;
    case 5: //便携全向
        dev_info.dev_type = 2;
        dev_info.workradius = 500;
        break;
    case 6: //船载设备
        dev_info.dev_type = 7;
        dev_info.workradius = 1000;
        break;
    default:
        break;
    }
    if(clouddeck_num > 0) {
        if(!cloud_deck->tcpsocket_cli->server_ip.isEmpty() && (cloud_deck->tcpsocket_cli->server_port != 0)) {
            if(cloud_deck->tcpsocket_cli->connectServer()) {
                QThread *m_thread_cd = new QThread(this);
                cloud_deck->moveToThread(m_thread_cd);
                m_thread_cd->start();
            }
        }
    }
#endif

/*********************************websocket******************************************/
        w_socket = new DataReceive(this);
        send_timer = new QTimer();
        if(dev_info.work_pattern == 0) {    //工作模式
            w_socket->URL = "ws://112.126.101.172:7001/websocket/bg/";
        }else {
            w_socket->URL = "ws://123.56.133.177:7001/websocket/bg/";
        }
        w_socket->dev_id = dev_info.dev_id;
        this->connect(this,SIGNAL(startConnect()),w_socket,SLOT(createDataRecvWS()));
        this->connect(this,SIGNAL(closeConnect()),w_socket,SLOT(closeConnected()));
        this->connect(this,SIGNAL(sendMsg(QString)),w_socket,SLOT(sendTextmsg(QString)));
        //this->connect(this,SIGNAL(send_byteMsg(QByteArray)),w_socket,SLOT(sendBinaryy(QByteArray)));
        this->connect(send_timer,SIGNAL(timeout()),this,SLOT(add_sendMsg()));
        this->connect(w_socket,SIGNAL(recv_Msg_pro(QByteArray)),this,SLOT(recvDowninfo_parser(QByteArray)));


        if(index < 4 && index > 0) {
            //无人机数据接口
            md_socket = new DataReceive(this);      //2020-3-31
            md_send_timer = new QTimer();
            md_socket->dev_id = "";
            this->connect(this,SIGNAL(startConnect()),md_socket,SLOT(createDataRecvWS()));
            this->connect(this,SIGNAL(closeConnect()),md_socket,SLOT(closeConnected()));
            this->connect(this,SIGNAL(sendmdMsg(QString)),md_socket,SLOT(sendTextmsg(QString)));
            this->connect(this->md_send_timer, &QTimer::timeout, this, &Widget::timer_sendHeartBeatMsg);
            //Server "ws://112.126.101.172:7001/websocket/droneresource/"
            //test Server "http://123.56.133.177:7001/websocket/droneresource/"
            if(dev_info.work_pattern == 0) {
                md_socket->URL = "ws://112.126.101.172:7001/websocket/droneresource/";
            }else if(dev_info.work_pattern == 1) {
                md_socket->URL = "ws://123.56.133.177:7001/websocket/droneresource/";
            }else {
                md_socket->URL = "ws://112.126.101.172:7001/websocket/droneresource/";

                md_socket2 = new DataReceive(this);
                md_socket2->URL = "ws://112.126.100.124:8000";   //websockets测试接口
                md_socket2->dev_id = "";

                this->connect(this,SIGNAL(startConnect()),md_socket2,SLOT(createDataRecvWS()));
                this->connect(this,SIGNAL(closeConnect()),md_socket2,SLOT(closeConnected()));
                this->connect(this,SIGNAL(sendmdMsg(QString)),md_socket2,SLOT(sendTextmsg(QString)));
                this->connect(this,&Widget::sendmdHeartbeat, md_socket2, &DataReceive::sendTextmsg);
            }
//            md_send_timer->start(30*1000);
        }
/*********************************串口初始化******************************************/

    port_list = new SerialPortList(this);
    port_list->ScanStart();
    connect(port_list, SIGNAL(onNewSerialPort(QStringList)), this, SLOT(disp_port_list(QStringList)));
#if 1
    emit this->startConnect();
    sleep_ms(5000);
    send_timer->start(2000);

#endif
}
//输出航空信息
void Widget::output_avi()
{
    QString comp_str;
    //my_pro_avi_recv.resize(1024);
    my_pro_avi_recv.clear();
    //tedit_avi->clear();
    my_pro_avi_recv = my_pro_avi->readAllStandardOutput();
    //my_pro_avi_recv = my_pro_avi.readAll();
    comp_str.append(my_pro_avi_recv);

    QString tmp_str = QString::number(recv_count_avi++);
    this->tedit_avi->setText(my_pro_avi_recv);
    autoScrollTextEdit(tedit_avi, comp_str, tmp_str);
#ifdef _DEBUG_
    qDebug()<<"my_pro_avi_recv = "<<my_pro_avi_recv<<endl;
#endif
    QStringList avi_list = Avi_prot_anal(my_pro_avi_recv, my_pro_avi_recv.length());
    //qDebug()<<"avi_list = "<<avi_list;
    Avi_info_pro(avi_list);
}
//输出GPS信息
void Widget::output_gps()
{
    QString comp_str;
    my_pro_gps_recv.clear();
    recv_buff_gps.clear();
    my_pro_gps_recv = my_qx_gps->readAllStandardOutput();
    recv_buff_gps = my_pro_gps_recv;
    //comp_str.append(my_pro_gps_recv);

    QString tmp_str = QString::number(recv_count_gps++);
    this->tedit_gps->setText(my_pro_gps_recv);
    autoScrollTextEdit(tedit_gps, comp_str, tmp_str);
#ifdef _DEBUG_
    qDebug()<<"Recive MC-120 Data len:"<<my_pro_gps_recv.length()<<endl;
    qDebug()<<"Recive MC-120 Data:"<<my_pro_gps_recv<<endl;
#endif
    //my_pro_gps_recv = my_pro_gps.readAll();
    //this->tedit_gps->setText(my_pro_gps_recv);
}
//输出设备信息
void Widget::output_dev()
{
    my_pro_dev_recv = my_pro_dev.readAll();
    this->tedit_dev->setText(my_pro_dev_recv);

}
/**********************************slot my_pro_avi*************************************/
//显示状态
void Widget::showstat_avi_pro(QProcess::ProcessState state)
{
    qDebug()<<"show avi pro state";
    if(state==QProcess::NotRunning){
        qDebug() << "Not Running";
    }else if(state==QProcess::Starting){
        qDebug() << "Starting";;
    }else{
        qDebug() << "Running";
    }
}
//显示飞行错误
void Widget::showErr_avi_pro()
{
    qDebug()<<"showError:"<<endl<<my_pro_avi->errorString();
}
//显示飞行信息
void Widget::showFini_avi_pro(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "showFinished:" << endl << exitCode << exitStatus;
    //procOutput = myProcess.readAll(); /* 读取标准输出数据 */
   // qDebug() << procOutput << endl;
}
/**********************************slot my_pro_avi*************************************/
/**********************************slot my_pro_gps*************************************/
//显示状态
void Widget::showstat_gps_pro(QProcess::ProcessState state)
{
    qDebug()<<"show avi pro state";
    if(state==QProcess::NotRunning){
        qDebug() << "Not Running";
    }else if(state==QProcess::Starting){
        qDebug() << "Starting";;
    }else{
        qDebug() << "Running";
    }
}
//显示错误
void Widget::showErr_gps_pro()
{
    qDebug()<<"showError:"<<endl<<my_pro_gps.errorString();
}
void Widget::showFini_gps_pro(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "showFinished:" << endl << exitCode << exitStatus;
    //procOutput = myProcess.readAll(); /* 读取标准输出数据 */
   // qDebug() << procOutput << endl;
}
/**********************************slot my_pro_gps*************************************/
/**********************************serialport*************************************/
//显示串口列表
void Widget::disp_port_list(QStringList str_list)
{
    QString tmp_str;
    QString sub0_str;
    QString sub1_str;
    QString sub2_str;
    QString sub3_str;
    tmp_str = "Search for the following ports>>";
    tmp_str += str_list.join(",");
    this->tedit_dev->clear();
    this->tedit_dev->setText(tmp_str);
    this->port_list->ScanStop();
    qDebug()<<tmp_str;
    sub0_str = tmp_str.section(',',0,0);
    sub1_str = tmp_str.section(',',1,1);
    sub2_str = tmp_str.section(',',2,2);
    sub3_str = tmp_str.section(',',3,3);
    qDebug()<<tr("Comm0 = %1, Comm1 = %2, Comm2 = %3, Comm3 = %4").arg(sub0_str).arg(sub1_str).arg(sub2_str).arg(sub3_str)<<endl;
    if(this->Comm_Init(sub1_str, sub2_str, sub3_str) > 0) {
        qDebug()<<">>>Widget:Comm Interface Init done.<<<"<<endl;
    }else {
        qDebug()<<">>>Widget:Comm Interface Init Error.<<<"<<endl;
    }
}
//串口初始化
int Widget::Comm_Init(QString sub1_str, QString sub2_str, QString sub3_str)
{
    if(dev_info.comm_pro_ver == "V1.0") {

    }else if(dev_info.comm_pro_ver == "V2.0") {

    }else {
        qDebug()<<tr("---Comm Init:Device Communication protocol version Error!---")<<endl;
        return -1;
    }
    Dev_serial = new SerialPort("ttyUSB4", 115200);
    Gps_serial = new SerialPort("ttyS4", 115200);
    //Test_serial = new SerialPort(sub3_str, 115200);
    qDebug()<<tr("-->>Widget:Start Init Serial...")<<endl;
    //线程连接
//    connect(this->pbutt_1, SIGNAL(clicked()), this, SLOT(pbutt_start_event()));
//    connect(this->pbutt_2, SIGNAL(clicked()), this, SLOT(pbutt_stop_event()));
    connect(this, SIGNAL(serialsend(QByteArray)), Dev_serial, SLOT(write_data(QByteArray)));
    if(dev_index == 4)
        connect(Dev_serial, SIGNAL(receive_data(QByteArray)), this, SLOT(on_receive_dev(QByteArray)), Qt::QueuedConnection);
    connect(Gps_serial, SIGNAL(receive_data(QByteArray)), this, SLOT(on_receive_gps(QByteArray)), Qt::QueuedConnection);
    //开启定时处理GPS串口数据
    connect(&time_out_gps, SIGNAL(timeout()), this, SLOT(Serial_gps_Read()));
    sleep_ms(1000);
    if(1) {
        qDebug()<<tr("-->>Widget:Send Device CAN Init Command1...")<<endl;
        emit serialsend("AT+CAN_MODE=0\r\n");
        qDebug()<<tr("-->>Widget Start Init Serial List...")<<endl;
        sleep_ms(100);
        qDebug()<<tr("-->>Widget:Send Device CAN Init Command2...")<<endl;
        emit serialsend("AT+AT\r\n");     //2020-4-22
        sleep_ms(100);
        qDebug()<<"Comm Port Send CFG CAN Moudles Data Done.-->>"<<endl;
    }
    time_out_gps.start(800);
/*
    //设备串口连接
    connect(&comm_dev, &QSerialPort::readyRead, this, &Widget::comm_dev_delay);
    connect(&time_out_dev, SIGNAL(timeout()), this, SLOT(Serial_dev_Read()));
    //GPS串口连接
    connect(&comm_gps, &QSerialPort::readyRead, this, &Widget::comm_gps_delay);
    connect(&time_out_dev, SIGNAL(timeout()), this, SLOT(Serial_gps_Read()));

    comm_gps.setPortName(sub2_str);
    qDebug()<<"comm_gps port = "<<sub2_str;
    switch (COMM_GPS_BAUD) {
    case 4800:
        comm_gps.setBaudRate(QSerialPort::Baud4800);
        break;
    case 9600:
        comm_gps.setBaudRate(QSerialPort::Baud9600);
        qDebug()<<"comm_gps Band = "<< "9600";
        break;
    case 19200:
        comm_gps.setBaudRate(QSerialPort::Baud19200);
        break;
    case 38400:
        comm_gps.setBaudRate(QSerialPort::Baud38400);
        break;
    case 57600:
        comm_gps.setBaudRate(QSerialPort::Baud57600);
        break;
    case 115200:
        comm_gps.setBaudRate(QSerialPort::Baud115200);
        qDebug()<<"comm_gps Band = "<< "115200";
        break;
    default:
        break;
    }
//    comm.setBaudRate(QSerialPort::Baud115200);
    comm_gps.setDataBits(QSerialPort::Data8);
    comm_gps.setParity(QSerialPort::NoParity);
    comm_gps.setStopBits(QSerialPort::OneStop);
    comm_gps.setFlowControl(QSerialPort::NoFlowControl);
    comm_gps.open(QIODevice::ReadWrite);


    comm_dev.setPortName(sub1_str);
    qDebug()<<"comm_dev port = "<<sub1_str;
    switch (COMM_DEV_BAUD) {
    case 4800:
        comm_dev.setBaudRate(QSerialPort::Baud4800);
        break;
    case 9600:
        comm_dev.setBaudRate(QSerialPort::Baud9600);
        qDebug()<<"comm_dev Baud = "<<"9600";
        break;
    case 19200:
        comm_dev.setBaudRate(QSerialPort::Baud19200);
        break;
    case 38400:
        comm_dev.setBaudRate(QSerialPort::Baud38400);
        break;
    case 57600:
        comm_dev.setBaudRate(QSerialPort::Baud57600);
        break;
    case 115200:
        comm_dev.setBaudRate(QSerialPort::Baud115200);
        qDebug()<<"comm_dev Baud = "<<"115200";
        break;
    default:
        break;
    }
//    comm.setBaudRate(QSerialPort::Baud115200);
    comm_dev.setDataBits(QSerialPort::Data8);
    comm_dev.setParity(QSerialPort::NoParity);
    comm_dev.setStopBits(QSerialPort::OneStop);
    comm_dev.setFlowControl(QSerialPort::NoFlowControl);
    comm_dev.open(QIODevice::ReadWrite);
*/
    return 1;
}

void Widget::Get_DevMac()
{
    if(get_mac_flag!=true) {
        QStringList mac_list = local_macaddr.split(":");
        dev_info.dev_id.append("_");
        for(int i = 0; i < mac_list.count(); i++) {
            dev_info.dev_id.append(mac_list[i]);
        }
        qDebug()<<"----------------------"<<endl;
        qDebug()<<"Dev_Id:"<<dev_info.dev_id<<endl;
        qDebug()<<"----------------------"<<endl;
    }
}
//gps串口延时
void Widget::comm_gps_delay()
{
    QString rev_str;

    time_out_gps.start(800);
    recv_buff.clear();
    //recv_buff.append(tmpdata);
}
//gps串口接收读取
void Widget::Serial_gps_Read()
{
    QString tmp_str;
    QString comp_str;
    QString gps_str;
    QDateTime date_time = QDateTime::currentDateTime();
    QTime current_time = QTime::currentTime();
    time_out_gps.stop();

    if(recv_buff_gps.length() != 0)
    {
        comp_str.append("[");
        comp_str.append(date_time.date().toString());
        comp_str.append(current_time.toString());
        comp_str.append("]:");
        comp_str.append(recv_buff_gps);
        tmp_str = QString::number(recv_count_gps++);
        qDebug()<<comp_str;
        autoScrollTextEdit(tedit_gps, comp_str, tmp_str);
    }
    //解析GPS数据
    gps_str = Gps_prot_anal(recv_buff_gps, recv_buff_gps.length());
    gps_str.prepend("$");
    //增加下发GPS数据到中控板     2020-4-5
    double lat;     //纬度
    double lon;     //经度
    if(gps_str.contains("GNGGA")) {
        QStringList str_list = gps_str.split(',');
        lat = str_list.at(2).toDouble();
        lon = str_list.at(4).toDouble();
        qDebug()<<tr(">>>Get GPS lat = %1, lon = %2<<<").arg(lat).arg(lon)<<endl;
        QByteArray send_cmd;
        char tmp_cmd[16];
        if((lat != 0.0) && (lon != 0.0)) {
            tmp_cmd[0] = char(0xA0);
            tmp_cmd[1] = char(0xB0);
            tmp_cmd[2] = 0x0A;
            tmp_cmd[3] = 0x05;
            tmp_cmd[4] = 0x01;
            tmp_cmd[5] = char(quint32(lat*100000)>>24&0xFF);
            tmp_cmd[6] = char(quint32(lat*100000)>>16&0xFF);
            tmp_cmd[7] = char(quint32(lat*100000)>>8&0xFF);
            tmp_cmd[8] = char(quint32(lat*100000)&0xFF);
            tmp_cmd[9] = char(quint32(lon*100000)>>24&0xFF);
            tmp_cmd[10] = char(quint32(lon*100000)>>16&0xFF);
            tmp_cmd[11] = char(quint32(lon*100000)>>8&0xFF);
            tmp_cmd[12] = char(quint32(lon*100000)&0xFF);
            tmp_cmd[13] = 0x0B;
            tmp_cmd[14] = 0x0A;
            tmp_cmd[15] = char(cloud_deck->cal_checkSum(tmp_cmd, 15));
            send_cmd.append(tmp_cmd, 16);
            emit serialsend(send_cmd);      //发送设备指令  2020-4-30
            //test_SendShield_Cmd();  //2020-5-1
            //qDebug()<<tr(">>>>Send Main Control Panel command:")<<send_cmd.toHex()<<"<<<<"<<endl;
//            send_cmd.clear();         //2020-4-22
//            send_cmd = "AT+AT\n";
//            emit serialsend(send_cmd);
//            qDebug()<<tr(">>>Send Config Can Module command:")<<send_cmd<<endl;
        }

    }
    //增加下发GPS数据到中控板     2020-4-5
    qDebug()<<"gps_str:"<<gps_str<<endl;
    this->JS_gps_info_obj.insert("GNGGA", gps_str);       //GNGGA信息
    recv_buff_gps.clear();
    time_out_gps.start(1000);
}
//添加消息内容 --有效
void Widget::add_sendMsg()
{
    QJsonObject tmp1_jsonobj, tmp2_jsonobj;
    QJsonObject send_jsonobj;
    QJsonValue tmp1_val, tmp2_val;

    QJsonArray JS_freq_range_arr;   //频段信息

    int m = 0;
    int tmp_jsoncnt;
    send_timer->stop();
    //DEBUG 输出设备信息
    qDebug()<<tr(">>>>Current Group_num = %1 <<<<").arg(dev_info.group_num)<<endl;
    //2021-1-8
    if(dev_info.dev_type == 2 || dev_info.dev_type == 3) {
        if(dev_info.comm_pro_ver == "V2.0") {
            for(quint8 i = 0; i < 16; i++) {
                qDebug()<<tr(">>>>Command version V2.0 Current Shield ID = %1, CH_Count = %2<<<<")
                          .arg(dev_info.shield_arry[i].shield_id)
                          .arg(dev_info.shield_arry[i].ch_count)<<endl;
                if(dev_info.shield_arry[i].ch_count > 0 &&
                        dev_info.shield_arry[i].shield_id != 0xF0) {
                    QJsonObject JS_shield_obj;      //屏蔽器
                    QJsonArray JS_shield_info_arr;  //屏蔽器信息
        //            qDebug()<<tr(">>>>Current Shield ID = %1, CH_Count = %2<<<<")
        //                      .arg(dev_info.shield_arry[i].shield_id)
        //                      .arg(dev_info.shield_arry[i].ch_count)<<endl;
                    for(quint8 j = 0; j < 32; j++) {
                        QJsonObject JS_shield_info_obj; //屏蔽器
                        if((dev_info.shield_arry[i].ch_info[j].ch_id != 0xF0) &&
                                (dev_info.shield_arry[i].ch_info[j].ch_freq != 0)) {
                            //获取功放告警状态
                            quint8 tmp_num = dev_info.shield_arry[i].pa_info[j].alarm_stat;
                            qint8 stat;
                            if((tmp_num&0x1) == 0x1)
                                stat = -1;  //过温
                            else if((tmp_num&0x2) == 0x2)
                                stat = -2;  //过功率
                            else if((tmp_num&0x4) ==0x4)
                                stat = -3;  //驻波告警
                            else if((tmp_num&0x8) == 0x8)
                                stat = -4;  //输出欠功率
                            else if((tmp_num&0x10) == 0x10)
                                stat = -5;  //过流
                            else if((tmp_num&0x20) == 0x20)
                                stat = -6;  //欠压告警
                            else if((tmp_num&0x40) == 0x40)
                                stat = -7;  //过压告警
                            else if((tmp_num&0x60) == 0x60)
                                stat = -8;  //参数错误
                            else if((tmp_num&0x80) == 0x80)
                                stat = -9;
                            else
                                stat = 0;    //正常
                            JS_shield_info_obj.insert("id", dev_info.shield_arry[i].ch_info[j].ch_id);
                            JS_shield_info_obj.insert("frequency", dev_info.shield_arry[i].ch_info[j].ch_freq);
                            JS_shield_info_obj.insert("switch",dev_info.shield_arry[i].ch_info[j].ch_swtich);
                            JS_shield_info_obj.insert("current", dev_info.shield_arry[i].ch_info[j].ch_current);
                            JS_shield_info_obj.insert("power_gear", dev_info.shield_arry[i].ch_info[j].ch_power_gear);
                            JS_shield_info_obj.insert("vswr", dev_info.shield_arry[i].ch_info[j].ch_swr);
                            JS_shield_info_obj.insert("positivePower", dev_info.shield_arry[i].ch_info[j].ch_current*28);
                            JS_shield_info_obj.insert("temperature", dev_info.shield_arry[i].shield_temp);
                            JS_shield_info_obj.insert("voltage", 28);
                            JS_shield_info_obj.insert("status", 0);
                            //JS_shield_info_obj.insert("band", 0);
                            //JS_shield_info_obj.insert("reversePower", 0);
                            qDebug()<<tr(">>>>Command version V2.0 Current CH Param: CH_ID = %1, CH_Freq = %2, CH_Swtich = %3, CH_Temp = %4, CH_Curr = %5, CH_PG = %6, CH_SWR = %7, CH_PP = %8<<<<")
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_id)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_freq)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_swtich)
                                      .arg(dev_info.shield_arry[i].shield_temp)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_current)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_power_gear)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_swr)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_current*28)<<endl;
                            dev_info.shield_arry[i].ch_info[j].ch_id = 0;
                            dev_info.shield_arry[i].ch_info[j].ch_freq = 0;
                            dev_info.shield_arry[i].ch_info[j].ch_swtich = 0;
                            dev_info.shield_arry[i].ch_info[j].ch_current = 0;
                            dev_info.shield_arry[i].ch_info[j].ch_power_gear = 0;
                            dev_info.shield_arry[i].ch_info[j].ch_swr = 0;
                            dev_info.shield_arry[i].shield_temp = 0;

                            JS_shield_info_arr.append(JS_shield_info_obj);
                        }
                    }
                    JS_shield_obj.insert("shield_id", dev_info.shield_arry[i].shield_id);
                    JS_shield_obj.insert("shield_info", JS_shield_info_arr);
                    JS_freq_range_arr.append(JS_shield_obj);
                }
            }
        }else if(dev_info.comm_pro_ver == "V1.0") {
            for(quint8 i = 0; i < 16; i++) {
                qDebug()<<tr(">>>>Command version V1.0 Current Shield ID = %1, CH_Count = %2<<<<")
                          .arg(dev_info.shield_arry[i].shield_id)
                          .arg(dev_info.shield_arry[i].ch_count)<<endl;
                if(dev_info.shield_arry[i].ch_count > 0 &&
                        dev_info.shield_arry[i].shield_id != 0xF0) {
                    QJsonObject JS_shield_obj;      //屏蔽器
                    QJsonArray JS_shield_info_arr;  //屏蔽器信息
                    qDebug()<<tr(">>>>Current Shield ID = %1, CH_Count = %2<<<<")
                              .arg(dev_info.shield_arry[i].shield_id)
                              .arg(dev_info.shield_arry[i].ch_count)<<endl;
                    for(quint8 j = 0; j < 32; j++) {
                        QJsonObject JS_shield_info_obj; //屏蔽器
                        if((dev_info.shield_arry[i].ch_info[j].ch_id != 0xF0) &&
                                (dev_info.shield_arry[i].pa_info[j].freq != 0)) {
                            //获取功放告警状态
                            quint8 tmp_num = dev_info.shield_arry[i].pa_info[j].alarm_stat;
                            qint8 stat;
                            if((tmp_num&0x1) == 0x1)
                                stat = -1;  //过温
                            else if((tmp_num&0x2) == 0x2)
                                stat = -2;  //过功率
                            else if((tmp_num&0x4) ==0x4)
                                stat = -3;  //驻波告警
                            else if((tmp_num&0x8) == 0x8)
                                stat = -4;  //输出欠功率
                            else if((tmp_num&0x10) == 0x10)
                                stat = -5;  //过流
                            else if((tmp_num&0x20) == 0x20)
                                stat = -6;  //欠压告警
                            else if((tmp_num&0x40) == 0x40)
                                stat = -7;  //过压告警
                            else if((tmp_num&0x60) == 0x60)
                                stat = -8;  //参数错误
                            else if((tmp_num&0x80) == 0x80)
                                stat = -9;
                            else
                                stat = 0;    //正常

                            JS_shield_info_obj.insert("id", dev_info.shield_arry[i].ch_info[j].ch_id);
                            JS_shield_info_obj.insert("frequency", dev_info.shield_arry[i].pa_info[j].freq);
                            JS_shield_info_obj.insert("band",dev_info.shield_arry[i].pa_info[j].band);
                            JS_shield_info_obj.insert("switch",dev_info.shield_arry[i].pa_info[j].swtich_stat);
                            JS_shield_info_obj.insert("status", stat);
                            JS_shield_info_obj.insert("temperature", dev_info.shield_arry[i].pa_info[j].temp);
                            JS_shield_info_obj.insert("voltage", dev_info.shield_arry[i].pa_info[j].volt);
                            JS_shield_info_obj.insert("current", dev_info.shield_arry[i].pa_info[j].current);
                            JS_shield_info_obj.insert("vswr", dev_info.shield_arry[i].pa_info[j].swr);
                            JS_shield_info_obj.insert("positivePower", dev_info.shield_arry[i].pa_info[j].f_power);
                            JS_shield_info_obj.insert("reversePower", dev_info.shield_arry[i].pa_info[j].b_power);
                            qDebug()<<tr(">>>>Command version V1.0 Current PA Param: CH_ID = %1, CH_Freq = %2, CH_Swtich = %3, PA_ID = %4, PA_Freq = %5, PA_Band = %6, Swtich = %7, Alarm_stat = %8 <<<<")
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_id)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_freq)
                                      .arg(dev_info.shield_arry[i].ch_info[j].ch_swtich)
                                      .arg(dev_info.shield_arry[i].ch_info[j].pa_id)
                                      .arg(dev_info.shield_arry[i].pa_info[j].freq)
                                      .arg(dev_info.shield_arry[i].pa_info[j].band)
                                      .arg(dev_info.shield_arry[i].pa_info[j].swtich_stat)
                                      .arg(dev_info.shield_arry[i].pa_info[j].alarm_stat)<<endl;
                            dev_info.shield_arry[i].ch_info[j].ch_id = 0;
                            dev_info.shield_arry[i].ch_info[j].ch_freq = 0;
                            dev_info.shield_arry[i].ch_info[j].ch_swtich = 0;
                            dev_info.shield_arry[i].ch_info[j].pa_id = 0;
                            dev_info.shield_arry[i].pa_info[i].freq = 0;
                            dev_info.shield_arry[i].pa_info[i].band = 0;
                            dev_info.shield_arry[i].pa_info[i].swtich_stat = 0;
                            dev_info.shield_arry[i].pa_info[i].alarm_stat = 0;

                            JS_shield_info_arr.append(JS_shield_info_obj);
                        }
                    }
                    JS_shield_obj.insert("shield_id", dev_info.shield_arry[i].shield_id);
                    JS_shield_obj.insert("shield_info", JS_shield_info_arr);
                    JS_freq_range_arr.append(JS_shield_obj);
                }
            }
        }
        else {
            qDebug()<<"-->>Device CommPro Version Error!<<--"<<endl;
        }
    #if 1
        /* 删除重复的屏蔽器信息 */
        tmp_jsoncnt = JS_freq_range_arr.size();
        //qDebug()<<"json_count = "<<tmp_jsoncnt;
        if(!JS_freq_range_arr.isEmpty()) {
           for(int i = 0; i < tmp_jsoncnt; i++) {
               if(this->js_shield_arry.at(i).isObject()) {
                   tmp1_jsonobj = JS_freq_range_arr.at(i).toObject();
                   tmp1_val = tmp1_jsonobj.value("shield_id");
                   for(int m = i+1; m < tmp_jsoncnt; m++) {
                       tmp2_jsonobj = JS_freq_range_arr.at(m).toObject();
                       tmp2_val = tmp2_jsonobj.value("shield_id");
                       //qDebug()<<"tmp1_val = "<<tmp1_val;
                       //qDebug()<<"tmp2_val = "<<tmp2_val;
                       if(tmp1_val == tmp2_val) {
                           JS_freq_range_arr.removeAt(i);
                           i--;
                           tmp_jsoncnt = JS_freq_range_arr.size();
                       }
                   }
               }
               //qDebug()<<jsonArray.at(i);
           }
        }
    #endif

    //删除信息空的屏蔽器信息
    #if 1
        if(!JS_freq_range_arr.isEmpty()) {
            for(int n = 0; n < this->js_shield_arry.size(); n++) {
                if(JS_freq_range_arr.at(n).isNull())
                {
                    JS_freq_range_arr.removeAt(n);
                    n--;
                }
            }
        }
    #endif
    }
    mutex_dev.lock();
    //emit this->get_gps_info();
    qDebug()<<"add_sendMsg mutex_dev lock done!";
    //qDebug()<<"shield count = "<<JS_freq_range_arr.size()/2;

    //添加子设备信息   2020-4-6
    QJsonArray local_subdev_arr;
    subdev_num = wavedetec_num+clouddeck_num;
    if(wavedetec_num > 0) {
        qDebug()<<tr(">>>Current Wavedetect Divece Count = %1").arg(wavedetec_num)<<endl;
        QJsonObject subdev_type;
        QJsonObject tmp_obj;
        if(wavedetec_stat == 2) {
            subdev_type.insert("type", 14);
            tmp_obj.insert("id", wavedetec_dev->localdevid);
            tmp_obj.insert("switch", wavedetec_dev->online_stat);
            tmp_obj.insert("stat", wavedetec_dev->md_dev_stat);
            subdev_type.insert("info", tmp_obj);
            qDebug()<<tr(">>>TCP Wavedetec:Insert Subdev Information Done!")<<endl;
        }else if(wavedetec_stat == 1) {
            subdev_type.insert("type", 14);
            tmp_obj.insert("id", http_cli->local_devid);
            tmp_obj.insert("switch", http_cli->online_stat);
            tmp_obj.insert("stat", http_cli->work_stat);
            subdev_type.insert("info", tmp_obj);
            qDebug()<<tr(">>>HTTP Wavedetec:Insert Subdev Information Done!")<<endl;
        }else if(wavedetec_stat == 3) {
            subdev_type.insert("type", 14);
            QString id_str = tr("%1").arg(zmq_wavedetec_dev->dev_repinfo.dev_id);
            tmp_obj.insert("id", id_str);
            tmp_obj.insert("switch", zmq_wavedetec_dev->online_stat);
            bool stat_flag;
            if(zmq_wavedetec_dev->dev_repinfo.dev_stat == "normal")
                stat_flag = true;
            else
                stat_flag = false;
            tmp_obj.insert("stat", stat_flag);
            subdev_type.insert("info", tmp_obj);
            qDebug()<<tr(">>>ZMQ Wavedetec:Insert Subdev Information Done!")<<endl;
        }
        local_subdev_arr.append(subdev_type);
        qDebug()<<tr(">>>local_subdev:")<<local_subdev_arr<<endl;
    }
    //添加云哨信息
    if(cloudwhistle_num > 0) {
        QJsonObject subdev_type;
        QJsonObject tmp_obj;
        if(cloud_whistle->online_stat == true && !cloud_whistle->localdevid.isEmpty()) {
            subdev_type.insert("type", 13);
            tmp_obj.insert("id", cloud_whistle->localdevid);
            tmp_obj.insert("switch", cloud_whistle->online_stat);
            tmp_obj.insert("stat", cloud_whistle->md_dev_stat);
            subdev_type.insert("info", tmp_obj);
            qDebug()<<tr(">>>Cloud whistle:Insert Subdev Information Done!")<<endl;
        }
        local_subdev_arr.append(subdev_type);
        qDebug()<<tr(">>>local_subdev:")<<local_subdev_arr<<endl;
    }
    //
    if(radar_num > 0) {
        QJsonObject subdev_type;
        QJsonObject tmp_obj;
        if(radar_stat == 1) {
            subdev_type.insert("type", 15);
            tmp_obj.insert("id", ("RD" + m_radar->md_dev_model + QString::number(m_radar->md_dev_id)));
            tmp_obj.insert("switch", m_radar->online_stat);
            tmp_obj.insert("stat", m_radar->online_stat);
            subdev_type.insert("info", tmp_obj);
            qDebug()<<tr(">>>Udp Radar:Insert Subdev Information Done!")<<endl;
        }
        local_subdev_arr.append(subdev_type);
        qDebug()<<tr(">>>local_subdev:")<<local_subdev_arr<<endl;
    }
    qint16 pit_angle = 0;   //
    qint16 hor_angle = 0;
    if(dev_info.angle_sw) {
        pit_angle = dev_info.pit_compangle;   //补偿角度     2020-6-1
        hor_angle = dev_info.hor_compangle;   //补偿角度     2020-6-1
        qDebug()<<tr(">>>设备补偿水平角:")<<dev_info.hor_compangle<<tr(" 俯仰角:")<<dev_info.pit_compangle<<endl;        
    }
	if(clouddeck_num) {
        pit_angle += cloud_deck->pit_angle;    //云台俯仰角+补偿角度 2020-6-1
        hor_angle += cloud_deck->hor_angle; 
		qDebug()<<tr(">>>云台设备水平角:")<<cloud_deck->hor_angle<<tr(" 俯仰角:")<<cloud_deck->pit_angle<<endl;		//指南针+云台旋转角度     2020-6-1
    }
    if(dev_info.att_info.att_stat) {
        hor_angle += dev_info.att_info.dir_angle;
        pit_angle += dev_info.att_info.pit_angle;
        qDebug()<<tr(">>>指南针水平角:%1, 仰角:%2").arg(QString::number(dev_info.att_info.dir_angle)).arg(QString::number(dev_info.att_info.pit_angle))<<endl;
    }
    if(hor_angle >= 360) {
        hor_angle %= 360;
    } else if(hor_angle < 0) {
        hor_angle %= 360;
        hor_angle += 360;
    } else {
        ;
    }
    qDebug()<<tr(">>>发送水平角:")<<hor_angle<<tr("发送俯仰角:")<<pit_angle<<endl;
    this->JS_dev_info_obj.insert("directAngle", 0);  //
    //this->JS_dev_info_obj.insert("frequencyRange", js_shield_arry);         //屏蔽器信息   19-9-12
    this->JS_dev_info_obj.insert("frequencyRange", JS_freq_range_arr);
    this->JS_dev_info_obj.insert("voltage", dev_info.batt_info.work_volt);           //电池电压0.1倍值
    this->JS_dev_info_obj.insert("current", dev_info.batt_info.work_current);        //电池电流0.1倍值
    this->JS_dev_info_obj.insert("batt_temp", dev_info.batt_info.temp);              //电池温度
    this->JS_dev_info_obj.insert("surplus_cap", dev_info.batt_info.surplus_cap);     //电池余量
    this->JS_dev_info_obj.insert("direct", hor_angle);          //指南针+云台旋转角度+补偿角度     2020-4-7
    this->JS_dev_info_obj.insert("pitchAngle", pit_angle);      //云台俯仰角+补偿角度             2020-4-7
    this->JS_dev_info_obj.insert("workRadius", dev_info.workradius);        //工作半径
    this->JS_dev_info_obj.insert("gps_Inf", JS_gps_info_obj);               //添加gps
    this->JS_dev_info_obj.insert("status", 1);      //设备状态--有无均可
    //this->JS_dev_info_obj.insert("type", 1);      //设备类型
    this->JS_dev_info_obj.insert("type", dev_info.dev_type);      //设备类型
    this->JS_dev_info_obj.insert("group_num", this->dev_info.group_num);    //屏蔽器数量
    //this->JS_dev_info_obj.insert("monitorId", "123456789");     //主设备ID
    this->JS_dev_info_obj.insert("monitorId", this->dev_info.dev_id);     //主设备ID

    this->JS_send_msg_obj.insert("aviation_Inf", this->JS_avia_info_arry);  //航空信息
    this->JS_send_msg_obj.insert("obj", JS_dev_info_obj);    //屏蔽设备信息
    this->JS_send_msg_obj.insert("local_subdev", local_subdev_arr);     //本地子设备
    this->JS_send_msg_obj.insert("subdev_num", subdev_num);     //子设备数量
    this->JS_send_msg_obj.insert("operation", "syncinfo");   //消息头
    //this->JS_send_msg_obj.insert("operation", "test");     //消息头
    //qDebug()<<JS_freq_range_arr<<endl;
//清空屏蔽器数据信息
#if 1
    if(!JS_freq_range_arr.isEmpty())
    {
        for(m = 0; m < JS_freq_range_arr.size(); m++)
        {
            //JS_freq_range_arr.removeFirst();
            JS_freq_range_arr.removeAt(m);     //2020-4-6
        }
    }
#endif
#if 1   //清除子设备信息   2020-4-7
    if(!local_subdev_arr.isEmpty()) {
        for(m = 0; m < JS_freq_range_arr.size(); m++)
        {
            //JS_freq_range_arr.removeFirst();
            local_subdev_arr.removeAt(m);     //2020-4-7
        }
    }
#endif  
    //添加本地侦测数据  2020-5-25
//    if(wavedetec_num) {
//        switch (wavedetec_stat) {
//        case 0:
//            break;
//        case 1:
//            break;
//        case 2:
//            emit wavedetec_dev->add_allData();  //添加侦测数据    2020-4-3
//            qDebug()<<tr(">>>TCP Wavedetect: Add up data done!")<<endl;
//            break;
//        default:
//            break;
//        }
//    }
    socket_sendMsg();
    mutex_dev.unlock();
    qDebug()<<"add_sendMsg mutex_dev unlock done!";
    this->send_timer->start(2000);
}
//定时发送  --暂时无效
void Widget::timeout_send()
{
    this->send_timer->stop();
    disconnect(Dev_serial, SIGNAL(receive_data(QByteArray)), this, SLOT(on_receive_dev(QByteArray)));
    disconnect(Gps_serial, SIGNAL(receive_data(QByteArray)), this, SLOT(on_receive_gps(QByteArray)));
    emit this->sendMsg(this->send_msg);
    //emit serialsend("AT+AT\n");     //2020-4-22
    //qDebug()<<"Comm Port Send Data-->>"<<endl;
    this->send_timer->start(2000);
}
//发送上行消息--有效
void Widget::socket_sendMsg()
{
    //QTextDocument *document = Q_NULLPTR;
    //QTextBlock textBlock;
    //int i = 0;
    int send_obj_count;
    QString send_str;
    QString send_mdstr;
    QString md_send_str;
    QByteArray byte_arr;

    //QTime current_time = QTime::currentTime();

    byte_arr[0] = '{';
    byte_arr[1] = '}';

#if 1
    send_str = QString(QJsonDocument(this->JS_send_msg_obj).toJson());
    /* 2020-12-2 Sync raspi */
    if(dev_info.dev_type > 12 && dev_info.dev_type < 19) {
        switch (dev_info.dev_type) {
        case 13:
            send_mdstr = QString(QJsonDocument(this->cloud_whistle->md_upMQ_info).toJson());
            break;
        case 14:
            if(wavedetec_stat == 2)
                send_mdstr = QString(QJsonDocument(this->wavedetec_dev->md_upMQ_info).toJson());
            else if(wavedetec_stat == 1) {
                send_mdstr = QString(QJsonDocument(this->http_cli->json_MQ).toJson());
            }
            else if(wavedetec_stat == 3) {  //2021-1-7
                send_mdstr = QString(QJsonDocument(this->zmq_wavedetec_dev->js_obj).toJson());
            }
            else
                qDebug()<<tr("Unknown type of microwave detection device!")<<endl;
            break;
        case 15:
            break;
        default:
            break;
        }
    }
    /* 2020-12-2 Sync raspi */
    //2020-12-2 Add annotation
//    if(cloudwhistle_num)
//        send_mdstr = QString(QJsonDocument(this->cloud_whistle->md_upMQ_info).toJson());
#if 0
    //微波侦测和云哨设备数据
    if(wavedetec_num && !wavedetec_dev->md_upMQ_info.isEmpty()) {
        md_send_str = QString(QJsonDocument(this->wavedetec_dev->md_upMQ_info).toJson());
        emit this->wavedetec_dev->clear_allData();
        qDebug()<<tr(">>>>Websocket Interface Send Wave Detecte UAV Information Data: %1 ").arg(md_send_str)<<endl;
    } else if(wavedetec_num && !http_cli->json_MQ.isEmpty()) {
        //md_send_str = QString(QJsonDocument(this->http_cli->json_MQ).toJson());
        //emit this->http_cli->clear_allData();
    } else if(cloudwhistle_num &&!cloud_whistle->md_upMQ_info.isEmpty()) {
        md_send_str = QString(QJsonDocument(this->cloud_whistle->md_upMQ_info).toJson());
        emit this->cloud_whistle->clear_allData();
        qDebug()<<tr(">>>>Websocket Interface Send Cloud Whistle UAV Information Data: %1 ").arg(md_send_str)<<endl;
    } else {
        md_send_str = "";
    }
#endif
    if(!send_str.isNull())
        emit this->sendMsg(send_str);
    if(clouddeck_num > 0) {
        cloud_deck->query_Comm_horAngle(cloud_deck->dev_addr);         //查询云台当前水平角度    2020-4-5
    }
    //sleep_ms(200);
    //cloud_deck->query_Comm_pitAngle(cloud_deck->dev_addr);         //查询云台当前俯仰角度    2020-4-5

    //打印发送服务器数据
    //qDebug()<<"send_str = "<<send_str;
    qDebug()<<"send_str len: "<<send_str.length()<<" Bytes."<<endl;
    qDebug()<<"md_send_str len:"<<md_send_str.length()<<" Bytes."<<endl;
    //2020-8-10 清除功放计数
    for(quint8 i = 0; i < 16; i++) {
        if(dev_info.shield_arry[i].ch_count != 0)
            dev_info.shield_arry[i].ch_count = 0;
    }
#endif
    /* 清除对象键值对*/
    if(!JS_send_msg_obj.isEmpty())
    {
        QStringList key_list = JS_send_msg_obj.keys();
        send_obj_count = JS_send_msg_obj.size();
        for(qint16 m = 0; m < send_obj_count; m++)
        {
            JS_send_msg_obj.remove(key_list[m]);
        }
    }
    //disp_Dev_Info();    //显示设备信息
    get_stat_Fun();
    //test_SendShield_Cmd();
}
//获取状态
void Widget::get_stat_Fun()
{
    switch (dev_index) {
    case 1:
        if(cloud_whistle->online_stat && cloud_whistle->tcp_cli->m_online_stat) {
            stat_info.connDevStat = true;
            stat_info.devID = dev_info.dev_id;
        }else {
            stat_info.connDevStat = false;
            stat_info.devID = "NULL";
        }
        break;
    case 2:
        if(wavedetec_stat == 1) {
            if(http_cli->online_stat) {
                stat_info.connDevStat = true;
                stat_info.devID = dev_info.dev_id;
            }else {
                stat_info.connDevStat = false;
                stat_info.devID = "NULL";
            }
        }else if(wavedetec_stat == 2) {
            if(wavedetec_dev->online_stat) {
                stat_info.connDevStat = true;
                stat_info.devID = dev_info.dev_id;
            }else {
                stat_info.connDevStat = false;
                stat_info.devID = "NULL";
            }
        }else if(wavedetec_stat == 3) {
            if(zmq_wavedetec_dev->m_zeroMqcli->conn_stat) {
                stat_info.connDevStat = true;
                stat_info.devID = dev_info.dev_id;
            }else {
                stat_info.connDevStat = false;
                stat_info.devID = "NULL";
            }
        }else {
            stat_info.connDevStat = false;
            stat_info.devID = "NULL";
        }

        break;
    case 3:

        break;
    case 4:
        if(dev_info.stat) {
            stat_info.connDevStat = true;
            stat_info.devID = dev_info.dev_id;
        }else {
            stat_info.connDevStat = false;
            stat_info.devID = "NULL";
        }
        break;
    case 5:
        if(dev_info.stat) {
            stat_info.connDevStat = true;
            stat_info.devID = dev_info.dev_id;
        }else {
            stat_info.connDevStat = false;
            stat_info.devID = "NULL";
        }
        break;
    case 6:
        if(dev_info.stat) {
            stat_info.connDevStat = true;
            stat_info.devID = dev_info.dev_id;
        }else {
            stat_info.connDevStat = false;
            stat_info.devID = "NULL";
        }
        break;
    default:
        break;
    }
    //连接服务器状态--设备接口+无人机数据接口
    if(dev_info.dev_type < 16 && dev_info.dev_type > 12) {
        if(w_socket->connectStatus && md_socket->connectStatus) {
            stat_info.connServerStat = true;
        }else {
            stat_info.connServerStat = false;
        }
    }else {
        if(w_socket->connectStatus) {
            stat_info.connServerStat = true;
        }else {
            stat_info.connServerStat = false;
        }
    }

}
//串口延时(超时接收)--未启用
void Widget::comm_dev_delay()
{
    QString rev_str;

    time_out_dev.start(80);
    recv_buff.clear();
    recv_buff.append(comm_dev.readAll());
    //recv_buff.append(comm_dev.readAll().toHex());
    //rev_str.append(comm_dev.readAll());
    //rev_str.append(comm_dev.readAll());
    //StringToHex(rev_str, recv_buff);
    //qDebug()<<"receive Qbytearray:"<<recv_buff;
    //qDebug()<<"receive QString:"<<rev_str;
    //qDebug()<<"receive data len = "<<recv_buff.length();

}
//设备串口接收读取(超时接收)--未启用
void Widget::Serial_dev_Read()
{
    QString tmp_str;
    QString comp_str;
    QDateTime date_time = QDateTime::currentDateTime();
    QTime current_time = QTime::currentTime();
    time_out_dev.stop();
    //this->tedit_dev->clear();
    if(recv_buff.length() )
    {
        //this->tedit_dev->setText(current_time.toString());
        //comp_str = "["+current_time.toString()+"]:"+recv_buff+"\n";
        comp_str.append("[");
        comp_str.append(current_time.toString());
        comp_str.append("]:");
        comp_str.append(recv_buff.toHex());
        tmp_str = QString::number(recv_count_dev++);
        //qDebug()<<comp_str;
        autoScrollTextEdit(tedit_dev, comp_str, tmp_str);
    }
    //Dev_prot_anal(recv_buff, recv_buff.length());


/*
    QScrollBar *scrollbar = this->tedit_dev->verticalScrollBar();
    if(scrollbar)
        scrollbar->setSliderPosition(scrollbar->maximum());
*/
/*
    tmp_str = this->tedit_dev->toPlainText();
    int len = tmp_str.count();
    text_alllen += len;

    if(text_alllen > MAX_DISP_COUNT)
    {
        this->tedit_dev->clear();
        text_alllen = 0;
    }
*/
}
//自动显示文本框最后一行
void Widget::autoScrollTextEdit(QTextEdit *textEdit,QString &text,QString &add_text)
{
    //添加行字符并刷新编辑框
    add_text += "\n";
    //text+=add_text+"\n";
    //textEdit->setText(text);
    textEdit->append(text);
    //保持编辑器在光标最后一行
    QTextCursor cursor=textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    textEdit->setTextCursor(cursor);
    //如果超过一定行数清空一下
    if(textEdit->document()->lineCount()>1024)
    {
        text.clear();
        textEdit->clear();
    }
}
//线程处理设备串口接收 -- 读取设备CAN数据   2020-5-1
void Widget::on_receive_dev(QByteArray tmpdata)
{
    QString tmp_str;
    QString comp_str;
    QDateTime date_time = QDateTime::currentDateTime();
    QTime current_time = QTime::currentTime();

    qDebug()<<tr(">>>>Receive Device Send Data:")<<tmpdata.toHex()<<"<<<<---"<<date_time.toString()<<endl;
    if(tmpdata.size() != 17) {
        qDebug()<<tr(">>>>Receive Device Send Data Length Error!")<<endl;
        return;
    }
    if(tmpdata.at(0) == 0x41 && tmpdata.at(1) == 0x54 && tmpdata.at(15) == 0x0d && tmpdata.at(16) == 0x0a) {
        QByteArray Can_data;    //CAN数据
        quint32 Extid;          //扩展ID
        quint8 Can_data_len;    //CAN数据长度
        quint8 shield_id;       //屏蔽器ID
        quint8 channel_id;      //信道ID
        //quint8 pa_id;           //功放ID
        quint8 info_num;        //信息序号
        QList<quint8>::iterator ite_sh;    //屏蔽器迭代器
        QList<quint8>::iterator ite_ch;     //信道迭代器
        Extid = quint32((tmpdata.at(2)<<24)+(tmpdata.at(3)<<16)+(tmpdata.at(4)<<8)+tmpdata.at(5));
        Extid = (Extid>>3);
        if((Extid>>24) == 0x01) {            
            shield_id = Extid>>16&0xFF;
            channel_id = Extid>>8&0xFF;
            //pa_id = channel_id;
            info_num = Extid&0xFF;
            Can_data_len = quint8(tmpdata.at(6));       //数据长度
            if(shield_id < 32) {    //屏蔽器ID最大32
                //vector_Shield[shield_id].shield_ID = shield_id;
                bool yorn;
                //统计屏蔽器数量--2020-5-1
                for(quint8 i = 0; i < 16; i++) {
                    if(dev_info.shield_arry[i].shield_id != 0xF0)
                        qDebug()<<tr(">>>Device Information Shield arry[%1] Shield ID = %2 <<<").arg(i).arg(dev_info.shield_arry[i].shield_id)<<endl;
                    //查询ID是否存在
                    if(dev_info.shield_arry[i].shield_id != shield_id) {
                        yorn = false;
                    } else {
                        yorn = true;
                        break;
                    }
                }
                if(!yorn) {
                    dev_info.shield_arry[shield_id].shield_id = shield_id;
                    dev_info.shield_IDlist.append(shield_id);
                    dev_info.group_num++;

                }
                //遍历屏蔽器迭代器
                for(ite_sh = dev_info.shield_IDlist.begin(); ite_sh != dev_info.shield_IDlist.end(); ++ite_sh) {
                    if(*ite_sh != shield_id) {
                        dev_info.shield_IDlist.append(shield_id);

                    } else {
                        ;
                    }
                }

            } else {
                //接收到无效的ID号
                qDebug()<<tr("Recieve Invalid Device ID Number!<<<")<<endl;
                return;
            }
            //信道和功放信息
            if(dev_info.comm_pro_ver == "V1.0" && channel_id < 0xF0) {
                bool yorn = true;
                for(quint8 i = 0; i < 32; i++) {
                    if((dev_info.shield_arry[shield_id].ch_info[i].ch_id != channel_id)) {
                        yorn = false;

                    } else {
                        yorn = true;
                        break;
                    }
                }
                if(!yorn) {
                    //刷新PAID
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id = channel_id;
                    dev_info.shield_arry[shield_id].ch_count++;
                }
                switch (info_num) {
                case 0:
                    //功放状态信息1
                    dev_info.shield_arry[shield_id].pa_info[channel_id].alarm_stat = tmpdata.at(7);
                    dev_info.shield_arry[shield_id].pa_info[channel_id].swtich_stat = quint8(tmpdata.at(8));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].f_power = quint8(tmpdata.at(9));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].b_power = quint8(tmpdata.at(10));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].temp = quint8(tmpdata.at(11));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].volt = quint8(tmpdata.at(12));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].current = quint8(tmpdata.at(13));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].swr = quint8(tmpdata.at(14));
                    qDebug()<<tr(">>>>Version1.0 Tmpdata Data: alarm_stat = %1, swtich_stat = %2, volt = %3, current = %4 <<<<")
                              .arg(QString::number(tmpdata.at(7))).arg(QString::number(tmpdata.at(8)))
                              .arg(QString::number(tmpdata.at(12))).arg(QString::number(tmpdata.at(13)))<<endl;
                    dev_info.stat = true;
                    break;
                case 1:
                    //功放状态信息2
                    dev_info.shield_arry[shield_id].pa_info[channel_id].freq = quint16((tmpdata.at(7)<<8)+tmpdata.at(8));

                    dev_info.shield_arry[shield_id].pa_info[channel_id].band = quint16((tmpdata.at(9)<<8)+tmpdata.at(10));
                    qDebug()<<tr(">>>>>Version1.0 Tmpdata Data: Freq = %1, Band = %2 <<<<")
                              .arg(QString::number((tmpdata.at(7)<<8)+tmpdata.at(8))).arg(QString::number((tmpdata.at(9)<<8)+tmpdata.at(10)))<<endl;
                    dev_info.stat = true;
                    break;
                case 2:
                    //工作状态
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_freq = quint16((tmpdata.at(7)<<8)+tmpdata.at(8));
                    dev_info.shield_arry[shield_id].ch_info[channel_id].pa_id = quint8(tmpdata.at(9));
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_swtich = quint8(tmpdata.at(10));
                    qDebug()<<tr(">>>>>Version1.0 Tmpdata Data: CH_Freq = %1, Pa_id = %2, CH_Swtich = %3 <<<<")
                              .arg(QString::number((tmpdata.at(7)<<8)+tmpdata.at(8))).arg(QString::number(tmpdata.at(9))).arg(QString::number(tmpdata.at(10)))<<endl;
                    break;
                default:
                    break;
                }
            } else if(dev_info.comm_pro_ver == "V2.0" && channel_id < 0xF0) {
                bool yorn = true;
                for(quint8 i = 0; i < 32; i++) {
                    if((dev_info.shield_arry[shield_id].ch_info[i].ch_id != channel_id)) {
                        yorn = false;
                    } else {
                        yorn = true;
                        break;
                    }
                }
                if(!yorn) {
                    //刷新PAID
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id = channel_id;
                    dev_info.shield_arry[shield_id].ch_count++;
                }
                switch (info_num) { //信息序号
                case 0:
                    //信道信息
                    if(channel_id < 32) {
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_swtich = quint8(tmpdata.at(7));
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_current = quint16(((tmpdata.at(8)<<8)+tmpdata.at(9))/1000);  //mA->A
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_power_gear = quint8(tmpdata.at(10));
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_freq = quint16((tmpdata.at(12)<<8)+tmpdata.at(13));
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_swr = tmpdata.at(14)/10;    //Unit = 0.1
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id = channel_id;
                    }
                    qDebug()<<tr(">>>>Version2.0 Channel Tmpdata Data: Switch = %1, Current = %2 mA, Power_gear = %3, Freq = %4 SWR = %5 Channel_id = %6 <<<<")
                              .arg(QString::number(tmpdata.at(7))).arg(QString::number((tmpdata.at(8)<<8)+tmpdata.at(9)))
                              .arg(QString::number(tmpdata.at(10))).arg(QString::number((tmpdata.at(12)<<8)+tmpdata.at(13)))
                              .arg(QString::number(tmpdata.at(14))).arg(QString::number(channel_id))<<endl;
#if 0
                    //遍历信道迭代器
                    for(ite_ch = dev_info.shield_arry[shield_id].ch_list.begin(); ite_ch != dev_info.shield_arry[shield_id].ch_list.end(); ++ite_ch) {
                        if(*ite_ch != dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id) {
                            dev_info.shield_arry[shield_id].ch_list.append(dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id);
                        }
                    }
#endif
                    dev_info.stat = true;
                    break;
                case 1:
                    //屏蔽器信息
                    dev_info.shield_arry[shield_id].shield_sw_stat = quint8(tmpdata.at(7));  //开关状态
                    if(quint8(tmpdata.at(8)) != 0) {
                        dev_info.shield_arry[shield_id].shield_temp = quint8(tmpdata.at(8));     //温度
                    }
                    dev_info.shield_arry[shield_id].shield_mode = quint8(tmpdata.at(9));     //防御模式 1-躯离 2-迫降
                    qDebug()<<tr(">>>>Version2.0 Shield Tmpdata Data: Switch = %1, Temp = %2, Mode = %3 <<<<")
                              .arg(QString::number(tmpdata.at(7))).arg(QString::number(tmpdata.at(8))).arg(QString::number(tmpdata.at(9)))<<endl;
                    dev_info.stat = true;
                    break;
                case 2:
                    //屏蔽器信息(模式配置）
                    dev_info.shield_arry[shield_id].shield_cfg_exple = quint32((tmpdata.at(7)<<24)+(tmpdata.at(8)<<16)+(tmpdata.at(9)<<8)+tmpdata.at(10));
                    dev_info.shield_arry[shield_id].shield_cfg_land = quint32((tmpdata.at(11)<<24)+(tmpdata.at(12)<<16)+(tmpdata.at(13)<<8)+tmpdata.at(14));
                    break;
                default: 
                    break;
                }

            } else {

                qDebug()<<tr("---Device Communication protocol version Error!---")<<endl;
            }

        } else if((Extid>>24) == 0x02) {
            //磁罗盘信息
            dev_info.att_info.dir_angle = quint16((tmpdata.at(7)<<8)+tmpdata.at(8));
            dev_info.att_info.pit_angle = qint16((tmpdata.at(9)<<8)+tmpdata.at(10));
            dev_info.att_info.roll_angle = qint16((tmpdata.at(11)<<8)+tmpdata.at(12));
            dev_info.att_info.att_stat = true;
            qDebug()<<tr(">>>>Receive Angle Module Data: DIR_Angle = %1, PIT_Angle = %2, Roll_Angle = %3")
                      .arg(dev_info.att_info.dir_angle).arg(dev_info.att_info.pit_angle).arg(dev_info.att_info.roll_angle)<<endl;
        } else if((Extid>>24) == 0x03) {
            //电池箱信息
            dev_info.batt_info.work_volt = quint8(tmpdata.at(7));
            dev_info.batt_info.work_current = quint8(tmpdata.at(8));
            dev_info.batt_info.temp = quint8(tmpdata.at(9));
            dev_info.batt_info.surplus_cap = quint8(tmpdata.at(10));
            dev_info.batt_info.trouble_code = quint8(tmpdata.at(11));
            dev_info.stat = true;
        } else if((Extid>>24) == 0x04) {
            dev_info.stat = true;
        } else {
            qDebug()<<tr(">>>Receive Invalid CAN Data Information!<<<")<<endl;
            dev_info.stat = false;
            return;
            //其它数据信息
        }

#if 0
        Can_data.append(tmpdata.at(7));     //on/off
        Can_data.append(tmpdata.at(8));     //电流高字节
        Can_data.append(tmpdata.at(9));     //电流低字节
        Can_data.append(tmpdata.at(10));    //档位
        Can_data.append(tmpdata.at(11));    //ADC-R
        Can_data.append(tmpdata.at(12));    //频率高字节
        Can_data.append(tmpdata.at(13));    //频率低字节
        Can_data.append(tmpdata.at(14));    //Null
        qDebug()<<tr(">>>>Extid = ")<<Extid<<endl;
        qDebug()<<tr(">>>>Can Data Extid:")<<QString::number(Extid,16)<<"<<<<"<<endl;
        qDebug()<<tr(">>>>Can Data Len:")<<QString::number(Can_data_len,10)<<"<<<<"<<endl;
        qDebug()<<tr(">>>>Can Data:")<<Can_data.toHex()<<"<<<<"<<endl;
        //遍历打印屏蔽器数量 2020-4-30 暂时无效
        qDebug()<<tr(">>>>Current Shield Count:")<<dev_info.shield_list.size()<<tr("<<<<")<<endl;
        for(quint8 i = 0; i < dev_info.shield_list.size(); i++) {
            qDebug()<<tr("Shield ID = ")<<dev_info.shield_list.at(i)<<endl;
            qDebug()<<tr(">>>>Current Channel Count in the Shield:")<<dev_info.shield_arry[dev_info.shield_list.at(i)].ch_list.size()<<endl;
            for(quint8 j = 0; j < dev_info.shield_arry[dev_info.shield_list.at(i)].ch_list.size(); j++) {
                qDebug()<<tr("Channel ID = ")<<dev_info.shield_arry[dev_info.shield_list.at(i)].ch_info[j].ch_id;
            }

        }
#endif
    }else {
        qDebug()<<tr(">>>>Receive Device Send Data Invalid!")<<endl;
        return;
    }
#if 1
    //显示设备发送消息  2019-9-5
    if(tmpdata.length() != 0)
    {
        comp_str.append("[");
        comp_str.append(date_time.date().toString());
        comp_str.append(current_time.toString());
        comp_str.append("]:");
        comp_str.append(tmpdata.toHex());
        tmp_str = QString::number(recv_count_dev++);
        qDebug()<<comp_str;
        autoScrollTextEdit(tedit_dev, comp_str, tmp_str);     //自动文本框最后一行
    }
#endif

}
//线程处理GPS串口接收
void Widget::on_receive_gps(QByteArray tmpdata)
{
    recv_buff_gps.append(tmpdata);
}
void Widget::on_receive_avi(QByteArray tmpdata)
{
    this->tedit_dev->append(tmpdata);

}
void Widget::on_receive_test(QByteArray tmpdata)
{
    qDebug()<<tmpdata;
}
void Widget::on_rd_btn_clicked(bool checked)
{
    if(checked) {
        commbox_paid->setVisible(true);
        commbox_shieldid->setVisible(true);

        pbutt_1->setVisible(true);
        pbutt_2->setVisible(true);

        label_shieldid->setVisible(true);
        label_paid->setVisible(true);
    }else {
        commbox_paid->setVisible(false);
        commbox_shieldid->setVisible(false);

        pbutt_1->setVisible(false);
        pbutt_2->setVisible(false);

        label_shieldid->setVisible(false);
        label_paid->setVisible(false);
    }

}
//gps串口读取
void Widget::comm_gps_readyRead()
{

    QString str_qx;
    this->comm_gps_rxbuf = comm_gps.readAll();
    this->tedit_dev->setText(str_qx.append(this->comm_gps_rxbuf));
    //this->tedit_dev->clear();
    this->tedit_dev->setText(comm_gps_rxbuf);
}
//设备串口读取
void Widget::comm_dev_readyRead()
{
    QString str_dev;
    comm_dev_rxbuf = this->comm_dev.readAll();
    this->tedit_dev->setText(str_dev.append(this->comm_dev_rxbuf));
    //this->tedit_dev->clear();
    //this->tedit_dev->setText(comm_dev_rxbuf.toHex());
}
//设备端口数据解析--19年树莓派方式解析
int Widget::Dev_prot_anal(QByteArray byte_arry, int len)
{
    quint8 check_sum = 0;
    quint8 i = 0;
    quint16 tmp_num;
    Pa_Info_st tmp_pa;
    quint8 id_shield;
    uint16_t arry_len;
    QString tmp_str;
    int count_pa;
    qint8 stat;
    QJsonArray pa_arry;
    QJsonArray shield_arr_t;
    QJsonObject tmp1_jsonobj,tmp2_jsonobj;
    QJsonObject shield_obj;
    QJsonArray shield_arry;
    QJsonValue tmp1_val, tmp2_val;
    QString tmp1_str, tmp2_str;
    QStringList shield_list;


    len += 1;
    //tmp_str = QString::number(byte_arry);
    //qDebug()<<"at(2) = "<<byte_arry.at(2) <<""<<"at(3) = "<<byte_arry.at(3);
    qDebug()<<tr(">>>>Receive Device Send Data:")<<byte_arry.toHex()<<"<<<<"<<endl;
#if 1   //树莓派方式解析中控板数据
    arry_len = (byte_arry.at(2)*256+byte_arry.at(3))&0xFFFF;
    arry_len += 7;
    //qDebug()<<"arry len = "<<arry_len<<endl;
    if((byte_arry.at(0) != '\xfe') && (byte_arry.at(1) != '\xae'))
    {
        qDebug()<<"Recevice frame start flag error!";
        //return -1;
    }
    if((byte_arry.at(arry_len-3) != '\xef') && (byte_arry.at(arry_len-2) != '\xea'))
    {
        qDebug()<<"Recevice frame end flag error!";
        //return -2;
    }
    for(int i = 0; i < arry_len-1; i++)
    {
        check_sum += byte_arry.at(i);
    }
    if(byte_arry.at(arry_len-1) != check_sum&&0xFF)
    {
        qDebug()<<"Recevice frame check sum error!";
        qDebug()<<"byte_arr check_sum = "<<QString::number(byte_arry.at(arry_len-1),10)<<"local check_sum = "<<QString::number(check_sum,10);
        //return -3;
    }
    switch (byte_arry.at(4)) {
        case 0x1:
            //屏蔽器信息
            //tmp_num = byte_arry.at(5);
            count_pa = byte_arry.at(2)*256+byte_arry.at(3)-1;
            qDebug()<<"Receive device send data len = "<<count_pa<<endl;
            count_pa = count_pa/20;
            id_shield = byte_arry.at(5);    //屏蔽器ID
            qDebug()<<"PA count = "<<count_pa<<endl;
            mutex_dev.lock();          
            for(i = 0; i < count_pa; i++)
            {
                Pa_Info_st tmp_pa;
                tmp_pa.pa_id = byte_arry.at(i*20+6);
                /*告警状态*/
                tmp_num = byte_arry.at(i*20+7);
                if((tmp_num&0x1) == 0x1)
                    stat = -1;  //过温
                else if((tmp_num&0x2) == 0x2)
                    stat = -2;  //过功率
                else if((tmp_num&0x4) ==0x4)
                    stat = -3;  //驻波告警
                else if((tmp_num&0x8) == 0x8)
                    stat = -4;  //输出欠功率
                else if((tmp_num&0x10) == 0x10)
                    stat = -5;  //过流
                else if((tmp_num&0x20) == 0x20)
                    stat = -6;  //欠压告警
                else if((tmp_num&0x40) == 0x40)
                    stat = -7;  //过压告警
                else if((tmp_num&0x60) == 0x60)
                    stat = -8;  //参数错误
                else if((tmp_num&0x80) == 0x80)
                    stat = -9;
                else
                    stat = 0;    //正常
                tmp_pa.alarm_stat = stat;
                //qDebug()<<"tmp_pa.alarm_stat = "<<tmp_pa.alarm_stat;
                 /*告警状态*/
                tmp_pa.pa_swtich = quint8(byte_arry.at(i*20+8));     //功放开关
                tmp_pa.f_power = quint8(byte_arry.at(i*20+9));       //正向功率
                tmp_pa.b_power = quint8(byte_arry.at(i*20+10));      //反向功率
                tmp_pa.temp = quint8(byte_arry.at(i*20+11));         //温度
                tmp_pa.volt = quint8(byte_arry.at(i*20+12));         //电压
                tmp_pa.current = quint8(byte_arry.at(i*20+13));      //电流
                tmp_pa.swr = quint8(byte_arry.at(i*20+14));          //驻波比
                //qDebug()<<"byte_arry["<<i*20+15<<"] ="<<byte_arry.at(i*20+15)*256<<"byte_arry["<<byte_arry.at(i*20+16)<<"] =";
                tmp_pa.freq = quint16(byte_arry.at(i*20+15)*256+byte_arry.at(i*20+16));
                tmp_pa.band = quint16(byte_arry.at(i*20+17)*256+byte_arry.at(i*20+18));
                tmp_pa.res = quint8(byte_arry.at(i*20+19));          //预留
            /*
                qDebug()<<"shield_id = "<<id_shield;
                qDebug()<<"pa_id = "<<tmp_pa.pa_id.toInt()<<" alarm_stat = "<<tmp_pa.alarm_stat.toInt();
                qDebug()<<" pa_switch = "<<tmp_pa.pa_switch.toInt()<<" pa_fpower = "<<tmp_pa.f_power.toInt()<<" pa_b_power = "<<tmp_pa.b_power.toInt();
                qDebug()<<" pa_temp = "<<tmp_pa.temp.toInt()<<" pa_volt = "<<tmp_pa.volt.toInt();
                qDebug()<<" pa_current = "<<tmp_pa.current.toInt()<<" pa_swr = "<<tmp_pa.swr.toInt();
                qDebug()<<" pa_freq = "<<tmp_pa.freq<<" pa_band = "<<tmp_pa.band.toInt();
            */

                this->JS_pa_info_obj.insert("temperature", tmp_pa.temp);
                this->JS_pa_info_obj.insert("reversePower", tmp_pa.b_power);
                this->JS_pa_info_obj.insert("positivePower", tmp_pa.f_power);
                this->JS_pa_info_obj.insert("vswr", tmp_pa.swr);
                this->JS_pa_info_obj.insert("current", tmp_pa.current);
                this->JS_pa_info_obj.insert("voltage", tmp_pa.volt);
                this->JS_pa_info_obj.insert("band", tmp_pa.band);
                this->JS_pa_info_obj.insert("frequency", tmp_pa.freq);
                this->JS_pa_info_obj.insert("switch", tmp_pa.pa_swtich);
                this->JS_pa_info_obj.insert("status", tmp_pa.alarm_stat);
                this->JS_pa_info_obj.insert("id", tmp_pa.pa_id);
                pa_arry.append(this->JS_pa_info_obj);
                //qDebug()<<"pa arry conut = "<<JS_pa_info_arry.size();
            }
        #if 0
            if(JS_pa_info_arry.size() > count_pa)
            {
                for(m = 0; m < JS_pa_info_arry.size()-count_pa; m++)
                {
                    //JS_pa_info_arry.removeAt(0);
                    JS_pa_info_arry.removeLast();
                    qDebug()<<"PA arry remove at "<<JS_pa_info_arry.size()-count_pa;
                }
            }
        #endif
        #if 0
            /* 删除重复的信息 */
            json_count = this->JS_pa_info_arry.size();
            qDebug()<<"json_count = "<<json_count;
            if(json_count > 1) {
               for(int i = 0; i < json_count; i++) {
                   if(this->JS_pa_info_arry.at(i).isObject()) {
                       tmp1_jsonobj = this->JS_pa_info_arry.at(i).toObject();
                       tmp1_val = tmp1_jsonobj.value("id");
                    /*
                       if(i == 0 && tmp1_val != 0)
                       {
                            JS_pa_info_arry.removeAt(i);
                            json_count = JS_pa_info_arry.size();
                            i--;
                            continue;
                       }
                    */
                       for(int m = i+1; m < json_count; m++) {
                           tmp2_jsonobj = this->JS_pa_info_arry.at(m).toObject();
                           tmp2_val = tmp2_jsonobj.value("id");
                           qDebug()<<"tmp1_val = "<<tmp1_val;
                           qDebug()<<"tmp2_val = "<<tmp2_val;
                           if(tmp1_val == tmp2_val) {
                               this->JS_pa_info_arry.removeAt(0);
                               //json_count = JS_pa_info_arry.size();
                           }
                       }
                   }
                   //qDebug()<<jsonArray.at(i);
               }
            }
        #endif
            shield_obj.insert("shield_info", pa_arry);
            shield_obj.insert("shield_id", id_shield);
            js_shield_arry.append(shield_obj);
            break;
        case 0x2:
            //雷达信息
            break;
        case 0x3:
            //电池箱信息
            dev_info.batt_info.work_volt = quint8(byte_arry.at(5));    //电池电压
            int temp_val;
            temp_val = byte_arry.at(6);     //电池电流
            if(temp_val > 0x7F) {
                temp_val = 0-(temp_val&0x7F);
            }
            dev_info.batt_info.work_current = quint8(temp_val);
            temp_val = byte_arry.at(7);     //电池温度
            if(temp_val > 0x7F) {
                temp_val = 0-(temp_val&0x7F);
            }
            dev_info.batt_info.temp = quint8(temp_val);
            dev_info.batt_info.surplus_cap = quint8(byte_arry.at(8));      //电池余量
            break;
        case 0x4:
            //指南针信息
            dev_info.att_info.dir_angle = quint16(byte_arry.at(5)*256+byte_arry.at(6));
            dev_info.att_info.att_stat = true;
            qDebug()<<"dir_angel = "<<dev_info.att_info.dir_angle;
            break;
        default:
            break;
    }
#endif
    mutex_dev.unlock();
    qDebug()<<"add device information done!";
    return 0;
}
//GPS信息解析
QString Widget::Gps_prot_anal(QByteArray byte_arry, int len)
{
    QString tmp_str;
	QString ret_str;
	int line = 0;

	tmp_str = QString(byte_arry);
	len += 1;
    if(len > 1024) {    //接收到超长的GPS信息
        qDebug()<<tr("GPS:Received super long GPS information!")<<endl;
        return record_gpsInfo;
    }
	if(true != tmp_str.contains("$GNGGA",Qt::CaseSensitive))
	{
        qDebug()<<tr("GPS:The specified location information was not found!")<<endl;
		return record_gpsInfo;
	}
	if(true != tmp_str.contains("$GNVTG", Qt::CaseSensitive)) {
		qDebug()<<tr("GPS:The specified speed information was not found!");
		return record_gpsInfo;
	}
	QStringList list = tmp_str.split("$");
	line = list.count();
    //qDebug()<<tr(">>>GPS:StringList Count = %1 <<<").arg(line)<<endl;
	for(uint8_t i = 0; i < line; i++)
	{
        if(!list[i].contains("GN")) {
			continue;
		}
        if(true == list[i].contains("GNVTG", Qt::CaseSensitive)) {
            //qDebug()<<tr(">>>GPS:StringList [%1] = %2 <<<").arg(i).arg(QString(list[i]))<<endl;
			QStringList sub_list = list[i].split(",");
            quint8 sub_line = quint8(sub_list.count());
            //qDebug()<<tr(">>>GPS:GNVTG Sub QstringList Count = %1 <<<").arg(sub_line)<<endl;
			if(sub_line > 7) {
				if(!(sub_list.at(1).isEmpty()) && !(sub_list.at(1).isNull())) {
                    //qDebug()<<tr(">>>GPS:GNVTG sublist [%1] = %2").arg(1).arg(QString(sub_list[1]))<<endl;
					gps_info.curr_cog = sub_list.at(1).toDouble();
					qDebug()<<tr("----->>>GPS:Current COG : %1 ").arg(gps_info.curr_cog)<<endl;
				}
				if(!(sub_list.at(5).isEmpty()) && !(sub_list.at(5).isNull())) {
                    //qDebug()<<tr(">>>GPS:GNVTG sublist [%1] = %2").arg(5).arg(QString(sub_list[5]))<<endl;
					gps_info.m_speed = sub_list.at(5).toDouble();
					qDebug()<<tr("----->>>GPS:Current Move Speed : %1 mile/h<<<").arg(gps_info.m_speed)<<endl;
				}
				if(!(sub_list.at(7).isEmpty()) && !(sub_list.at(7).isNull())) {
                    //qDebug()<<tr(">>>GPS:GNVTG sublist [%1] = %2").arg(7).arg(QString(sub_list[7]))<<endl;
					gps_info.n_speed = sub_list.at(7).toDouble();
					qDebug()<<tr(">>>--------------------GPS:Current Move Speed : %1 km/h <<<").arg(gps_info.n_speed)<<endl;
				}
			} else {
				continue;
			}
		}
        if(true == list[i].contains("GNGGA", Qt::CaseSensitive)) {
            //qDebug()<<tr(">>>GPS:StringList [%1] = %2 <<<").arg(i).arg(QString(list[i]))<<endl;
			QStringList sub_list = list[i].split(",");
            quint8 sub_line = quint8(sub_list.count());
            //qDebug()<<tr(">>>GPS:GNGGA Sub QstringList Count = %1 <<<").arg(sub_line)<<endl;
			if(sub_line > 10) {
				if(!(sub_list[2].isEmpty()) && !(sub_list[2].isNull()) ) {
					//纬度值
					gps_info.lat = quint8(sub_list[2].toInt()/100) + double(sub_list[2].toDouble()/100 - sub_list[2].toInt()/100 *60);
					gps_info.lat_dir = sub_list[3];
					qDebug()<<tr(">>>GPS:Current lat = %1, dir = %2 <<<").arg(gps_info.lat).arg(gps_info.lat_dir)<<endl;
				}
				if(!(sub_list[4].isEmpty()) && !(sub_list[4].isNull())) {
					//经度值
					gps_info.lon = quint8(sub_list[4].toInt()/100) + double(sub_list[4].toDouble()/100 - sub_list[4].toInt()/100 *60);
					gps_info.lon_dir = sub_list[5];
					qDebug()<<tr(">>>GPS:Current lon = %1, dir = %2 <<<").arg(gps_info.lon).arg(gps_info.lon_dir)<<endl;
				}
				if(list[i].length() < 64) {
					ret_str = record_gpsInfo;
					qDebug()<<tr(">>>Add Record Gps Information done!")<<endl;
				} else {
					ret_str = list[i];
					record_gpsInfo = ret_str;
				}
			} else {
				continue;
			}
		}
        if(true == list[i].contains("GNRMC", Qt::CaseSensitive)) {
            //qDebug()<<tr(">>>GPS:StringList [%1] = %2 <<<").arg(i).arg(QString(list[i]))<<endl;
			QStringList sub_list = list[i].split(",");
            quint8 sub_line = quint8(sub_list.count());
            //qDebug()<<tr(">>>GPS:GNRMC Sub QstringList Count = %1 <<<").arg(sub_line)<<endl;
			if(sub_line > 9) {
				if(!(sub_list[8].isEmpty()) && !(sub_list[8].isNull())) {
					gps_info.curr_cog = sub_list[8].toDouble();
					qDebug()<<tr(">>>GPS:Current COG = %1 <<<").arg(QString(sub_list[8]))<<endl;
				}
			} else {
				continue;
			}
		}
	}
	return ret_str;
}
/**********************************serialport*************************************/
//航空信息处理
QStringList Widget::Avi_prot_anal(QByteArray byte_arry, int len)
{
    QStringList ret_list;
    QStringList rev_list;
    QString tmp_str;
    uint8_t i = 0;
    int line;
    tmp_str = QString(byte_arry);
    rev_list = tmp_str.split("\n");
    line = rev_list.count();
    len += 1;
    qDebug()<<"avi line = "<<line;
//    uint8_t j = 0;
//    uint8_t count = 0;
    for(i = 0; i < line; i++)
    {
        //qDebug()<<"avi information "<<i<<" = "<<rev_list.at(i)<<endl;
        if(true != rev_list[i].contains("./",Qt::CaseSensitive))
        {

            continue;
            //ret_list.removeAt(i);
        /*
            for(j = line-i; j < line; j++)
            {
                ret_list[i]=ret_list[line-j];
                qDebug()<<"j = "<<j;
            }
        */
        }
        //ret_list.append(rev_list[i]);
        ret_list.insert(i,rev_list[i]);
        //qDebug()<<"avi_info line = "<<line;
        //qDebug()<<"avi_info i = "<<i;
    }
    return ret_list;
}
//处理多条航空信息
int Widget::Avi_info_pro(QStringList list)
{
    QString tmp_str;
    int count;
    QStringList tmp_list;
    QJsonObject tmp1_jsonobj, tmp2_jsonobj;
    QJsonValue tmp1_val, tmp2_val;
    int avi_count = 0;
    int tmp_jsoncnt = 0;
    count = list.count();
    qDebug()<<"avi list.count = "<<count;
#if 1
    for(int i = 0; i < count; i++)
    {
        tmp_list = list[i].split("/");
        //qDebug()<<"sub_list = "<<tmp_list;
        if(tmp_list.count() < 8)
            break;
    #if 0
        this->avia_arry->avia_udid = tmp_list[1].remove(QRegExp("\\s"));
        this->avia_arry[i].flight_num = tmp_list[2].remove(QRegExp("\\s"));
        this->avia_arry[i].alt = tmp_list[3].remove(QRegExp("\\s"));
        this->avia_arry[i].speed = tmp_list[4].remove(QRegExp("\\s"));
        this->avia_arry[i].lat = tmp_list[5].remove(QRegExp("\\s"));
        this->avia_arry[i].lon = tmp_list[6].remove(QRegExp("\\s"));
               // avia_arry[i]
        if(true != avia_arry[i].lat.contains(".",Qt::CaseSensitive))
            continue;
        if(true != avia_arry[i].lon.contains(".",Qt::CaseSensitive))
            continue;
    #endif
        QJsonObject tmp_jsobj;
        tmp_jsobj.insert("udid", tmp_list[1].remove(QRegExp("\\s")));
        tmp_jsobj.insert("flight_num", tmp_list[2].remove(QRegExp("\\s")));
        tmp_jsobj.insert("alt", tmp_list[3].remove(QRegExp("\\s")));
        tmp_jsobj.insert("speed", tmp_list[4].remove(QRegExp("\\s")));
        tmp_jsobj.insert("lat", tmp_list[5].remove(QRegExp("\\s")));
        tmp_jsobj.insert("lon", tmp_list[6].remove(QRegExp("\\s")));
        this->JS_avia_info_arry.append(tmp_jsobj);
    }
#endif

#if 1
    /* 删除重复的信息 */
    tmp_jsoncnt = this->JS_avia_info_arry.size();
    qDebug()<<"avi information count = "<<tmp_jsoncnt;
    if(tmp_jsoncnt > 1) {
       for(int i = 0; i < tmp_jsoncnt; i++) {
           if(this->JS_avia_info_arry.at(i).isObject()) {
               tmp1_jsonobj = this->JS_avia_info_arry.at(i).toObject();
               tmp1_val = tmp1_jsonobj.value("udid");
               for(int m = i+1; m < tmp_jsoncnt; m++) {
                   tmp2_jsonobj = this->JS_avia_info_arry.at(m).toObject();
                   tmp2_val = tmp2_jsonobj.value("udid");
                   //qDebug()<<"tmp1_val = "<<tmp1_val;
                   //qDebug()<<"tmp2_val = "<<tmp2_val;
                   if(tmp1_val == tmp2_val) {
                       this->JS_avia_info_arry.removeAt(i);
                       i--;
                       tmp_jsoncnt = this->JS_avia_info_arry.size();
                   }
               }
           }
           //qDebug()<<jsonArray.at(i);
       }
    }
#endif
    qDebug()<<"avi_info_arry count = "<<JS_avia_info_arry.size();
    return 1;

}
/*
    函数名：sleep()
    参   数： msec - 单位为毫秒
    描   述： 延时功能
*/
//ms延时
bool Widget::sleep_ms(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    return true;
}
//显示名称
void Widget::disp_name(QString name)
{
    QPainter painter(this);
    QFont font;
    font.setPointSize(40);
    font.setBold(true);

    QFontMetrics metrics(font);
    QPainterPath path;
    QPen pen(QColor(255, 0, 0, 100));
    int penWidth = font.pointSize()/2;
    if (penWidth > 6) {
        penWidth = 6;
    }
    pen.setWidth(penWidth);

    int len = metrics.width(name);
    int w = width();
    int px = (len - w) / 2;
    if (px < 0) {
        px = -px;
    }

    int py = (height() - metrics.height()) / 2 + metrics.ascent();
    if(py < 0)
    {
        py = -py;
    }
    path.addText(px, py, font, name);
    painter.strokePath(path, pen);
    painter.drawPath(path);
    painter.fillPath(path, QBrush(Qt::blue));

}
//时间更新
void Widget::timerUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss");
    QPalette pe_time;
    QFont ft_time("Microsoft YaHei", 14, 50);
    pe_time.setColor(QPalette::WindowText, Qt::red);
//    lab_time->setPalette(pe_time);
//    lab_time->setFont(ft_time);
//    this->lab_time->setText(str);
}
//显示系统名称
void Widget::disp_sys_name(QString name)
{
    QPalette pe_sys_name;
    QFont ft_sys_name("Microsoft Yahei", 18, 75);
    pe_sys_name.setColor(QPalette::WindowText, Qt::blue);
    lab_sys_name->setPalette(pe_sys_name);
    lab_sys_name->setFont(ft_sys_name);
    this->lab_sys_name->setText(name);
}

//网络断开处理
void Widget::on_discNetProcess(bool Netstat)
{

    if(!Netstat) {
        qDebug()<<"The network signal is abnormal."<<endl;
//        imag_netstat = QPixmap(":/Icon/picture/net_no1.jpg");
//        net_imag = imag_netstat.scaled(lab_netstat->size(),Qt::KeepAspectRatio);
//        lab_netstat->setPixmap(net_imag);
        discNet_count--;
        if(discNet_count < 1) {
            emit send_reset4g();
        }
    }
    else {
        qDebug()<<"Network signal normal."<<endl;
        discNet_count = 20;
//        imag_netstat = QPixmap(":/Icon/picture/net_ok2.jpg");
//        net_imag = imag_netstat.scaled(lab_netstat->size(),Qt::KeepAspectRatio);
//        lab_netstat->setPixmap(net_imag);
    }
}
//检查外部进程是否运行
bool Widget::checkAppRunstat(const QString &appName)
{
    QProcess* process = new QProcess;
    process->start("tasklist", QStringList()<<"/FI"<<"imagename eq" + appName);
    process->waitForFinished();
    QString outputStr = QString::fromLocal8Bit(process->readAllStandardOutput());
    if(outputStr.contains(appName)) {
        return true;
    }
    else {
        return false;
    }
}
//关闭外部进程
void Widget::killApp_process(const QString &appName)
{
    QProcess p;
    QString c = "pkill "+appName;
    p.execute(c);
    p.close();
}
//发送开启指令
void Widget::pbutt_start_event()
{
    uint8_t cmd_len = 0x4;      //指令长度
    uint8_t dev_type = 0x1;     //设备类型
    uint8_t id_shield = 0x0;    //屏蔽器ID
    uint8_t id_pa = 0x0;    //功放ID
    uint8_t stat = 0x0;     //功放状态
    uint8_t check_sum = 0;  //校验和
    uint8_t i = 0;

    comm_dev_txbuf.clear();
    comm_dev_txbuf[0] = char(0xA0);
    comm_dev_txbuf[1] = char(0xB0);
    comm_dev_txbuf[2] = char(cmd_len);
    comm_dev_txbuf[3] = char(dev_type);
    id_shield = uint8_t(commbox_shieldid->currentIndex());
    comm_dev_txbuf[4] = char(id_shield);

    switch (this->commbox_paid->currentIndex()) {
    case 0:
        id_pa = 0x0;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 1:
        id_pa = 0x1;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 2:
        id_pa = 0x2;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 3:
        id_pa = 0x3;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 4:
        id_pa = 0x4;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 5:
        id_pa = 0x5;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 6:
        id_pa = 0x6;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 7:
        id_pa = 0x7;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 8:
        id_pa = 0x8;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 9:
        id_pa = 0x9;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 10:
        id_pa = 0xFF;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    default:
        id_pa = 0xFF;
        stat = 0x1;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    }
    comm_dev_txbuf[7] = 0x0A;
    comm_dev_txbuf[8] = 0x0B;
    for(i = 0; i < 9; i++)
    {
        check_sum += comm_dev_txbuf.at(i);
    }
    check_sum &=0xFF;
    comm_dev_txbuf[9] = char(check_sum);

    emit serialsend(comm_dev_txbuf);

}
//发送停止指令
void Widget::pbutt_stop_event()
{
    uint8_t cmd_len = 0x4;      //指令长度
    uint8_t dev_type = 0x1;     //设备类型
    uint8_t id_shield = 0x0;    //屏蔽器ID
    uint8_t id_pa = 0x0;    //功放ID
    uint8_t stat = 0x0;     //功放状态
    uint8_t check_sum = 0;  //校验和
    uint8_t i = 0;

    comm_dev_txbuf.clear();
    comm_dev_txbuf[0] = char(0xA0);
    comm_dev_txbuf[1] = char(0xB0);
    comm_dev_txbuf[2] = char(cmd_len);
    comm_dev_txbuf[3] = char(dev_type);
    id_shield = uint8_t(commbox_shieldid->currentIndex());
    comm_dev_txbuf[4] = char(id_shield);

    switch (this->commbox_paid->currentIndex()) {
    case 0:
        id_pa = 0x0;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 1:
        id_pa = 0x1;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 2:
        id_pa = 0x2;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 3:
        id_pa = 0x3;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 4:
        id_pa = 0x4;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 5:
        id_pa = 0x5;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 6:
        id_pa = 0x6;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 7:
        id_pa = 0x7;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 8:
        id_pa = 0x8;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 9:
        id_pa = 0x9;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    case 10:
        id_pa = 0xFF;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    default:
        id_pa = 0xFF;
        stat = 0x0;
        comm_dev_txbuf[5] = char(id_pa);
        comm_dev_txbuf[6] = char(stat);
        break;
    }
    comm_dev_txbuf[7] = 0x0A;
    comm_dev_txbuf[8] = 0x0B;
    for(i = 0; i < 9; i++)
    {
        check_sum += comm_dev_txbuf.at(i);
    }
    check_sum &=0xFF;
    comm_dev_txbuf[9] = char(check_sum);

    emit serialsend(comm_dev_txbuf);
}
//服务器数据解析
void Widget::recvDowninfo_parser(QByteArray bytearr)
{
    QJsonParseError jsonerro;
    //int shield_count = 0;
    //int pa_count = 0;
    bool recv_stat = false;
    QJsonDocument tmp_doucment = QJsonDocument::fromJson(bytearr, &jsonerro);
    qDebug()<<tr("--->>>Recv data bytearr:%1<<<---").arg(QString(bytearr))<<endl;
    if(!tmp_doucment.isNull() && (jsonerro.error == QJsonParseError::NoError))  {
        if(tmp_doucment.isObject()) {
            QJsonObject tmp_obj = tmp_doucment.object();
            if(tmp_obj.contains("operation")) {
                QJsonValue tmp_val = tmp_obj.value("operation");
                if(tmp_val.isString()) {
                    QString tmp_str = tmp_val.toString();
                    qDebug()<<"operation:"<<tmp_str;
                }
            }
            if(tmp_obj.contains("obj")) {       //obj键是否存在
                QJsonValue tmp_val = tmp_obj.value("obj");
                if(tmp_val.isObject()) {        //Object
                    QJsonObject obj_obj = tmp_val.toObject();
                    //qDebug()<<"obj:"<<obj_obj<<endl;
                    //ui->ptEdit_dataparser->setPlainText(obj_obj);
                    //ui->ptEdit_dataparser->appendPlainText(obj_obj);
                    if(obj_obj.contains("frequencyRange")) {      //freq键是否存在
                        tmp_val = obj_obj.value("frequencyRange");
                        if(tmp_val.isArray()) {             //freq是否为数组
                            QJsonArray arr_freq = tmp_val.toArray();
                            //qDebug()<<"frequencyRange:"<<arr_freq<<endl;
                            //ui->ptEdit_dataparser->appendPlainText(arr_freq);
                            int nSize = arr_freq.size();
                            shield_count = nSize;       //获取屏蔽器数量

                            for(int i = 0; i < nSize;i++) {     //遍历freq
                                QJsonValue freq_val = arr_freq.at(i);
                                if(freq_val.isObject()) {           //是否为对象
                                    QJsonObject obj_shield = freq_val.toObject();
                                    qDebug()<<"obj_shield:"<<obj_shield<<endl;
                                    //ui->ptEdit_dataparser->appendPlainText(obj_shield);
                                    if(obj_shield.contains("shieldId")) {       //shield_id键是否存在
                                        QJsonValue shield_id_val = obj_shield.value("shieldId");
                                        qDebug()<<"shield_id:"<<shield_id_val<<endl;
                                        shield_info[i].shield_id = quint8(shield_id_val.toInt());
                                        //ui->ptEdit_dataparser->appendPlainText(shield_id_val);
                                    }
                                    if(obj_shield.contains("shieldInfo")) {        //pa_info键是否存在
                                        QJsonValue pa_info_val = obj_shield.value("shieldInfo");
                                        qDebug()<<"shield_info:"<<pa_info_val<<endl;
                                        //ui->ptEdit_dataparser->appendPlainText(pa_info_val);
                                        if(pa_info_val.isArray()) {             //pa_info是否为数组
                                            QJsonArray arr_pa_info = pa_info_val.toArray();
                                            qDebug()<<"arr_pa_info:"<<arr_pa_info;
                                            int iSize = arr_pa_info.size();     //获取数组大小
                                            shield_info[i].pa_count = quint8(iSize);    //获取功放数量
                                            for(int m = 0; m < iSize; m++) {        //遍历pa_info
                                                QJsonValue pa_val = arr_pa_info.at(m);
                                                if(pa_val.isObject()) {         //是否为对象
                                                    QJsonObject obj_pa = pa_val.toObject();
                                                    qDebug()<<"obj_pa:"<<obj_pa<<endl;
                                                    if(obj_pa.contains("id")) {      //pa_id键是否存在
                                                        QJsonValue pa_id_val = obj_pa.value("id");
                                                        qDebug()<<"id:"<<pa_id_val<<endl;
                                                        this->shield_info[i].pa_info[m].pa_id = quint8(pa_id_val.toInt());
                                                        //ui->ptEdit_dataparser->appendPlainText(pa_id_val);
                                                        recv_stat = true;
                                                    }
                                                    if(obj_pa.contains("switch")) {       //stat键是否存在
                                                        QJsonValue pa_switch_val = obj_pa.value("switch");
                                                        qDebug()<<"switch:"<<pa_switch_val<<endl;
                                                        this->shield_info[i].pa_info[m].pa_swtich = quint8(pa_switch_val.toInt());
                                                        //ui->ptEdit_dataparser->appendPlainText(pa_switch_val);
                                                    }
                                                    if(obj_pa.contains("status")) {
                                                        QJsonValue pa_stat_val = obj_pa.value("status");
                                                        qDebug()<<"stat:"<<pa_stat_val<<endl;
                                                        this->shield_info[i].pa_info[m].alarm_stat = qint8(pa_stat_val.toInt());
                                                    }
                                                    if(obj_pa.contains("frequency")) {
                                                        QJsonValue pa_freq_val = obj_pa.value("frequency");
                                                        qDebug()<<"frequency:"<<pa_freq_val<<endl;
                                                        this->shield_info[i].pa_info[m].pa_freq = quint16(pa_freq_val.toInt());
                                                    }
                                                }
                                            }
                                        }

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    //qDebug()<<"shield_info:"<<shield_info<<endl;
    if(recv_stat) {
        send_Devcmd();
    }

}

//生成下发设备指令--19年树莓派--中控箱硬件协议
void Widget::send_Devcmd()
{
    QByteArray send_cmd;
    Exid_st exid_st;
    int check_sum = 0;
    //send_cmd.resize(11);
    if(shield_count != 0) {
        qDebug()<<"Down command shield_count:"<<shield_count<<endl;
        if(shield_count > 7) {
            qDebug()<<"Receive Invalid Server command!"<<endl;
            return;
        }
        for(int i = 0; i < shield_count; i++) {
            if(shield_info[i].pa_count != 0) {
                int pa_count = shield_info[i].pa_count;
                qDebug()<<tr("Down command shield[%1] pa_count = %2").arg(shield_info[i].shield_id)
                          .arg(pa_count)<<endl;
                if(pa_count > 16) {
                    qDebug()<<"Receive Invalid Server command!"<<endl;
                    return;
                }

                //2020年RK3399下行指令--2019协议  2020-5-1
                if(dev_info.comm_pro_ver == "V1.0") {
                    for(int m = 0; m < pa_count; m++) {
                        exid_st.shield_id = (u_char)shield_info[i].shield_id;
                        exid_st.ch_id = (u_char)shield_info[i].pa_info[m].pa_id;
                        exid_st.index = 0;
                        exid_st.len = 0x8;
                        if(shield_info[i].pa_info[m].pa_swtich == 0x01) {
                            exid_st.dat[0] = 1;
                        } else {
                            exid_st.dat[0] = 0;
                        }
                        exid_st.dat[1] = 0;
                        exid_st.dat[2] = 0;
                        exid_st.dat[3] = 0;
                        exid_st.dat[4] = 0;
                        exid_st.dat[5] = 0;
                        exid_st.dat[6] = 0;
                        exid_st.dat[7] = 0;
                        send_Shield_cmd(exid_st, exid_st.dat);
                        sleep_ms(100);
                    }
                } else if(dev_info.comm_pro_ver == "V2.0") {
                    //2020年协议
                    for(int m = 0; m < pa_count; m++) {
                        exid_st.shield_id = (u_char)shield_info[i].shield_id;
                        exid_st.ch_id = (u_char)shield_info[i].pa_info[m].pa_id;
                        exid_st.index = 0x8;
                        exid_st.len = 0x8;
                        if(shield_info[i].pa_info[m].pa_swtich == 0x01) {
                            exid_st.dat[0] = 1;
                        } else {
                            exid_st.dat[0] = 0;
                        }
                        exid_st.dat[1] = 0;
                        exid_st.dat[2] = 0;
                        exid_st.dat[3] = 0;
                        exid_st.dat[4] = 0;
                        exid_st.dat[5] = 0;
                        exid_st.dat[6] = 0;
                        exid_st.dat[7] = 0;
                        send_Shield_cmd(exid_st, exid_st.dat);
                        sleep_ms(100);
                    }

                } else {
                    qDebug()<<tr(">>>>No supported version of the protocol was found.<<<<")<<endl;  //未找到支持的协议版本
#if 0           //2019年树莓派下行指令
                for(int m = 0; m < pa_count; m++) {
                    send_cmd.clear();
                    send_cmd[0] = 0xA0;
                    send_cmd[1] = 0xB0;
                    send_cmd[2] = 0x4;   //发送指令长度
                    send_cmd[3] = 0x1;   //设备类型
                    send_cmd[4] = (u_char)shield_info[i].shield_id;  //屏蔽器ID
                    send_cmd[5] = (u_char)shield_info[i].pa_info[m].pa_id;
                    if(shield_info[i].pa_info[m].pa_swtich == 0x1) {
                        send_cmd[6] = 0x1;
                    }
                    else {
                        send_cmd[6] = 0x0;
                    }
                    send_cmd[7] = 0xB;
                    send_cmd[8] = 0xA;
                    check_sum = 0;
                    for(int n = 0; n < 9; n++) {
                        check_sum += send_cmd.at(n);
                    }
                    send_cmd[9] = (u_char)(check_sum&0xFF);
                    /********************发送数据**********************/
                    qDebug()<<"send_dev_cmd:"<<send_cmd.toHex()<<endl;
                    emit serialsend(send_cmd);      //发送设备指令
                    /********************发送数据**********************/
                    /**********************延时100ms*********************/
                    sleep_ms(100);
                    /**********************延时100ms*********************/
                }
#endif
                }
            } else {
                //屏蔽器通道数量错误
                qDebug()<<tr("----->>Send Device Command Shield ID = %1 Ch_Count Error!<<-----").arg(shield_info[i].shield_id)<<endl;
            }
            /**********************延时300ms*********************/
        }
    }
}
//下发CAN指令--2020年硬件协议 2020-4-30
void Widget::send_Shield_cmd(Exid_st exid_t, quint8 *val)
{
    QByteArray cmd_arry;
    char ch_tmp[32];
    quint32 exid = 0x10000000;

    exid |= uint(exid_t.shield_id<<16);
    exid |= uint(exid_t.ch_id<<8);
    exid |= exid_t.index;
    //exid = exid>>3;
    exid = exid<<3;
    exid |= 0x4;

    ch_tmp[0] = 0x41;
    ch_tmp[1] = 0x54;
    ch_tmp[2] = char((exid>>24)&0xFF);
    ch_tmp[3] = char((exid>>16)&0xFF);
    ch_tmp[4] = char((exid>>8)&0xFF);
    ch_tmp[5] = char(exid&0xFF);
    ch_tmp[6] = char(exid_t.len);
    for(quint8 i = 0; i < exid_t.len; i++) {
        ch_tmp[7+i] = *(val+i);
    }
    ch_tmp[7+exid_t.len] = 0x0d;
    ch_tmp[8+exid_t.len] = 0x0a;
    cmd_arry.append(ch_tmp,exid_t.len+9);
    emit serialsend(cmd_arry);
    qDebug()<<tr(">>>Send Shield Command:")<<cmd_arry.toHex()<<tr("<<<")<<endl;
}
//下发CAN指令--2020-9-8
void Widget::send_Compass_cmd(quint8 cmd_code)
{
    QByteArray cmd_arry;
    char ch_tmp[32];
    quint32 exid = 0x12000000;
    quint8 val[8] = {0xFE,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
    quint8 len = 8;

    switch (cmd_code) {
    case 1:
        exid |= 0x1;
        break;
    case 2:
        exid |= 0x2;
        break;
    case 3:
        exid |= 0xE;
        break;
    case 4:
        exid |= 0xF;
        break;
    default:
        break;
    }
    exid = exid<<3;
    exid |= 0x4;
    ch_tmp[0] = 0x41;
    ch_tmp[1] = 0x54;
    ch_tmp[2] = char((exid>>24)&0xFF);
    ch_tmp[3] = char((exid>>16)&0xFF);
    ch_tmp[4] = char((exid>>8)&0xFF);
    ch_tmp[5] = char(exid&0xFF);
    ch_tmp[6] = 0x8;
    for(quint8 i = 0; i < len; i++) {
        ch_tmp[7+i] = *(val+i);
    }
    ch_tmp[7+len] = 0x0d;
    ch_tmp[8+len] = 0x0a;
    cmd_arry.append(ch_tmp,len+9);
    emit serialsend(cmd_arry);
    qDebug()<<tr(">>>Send Compass Command:")<<cmd_arry.toHex()<<"<<<"<<endl;
}
//下发重启4G模块指令
void Widget::send_DevcmdReset4G()
{

}
void Widget::slot_sendmdInfo()
{
    QString md_send_str;
    if(dev_info.dev_type > 12 && dev_info.dev_type < 19) {
        //2020-11-10 增加对设备类型识别
        switch (dev_info.dev_type) {
        case 13:
            if(cloud_whistle->info_stat == 1) {
                md_send_str = QString(QJsonDocument(this->cloud_whistle->md_upMQ_info).toJson());
                emit this->cloud_whistle->clear_allData();
                emit this->sendmdMsg(md_send_str);
                qDebug()<<tr(">>>TCP Whistle:Websocket Interface Send Wave Detecte UAV Information Data Done!")<<endl;
            }
            break;
        case 14:
            if(wavedetec_stat == 2) {
                md_send_str = QString(QJsonDocument(this->wavedetec_dev->md_upMQ_info).toJson());
                emit this->wavedetec_dev->clear_allData();
                //qDebug()<<tr(">>>>Websocket Interface Send Wave Detecte UAV Information Data: %1 ").arg(md_send_str)<<endl;
                emit this->sendmdMsg(md_send_str);
                qDebug()<<tr(">>>TCP Wavedetect:Websocket Interface Send Wave Detecte UAV Information Data Done!")<<endl;
            } else if(wavedetec_stat == 1) {
                md_send_str = QString(QJsonDocument(this->http_cli->json_MQ).toJson());
                emit this->http_cli->clear_allData();
                emit this->sendmdMsg(md_send_str);
                qDebug()<<tr(">>>HTTP Wavedetect:Websocket Interface Send Wave Detecte UAV Information Data Done!")<<endl;
            } else if(wavedetec_stat == 3) {
                md_send_str = QString(QJsonDocument(this->zmq_wavedetec_dev->js_obj).toJson());
                emit this->zmq_wavedetec_dev->sig_clearUpInfo();
                emit this->sendmdMsg(md_send_str);
                zmq_wavedetec_dev->dev_lat = gps_info.lat;
                zmq_wavedetec_dev->dev_lon = gps_info.lon;
                qDebug()<<tr(">>>ZMQ Wavedetect:Websocket Interface Send Wave Detecte UAV Information Data Done!")<<endl;
            }
            break;
        case 15:
            if(radar_stat == 1) {
                md_send_str = QString(QJsonDocument(this->m_radar->md_upMQ_info).toJson());
                emit this->m_radar->sig_clearJsData();
                emit this->sendmdMsg(md_send_str);
                qDebug()<<tr(">>>UDP Radar:Websocket Interface Send Radar Detecte UAV Information Data Done!")<<endl;
            }
            break;
        default:
            break;
        }
        //2020-11-10 增加对设备类型识别
    }
}
//定时发送websocket心跳包
void Widget::timer_sendHeartBeatMsg()
{
    QString md_send_str;
    QJsonObject json_obj;

    md_send_timer->stop();
    json_obj.insert("Msg Type", "This a Heartbeat Packets..");
    md_send_str = QString(QJsonDocument(json_obj).toJson());
    QByteArray send_arry = md_send_str.toUtf8();
    switch (dev_info.dev_type) {
    case 13:
        emit this->cloud_whistle->sign_addHeartBeat(send_arry);
        //正式服务器和测试服务器模式
        //emit this->sendmdMsg("This a Heartbeat Packgets...");
        qDebug()<<tr("-->>Send Websocket Interface Heartbeat Packgets...<<--")<<endl;
        break;
    case 14:
        break;
    case 15:
        break;
    default:
        break;
    }
    md_send_timer->start(30*1000);
}
//接收到侦测设备信息
void Widget::rec_md_inf()
{
    timer_out_md.stop();
    this->http_cli->Http_DetResults(this->http_cli->url_getsys);
    timer_out_md.start(1000);
}
//显示设备信息
void Widget::disp_Dev_Info()
{
    for(quint8 i = 0; i < 16; i++) {
        if(dev_info.shield_arry[i].ch_count != 0) {
            qDebug()<<tr("-----------Shield Information-------------")<<endl;
            qDebug()<<tr(">>>Shield ID = %1,Temp = %2, SW_Stat = %3, Mode = %4, ")
                      .arg(dev_info.shield_arry[i].shield_id)
                      .arg(dev_info.shield_arry[i].shield_temp)
                      .arg(dev_info.shield_arry[i].shield_sw_stat)
                      .arg(dev_info.shield_arry[i].shield_mode)<<endl;
            for(quint8 j = 0; j < 32; j++) {
                qDebug()<<tr(">>Channel ID = %1, Freq = %2, SW_Stat = %3, Power_Gear = %4.").arg(dev_info.shield_arry[i].ch_info[j].ch_id).arg(dev_info.shield_arry[i].ch_info[j].ch_freq).arg(dev_info.shield_arry[i].ch_info[j].ch_swtich).arg(dev_info.shield_arry[i].ch_info[j].ch_power_gear)<<endl;
            }
            qDebug()<<tr("-----------Shield Information-------------")<<endl;
        }
    }
}
//测试屏蔽器指令
void Widget::test_SendShield_Cmd()
{
    Exid_st exid_st;
    exid_st.shield_id = 0xF;
    exid_st.ch_id = 0xF;
    exid_st.index = 0x5;
    exid_st.len = 0x8;
    exid_st.dat[0] = 0x1;
    exid_st.dat[1] = 0x2;
    exid_st.dat[2] = 0x3;
    exid_st.dat[3] = 0x4;
    exid_st.dat[4] = 0x5;
    exid_st.dat[5] = 0x6;
    exid_st.dat[6] = 0x7;
    exid_st.dat[7] = 0x8;

    send_Shield_cmd(exid_st, exid_st.dat);
}
//网络CAN数据处理
void Widget::slot_Netcan_pro(QByteArray tmpdata)
{
    QString tmp_str;
    QString comp_str;
    QDateTime date_time = QDateTime::currentDateTime();
    QTime current_time = QTime::currentTime();

    qDebug()<<tr(">>>>Receive Device Send Data:")<<tmpdata.toHex()<<"<<<<---"<<date_time.toString()<<endl;
    if(tmpdata.at(0) == 0x88) {
        QByteArray Can_data;    //CAN数据
        quint32 Extid;          //扩展ID
        quint8 Can_data_len;    //CAN数据长度
        quint8 shield_id;       //屏蔽器ID
        quint8 channel_id;      //信道ID
        //quint8 pa_id;           //功放ID
        quint8 info_num;        //信息序号
        QList<quint8>::iterator ite_sh;     //屏蔽器迭代器
        QList<quint8>::iterator ite_ch;     //信道迭代器
        Extid = quint32((tmpdata.at(1)<<24)+(tmpdata.at(2)<<16)+(tmpdata.at(3)<<8)+tmpdata.at(4));
        qDebug()<<tr("--->>>Extid = %1").arg(Extid)<<endl;
        //Extid = (Extid>>3);     //网络转CAN模块注释掉
        shield_id = Extid>>16&0xFF;
        channel_id = Extid>>8&0xFF;
        //pa_id = channel_id;
        info_num = Extid&0xFF;
        Can_data_len = 8;       //数据长度
        if(shield_id < 16) {    //屏蔽器ID最大255
            //vector_Shield[shield_id].shield_ID = shield_id;
            qDebug()<<tr("--->>>Shield ID = %1, Channel ID = %2, Info Num = %3<<<")
                      .arg(shield_id).arg(channel_id).arg(info_num)<<endl;
            bool yorn = true;
            //统计屏蔽器数量--2020-5-1
            for(quint8 i = 0; i < 16; i++) {
                //qDebug()<<tr(">>>Device Information Shield arry[%1] Shield ID = %2 <<<").arg(i).arg(dev_info.shield_arry[i].shield_id)<<endl;
                //查询ID是否存在
                if(dev_info.shield_arry[i].shield_id != shield_id) {
                    yorn = false;
                } else {
                    yorn = true;
                    break;
                }
            }
            if(!yorn) {
                dev_info.shield_arry[shield_id].shield_id = shield_id;                
                dev_info.shield_IDlist.append(shield_id);
                dev_info.group_num++;
            }
        #if 0
            //遍历屏蔽器迭代器
            for(ite_sh = dev_info.shield_IDlist.begin(); ite_sh != dev_info.shield_IDlist.end(); ++ite_sh) {
                if(*ite_sh != shield_id) {
                    dev_info.shield_IDlist.append(shield_id);

                } else {
                    ;
                }
            }
        #endif
        } else {
            //接收到无效的ID号
            qDebug()<<tr("Recieve Invalid Device ID Number!<<<")<<endl;
        }
        if((Extid>>24) == 0x01) {
            //信道和功放信息
            if(dev_info.comm_pro_ver == "V1.0" && channel_id < 0x20) {
                bool yorn = true;
                for(quint8 i = 0; i < 16; i++) {
                    if((dev_info.shield_arry[shield_id].ch_info[i].ch_id != channel_id)) {
                        yorn = false;

                    } else {
                        yorn = true;
                        break;
                    }
                }
                if(!yorn) {
                    //刷新PAID
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id = channel_id;
                    dev_info.shield_arry[shield_id].ch_count++;
                }
                switch (info_num) {
                case 0:
                    //功放状态信息1
                    dev_info.shield_arry[shield_id].pa_info[channel_id].alarm_stat = tmpdata.at(5);
                    dev_info.shield_arry[shield_id].pa_info[channel_id].swtich_stat = quint8(tmpdata.at(6));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].f_power = quint8(tmpdata.at(7));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].b_power = quint8(tmpdata.at(8));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].temp = quint8(tmpdata.at(9));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].volt = quint8(tmpdata.at(10));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].current = quint8(tmpdata.at(11));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].swr = quint8(tmpdata.at(12));
                    qDebug()<<tr(">>>>Version1.0 Tmpdata Data: alarm_stat = %1, swtich_stat = %2, volt = %3, current = %4 <<<<")
                              .arg(QString::number(tmpdata.at(5))).arg(QString::number(tmpdata.at(6)))
                              .arg(QString::number(tmpdata.at(10))).arg(QString::number(tmpdata.at(11)))<<endl;
                    break;
                case 1:
                    //功放状态信息2
                    dev_info.shield_arry[shield_id].pa_info[channel_id].freq = quint16((tmpdata.at(5)<<8)+tmpdata.at(6));
                    dev_info.shield_arry[shield_id].pa_info[channel_id].band = quint16((tmpdata.at(7)<<8)+tmpdata.at(8));
                    qDebug()<<tr(">>>>>Version1.0 Tmpdata Data: Freq = %1, Band = %2 <<<<")
                              .arg(QString::number((tmpdata.at(5)<<8)+tmpdata.at(6))).arg(QString::number((tmpdata.at(7)<<8)+tmpdata.at(8)))<<endl;

                    break;
                case 2:
                    //工作状态
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_freq = quint16((tmpdata.at(5)<<8)+tmpdata.at(6));
                    dev_info.shield_arry[shield_id].ch_info[channel_id].pa_id = quint8(tmpdata.at(7));
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_swtich = quint8(tmpdata.at(8));
                    qDebug()<<tr(">>>>>Version1.0 Tmpdata Data: CH_Freq = %1, Pa_id = %2, CH_Swtich = %3 <<<<")
                              .arg(QString::number((tmpdata.at(5)<<8)+tmpdata.at(6))).arg(QString::number(tmpdata.at(7))).arg(QString::number(tmpdata.at(8)))<<endl;
                    break;
                default:
                    break;
                }
            } else if(dev_info.comm_pro_ver == "V2.0" && channel_id < 0x20) {
                bool yorn = true;
                for(quint8 i = 0; i < 32; i++) {
                    if((dev_info.shield_arry[shield_id].ch_info[i].ch_id != channel_id)) {
                        yorn = false;
                        qDebug()<<tr(">>>>Version2.0 Information:yorn = false").arg(yorn)<<endl;
                    } else {
                        yorn = true;
                        qDebug()<<tr(">>>>Version2.0 Information:yorn = true")<<endl;
                        break;
                    }
                }
                if(!yorn) {
                    //刷新PAID
                    dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id = channel_id;
                    dev_info.shield_arry[shield_id].ch_count++;
                    qDebug()<<tr(">>>>Version2.0 Tmpdata Data:CH_count = %1").arg(dev_info.shield_arry[shield_id].ch_count)<<endl;
                }
                switch (info_num) { //信息序号
                case 0:
                    //信道信息
                    if(channel_id < 32) {
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_swtich = quint8(tmpdata.at(5));
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_current = quint16(((tmpdata.at(6)<<8)+tmpdata.at(7))/1000);  //mA->A
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_power_gear = quint8(tmpdata.at(8));
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_freq = quint16((tmpdata.at(10)<<8)+tmpdata.at(11));
                        dev_info.shield_arry[shield_id].ch_info[channel_id].ch_swr = (tmpdata.at(12))/10;    //Unit = 0.1
                        //dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id = channel_id;
                        dev_info.shield_arry[shield_id].pa_info[channel_id].f_power = 0;
                        dev_info.shield_arry[shield_id].pa_info[channel_id].b_power = 0;
                        dev_info.shield_arry[shield_id].pa_info[channel_id].volt = 28;
                        dev_info.shield_arry[shield_id].pa_info[channel_id].current = 0;
                        dev_info.shield_arry[shield_id].pa_info[channel_id].alarm_stat = 0;
                        dev_info.shield_arry[shield_id].pa_info[channel_id].swr = 0;
                        dev_info.shield_arry[shield_id].pa_info[channel_id].temp = 0;
                        qDebug()<<tr(">>>>Version1.0 Information: Add Channel Information OK!")<<endl;
                    }
                    qDebug()<<tr(">>>>Version1.0 Channel Tmpdata Data: Switch = %1, Current = %2 mA, Power_gear = %3, Freq = %4 SWR = %5 Channel_id = %6 <<<<")
                              .arg(QString::number(tmpdata.at(5))).arg(QString::number((tmpdata.at(6)<<8)+tmpdata.at(7)))
                              .arg(QString::number(tmpdata.at(8))).arg(QString::number((tmpdata.at(10)<<8)+tmpdata.at(11)))
                              .arg(QString::number(tmpdata.at(12))).arg(QString::number(channel_id))<<endl;
#if 0
                    //遍历信道迭代器
                    for(ite_ch = dev_info.shield_arry[shield_id].ch_list.begin(); ite_ch != dev_info.shield_arry[shield_id].ch_list.end(); ++ite_ch) {
                        if(*ite_ch != dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id) {
                            dev_info.shield_arry[shield_id].ch_list.append(dev_info.shield_arry[shield_id].ch_info[channel_id].ch_id);
                        }
                    }
#endif
                    break;
                case 1:
                    //屏蔽器信息
                    dev_info.shield_arry[shield_id].shield_sw_stat = quint8(tmpdata.at(5));  //开关状态
                    dev_info.shield_arry[shield_id].shield_temp = quint8(tmpdata.at(6));     //温度
                    dev_info.shield_arry[shield_id].shield_mode = quint8(tmpdata.at(7));     //防御模式 1-躯离 2-迫降
                    qDebug()<<tr(">>>>Version2.0 Shield Tmpdata Data: Switch = %1, Temp = %2, Mode = %3 <<<<")
                              .arg(QString::number(tmpdata.at(5))).arg(QString::number(tmpdata.at(6))).arg(QString::number(tmpdata.at(7)))<<endl;
                    break;
                case 2:
                    //屏蔽器信息(模式配置）
                    dev_info.shield_arry[shield_id].shield_cfg_exple = quint32((tmpdata.at(5)<<24)+(tmpdata.at(6)<<16)+(tmpdata.at(7)<<8)+tmpdata.at(8));
                    dev_info.shield_arry[shield_id].shield_cfg_land = quint32((tmpdata.at(9)<<24)+(tmpdata.at(10)<<16)+(tmpdata.at(11)<<8)+tmpdata.at(12));
                    break;
                case 3:
                    //序列号
                    dev_info.shield_arry[shield_id].sn[15] = quint8(tmpdata.at(5));
                    dev_info.shield_arry[shield_id].sn[14] = quint8(tmpdata.at(6));
                    dev_info.shield_arry[shield_id].sn[13] = quint8(tmpdata.at(7));
                    dev_info.shield_arry[shield_id].sn[12] = quint8(tmpdata.at(8));
                    dev_info.shield_arry[shield_id].sn[11] = quint8(tmpdata.at(9));
                    dev_info.shield_arry[shield_id].sn[10] = quint8(tmpdata.at(10));
                    dev_info.shield_arry[shield_id].sn[9] = quint8(tmpdata.at(11));
                    dev_info.shield_arry[shield_id].sn[8] = quint8(tmpdata.at(12));
                    break;
                case 4:
                    dev_info.shield_arry[shield_id].sn[7] = quint8(tmpdata.at(5));
                    dev_info.shield_arry[shield_id].sn[6] = quint8(tmpdata.at(6));
                    dev_info.shield_arry[shield_id].sn[5] = quint8(tmpdata.at(7));
                    dev_info.shield_arry[shield_id].sn[4] = quint8(tmpdata.at(8));
                    dev_info.shield_arry[shield_id].sn[3] = quint8(tmpdata.at(9));
                    dev_info.shield_arry[shield_id].sn[2] = quint8(tmpdata.at(10));
                    dev_info.shield_arry[shield_id].sn[1] = quint8(tmpdata.at(11));
                    dev_info.shield_arry[shield_id].sn[0] = quint8(tmpdata.at(12));
                    break;
                default:
                    break;
                }
            } else {
                qDebug()<<tr("---Device Communication protocol version Error!---")<<endl;
            }

        } else if((Extid>>24) == 0x02) {
            //磁罗盘信息
            dev_info.att_info.dir_angle = quint16((tmpdata.at(5)<<8)+tmpdata.at(6));
            if(dev_info.comm_pro_ver == "V2.0") {
                dev_info.att_info.pit_angle = quint16((tmpdata.at(7)<<8)+tmpdata.at(8));
            } else {
                dev_info.att_info.pit_angle = 0;
            }
        } else if((Extid>>24) == 0x03) {
            //电池箱信息
            dev_info.batt_info.work_volt = quint8(tmpdata.at(5));
            dev_info.batt_info.work_current = quint8(tmpdata.at(6));
            dev_info.batt_info.temp = quint8(tmpdata.at(7));
            dev_info.batt_info.surplus_cap = quint8(tmpdata.at(8));
            dev_info.batt_info.trouble_code = quint8(tmpdata.at(9));
        } else {
            qDebug()<<tr(">>>Receive Invalid CAN Data Information!<<<")<<endl;
            //其它数据信息
        }
    }
    //显示设备发送消息  2019-9-5
    if(tmpdata.length() != 0)
    {
        comp_str.append("[");
        comp_str.append(date_time.date().toString());
        comp_str.append(current_time.toString());
        comp_str.append("]:");
        comp_str.append(tmpdata.toHex());
        tmp_str = QString::number(recv_count_dev++);
        qDebug()<<comp_str;
        autoScrollTextEdit(tedit_dev, comp_str, tmp_str);     //自动文本框最后一行
    }
    Dev_prot_anal(tmpdata, tmpdata.length());
}
