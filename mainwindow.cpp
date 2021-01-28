#include "mainwindow.h"
#include "ui_mainwindow.h"

#define DEBUG_EN

#define UPSERVER_IP "39.102.43.157"
#define UPSERVER_USER "zhang"
#define UPSERVER_PASS "zhang123456"
#define UPSERVER_PATH "online_Update/RK3399"
#define LOCALSAVE_PATH "/home/pi/Downloads/UpdateFile/RK3399"

const char f_head[32] = {"Are you Xuan Xuan or Han Han?"};
const char f_end[32] = {"We are Xuan Xuan and Han Han."};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QDesktopWidget* desktopWidget = QApplication::desktop();
    //获取可用桌面大小
//    QRect deskRect = desktopWidget->availableGeometry();
    //获取设备屏幕大小
//    QRect screenRect = desktopWidget->screenGeometry();
//    this->setGeometry(0,0,screenRect.size().width(),screenRect.size().height());
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);     //使设计的界面随界面变化而自适应  //无边框显示
    this->setMinimumSize(800,480);
    this->setMaximumSize(800,480);
    //设置窗口背景色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::white);   //设置主界面背景颜色
    this->setPalette(palette);
    this->setParent(parent);
    this->setWindowTitle(QString(tr("Main Windows")));

    ft_lab.setBold(true);
    ft_lab.setPointSize(14);
    ui->lab_time->setFont(ft_lab);
    ui->lab_time->setStyleSheet("color:red");

    QString fir_version;
    ui->lab_pic->setFont(ft_lab);
    ui->lab_pic->setStyleSheet("color:black");
    ui->lab_pic->setAlignment(Qt::AlignCenter);
    fir_version = "应用程序版本";
    fir_version.append("\n");
    fir_version.append(FIRWAREVER);
    ui->lab_pic->setText(fir_version);

    my_detWidget = new Detect_Widget(this);
    my_defWidget = new defense_Widget(this);
    my_localWidget = new location_Widget(this);
    my_netWidget = new Net_Widget(this);
    my_compassWidget = new Compass_widget(this);
    my_Widget = new Widget(this);
    m_process = new QProcess(this);

    my_netWidget->setStyleSheet("background-color:black;");
    my_localWidget->setStyleSheet("background-color:white;");
    my_defWidget->setStyleSheet("background-color:white;");
    my_detWidget->setStyleSheet("background-color:white;");
    my_compassWidget->setStyleSheet("background-color:black;");

    ui->stackedWidget->addWidget(my_Widget);
    ui->stackedWidget->addWidget(my_netWidget);
    ui->stackedWidget->addWidget(my_localWidget);
    ui->stackedWidget->addWidget(my_detWidget);
    ui->stackedWidget->addWidget(my_defWidget);
    ui->stackedWidget->addWidget(my_compassWidget);
    ui->stackedWidget->setStyleSheet("background-color:black;");
    ui->stackedWidget->setCurrentWidget(my_netWidget);

    m_serverInfo.server_ip = UPSERVER_IP;
    m_serverInfo.server_user = UPSERVER_USER;
    m_serverInfo.server_passwd = UPSERVER_PASS;
    m_serverInfo.server_path = UPSERVER_PATH;
    m_upInfo_stc.firware_dir = LOCALSAVE_PATH;

    connect(my_defWidget, &defense_Widget::sn_dwSelectDevice, this, &MainWindow::slot_getDeviceID);
    connect(my_detWidget, &Detect_Widget::sn_dwSelectDevice, this, &MainWindow::slot_getDeviceID);
    connect(my_defWidget, &defense_Widget::sn_dwCencleSelect, this, &MainWindow::slot_cencelProcess);
    connect(my_detWidget, &Detect_Widget::sn_dwCencleSelect, this, &MainWindow::slot_cencelProcess);

    connect(my_compassWidget, &Compass_widget::signal_startCaliData, my_Widget, &Widget::send_Compass_cmd);
    connect(my_compassWidget, &Compass_widget::signal_saveCaliData, my_Widget, &Widget::send_Compass_cmd);

    QUuid uid = QUuid::createUuid();
    QString uid_str = uid.toString();
    qDebug()<<tr("Get Uuid = %1").arg(uid_str)<<endl;

    tim_time = new QTimer(this);
    tim_mapupdate = new QTimer(this);
    tim_stateup = new QTimer(this);
    connect(tim_time,&QTimer::timeout,this,&MainWindow::slot_timeUpdate);
    connect(tim_mapupdate, &QTimer::timeout, this, &MainWindow::slot_mapUpdate);
    connect(tim_stateup, &QTimer::timeout, this, &MainWindow::slot_stateUp);
    tim_time->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::reboot()
{
    QString program = QApplication::applicationFilePath();
    QStringList arguments = QApplication::arguments();
    QString workingDir = QDir::currentPath();
    QProcess::startDetached(program, arguments, workingDir);
    QApplication::exit();
}
//网络按键单击槽函数
void MainWindow::on_pbut_Network_clicked()
{
#ifdef DEBUG_EN
    qDebug()<<tr("The network button is pressed!")<<endl;
#endif
    ui->pbut_Network->icon().Normal;
    ui->pbut_Location->icon().Disabled;
    ui->pbut_Compass->icon().Disabled;
    ui->pbut_Detectors->icon().Disabled;
    ui->pbut_Shields->icon().Disabled;
    ui->stackedWidget->setStyleSheet("background-color:black;");
    ui->stackedWidget->setCurrentWidget(my_netWidget);
    ui->stackedWidget->show();
}
//位置按键单击槽函数
void MainWindow::on_pbut_Location_clicked()
{
#ifdef DEBUG_EN
    qDebug()<<tr("The location button is pressed!")<<endl;
#endif
    ui->pbut_Network->icon().Disabled;
    ui->pbut_Location->icon().Normal;
    ui->pbut_Compass->icon().Disabled;
    ui->pbut_Detectors->icon().Disabled;
    ui->pbut_Shields->icon().Disabled;

    ui->stackedWidget->setStyleSheet("background-color:white;");
    ui->stackedWidget->setCurrentWidget(my_localWidget);
    ui->stackedWidget->show();
    QDir temDir("../map_file/baidumap.html");
    QString absDir = temDir.absolutePath();
    QString filePath = "file:///" + absDir;
    my_localWidget->m_view->page()->load(QUrl(filePath));   //load the map in relevant path
//    if((my_Widget->gps_info.lon == 0.0) || (my_Widget->gps_info.lat == 0.0)) {
//        //qDebug()<<tr("-->>Location Infomation lon = %1, lat = %2").arg(116.400244).arg(39.92556)<<endl;
//        //emit my_localWidget->sn_updatemap(114.266714, 37.594726);
//    }else {
//        emit my_localWidget->sn_updatemap(my_Widget->gps_info.lon, my_Widget->gps_info.lat);
//    }

    tim_mapupdate->start(10000);
}
//方位按键单击槽函数
void MainWindow::on_pbut_Compass_clicked()
{
#ifdef DEBUG_EN
    qDebug()<<tr("The compass button is pressed!")<<endl;
#endif
    ui->pbut_Network->icon().Disabled;
    ui->pbut_Location->icon().Disabled;
    ui->pbut_Compass->icon().Normal;
    ui->pbut_Detectors->icon().Disabled;
    ui->pbut_Shields->icon().Disabled;
    ui->stackedWidget->setStyleSheet("background-color:black;");
    ui->stackedWidget->setCurrentWidget(my_compassWidget);
    ui->stackedWidget->show();

}
//侦测按键单击槽函数
void MainWindow::on_pbut_Detectors_clicked()
{
#ifdef DEBUG_EN
    qDebug()<<tr("The detector button is pressed!")<<endl;
#endif
    ui->pbut_Network->icon().Disabled;
    ui->pbut_Location->icon().Disabled;
    ui->pbut_Compass->icon().Disabled;
    ui->pbut_Detectors->icon().Normal;
    ui->pbut_Shields->icon().Disabled;
    ui->stackedWidget->setStyleSheet("background-color:white;");
    ui->stackedWidget->setCurrentWidget(my_detWidget);
    ui->stackedWidget->show();
}
//防御按键单击槽函数
void MainWindow::on_pbut_Shields_clicked()
{
#ifdef DEBUG_EN
    qDebug()<<tr("The shield button is pressed!")<<endl;
#endif
    ui->pbut_Network->icon().Disabled;
    ui->pbut_Location->icon().Disabled;
    ui->pbut_Compass->icon().Disabled;
    ui->pbut_Detectors->icon().Disabled;
    ui->pbut_Shields->icon().Normal;
    ui->stackedWidget->setStyleSheet("background-color:white;");
    ui->stackedWidget->setCurrentWidget(my_defWidget);
    ui->stackedWidget->show();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
#ifdef DEBUG_EN
    qDebug()<<tr("Slider valueChanged = %1").arg(value)<<endl;
#endif
}
void MainWindow::slot_getDeviceID(quint8 id)
{
    switch (id) {
    case 1:
        my_Widget->device_id = 1;
        my_Widget->wd_Init(id);
        my_defWidget->rbut_1->setEnabled(false);
        my_defWidget->rbut_2->setEnabled(false);
        my_defWidget->rbut_3->setEnabled(false);
        break;
    case 2:
        my_Widget->device_id = 2;
        if(!my_detWidget->pro_ver.isEmpty()) {
            my_Widget->dev_info.comm_pro_ver = my_detWidget->pro_ver;
            qDebug()<<tr("The current connection is to a %1 device")
                      .arg(my_Widget->dev_info.comm_pro_ver)<<endl;
        }
        my_Widget->wd_Init(id);
        my_defWidget->rbut_1->setEnabled(false);
        my_defWidget->rbut_2->setEnabled(false);
        my_defWidget->rbut_3->setEnabled(false);
        break;
    case 3:
        my_Widget->device_id = 3;
        my_Widget->wd_Init(id);
        my_defWidget->rbut_1->setEnabled(false);
        my_defWidget->rbut_2->setEnabled(false);
        my_defWidget->rbut_3->setEnabled(false);
        break;
    case 4:
        my_Widget->device_id = 4;
        if(!my_defWidget->dev_ver.isEmpty()) {
            my_Widget->dev_info.comm_pro_ver = my_defWidget->dev_ver;
            qDebug()<<tr("The current connection is to a %1 device")
                      .arg(my_Widget->dev_info.comm_pro_ver)<<endl;
        }
        my_Widget->wd_Init(id);
        my_detWidget->rbut1->setEnabled(false);
        my_detWidget->rbut2->setEnabled(false);
        my_detWidget->rbut3->setEnabled(false);
        break;
    case 5:
        my_Widget->device_id = 5;
        my_Widget->wd_Init(id);
        my_detWidget->rbut1->setEnabled(false);
        my_detWidget->rbut2->setEnabled(false);
        my_detWidget->rbut3->setEnabled(false);
        break;
    case 6:
        my_Widget->device_id = 6;
        my_Widget->wd_Init(id);
        my_detWidget->rbut1->setEnabled(false);
        my_detWidget->rbut2->setEnabled(false);
        my_detWidget->rbut3->setEnabled(false);
        break;
    default:
        my_Widget->device_id = 0;
        my_defWidget->rbut_1->setEnabled(false);
        my_defWidget->rbut_2->setEnabled(false);
        my_defWidget->rbut_3->setEnabled(false);
        my_detWidget->rbut1->setEnabled(false);
        my_detWidget->rbut2->setEnabled(false);
        my_detWidget->rbut3->setEnabled(false);
        break;
    }

    tim_stateup->start(2000);
}
void MainWindow::slot_timeUpdate()
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_time1 = current_date_time.toString("yyyy-MM-dd ddd");
    QString current_time2 = current_date_time.toString("hh:mm:ss");
    QString txt = current_time1;
    txt += "\n";
    txt += current_time2;
    ui->lab_time->setText(txt);
}
void MainWindow::slot_mapUpdate()
{
    tim_mapupdate->stop();

    if((my_Widget->gps_info.lon == 0.0) || (my_Widget->gps_info.lat == 0.0)) {
//        qDebug()<<tr("-->>Location Infomation lon = %1, lat = %2").arg(116.331398).arg(39.897445)<<endl;
//        emit my_localWidget->sn_updatemap(116.331398,39.897445);
        emit my_localWidget->sn_updatemap(116.235041, 39.545093);
    }else {
#ifdef DEBUG_EN
        qDebug()<<tr("-->>Location Infomation lon = %1, lat = %2").arg(my_Widget->gps_info.lon).arg(my_Widget->gps_info.lat)<<endl;
#endif
        emit my_localWidget->sn_updatemap(my_Widget->gps_info.lon, my_Widget->gps_info.lat);
    }
    //add checket Update Firware file
    //20-8-10
    if(!m_upInfo_stc.up_enable)
        check_upfirware("/home/pi/Downloads/UpdateFile/RK3399/update_info.txt");
    tim_mapupdate->start(10000);
}

void MainWindow::slot_cencelProcess(quint8 index)
{
    switch (index) {
    case 1:
        my_detWidget->pbt1->setEnabled(false);
        my_detWidget->pbt2->setEnabled(true);
        my_detWidget->rbut1->setEnabled(true);
        my_detWidget->rbut2->setEnabled(true);
        my_detWidget->rbut3->setEnabled(true);

        my_defWidget->pbt_1->setEnabled(false);
        my_defWidget->pbt_2->setEnabled(true);
        my_defWidget->rbut_1->setEnabled(true);
        my_defWidget->rbut_2->setEnabled(true);
        my_defWidget->rbut_3->setEnabled(true);
        break;
    case 2:
        my_detWidget->pbt1->setEnabled(false);
        my_detWidget->pbt2->setEnabled(true);
        my_detWidget->rbut1->setEnabled(true);
        my_detWidget->rbut2->setEnabled(true);
        my_detWidget->rbut3->setEnabled(true);

        my_defWidget->pbt_1->setEnabled(false);
        my_defWidget->pbt_2->setEnabled(true);
        my_defWidget->rbut_1->setEnabled(true);
        my_defWidget->rbut_2->setEnabled(true);
        my_defWidget->rbut_3->setEnabled(true);
        break;
    default:
        break;
    }
    my_Widget->close();
}
//
void MainWindow::slot_stateUp()
{
    tim_stateup->stop();
    my_Widget->stat_info.devID.insert(12, "\n");
    switch (my_Widget->device_id) {
    case 1:
        if(my_Widget->stat_info.connDevStat) {
            my_detWidget->lab_conn_icon1->setEnabled(true);
        }else {
            my_detWidget->lab_conn_icon1->setEnabled(false);
        }
        if(my_Widget->stat_info.connServerStat) {
            my_detWidget->lab_datadir1->setEnabled(true);
        }else {
            my_detWidget->lab_datadir1->setEnabled(false);
        }
        if(!my_Widget->stat_info.devIP.isEmpty()) {
            my_detWidget->dev_ip1 = my_Widget->stat_info.devID;
        }else {
            my_detWidget->dev_ip1 = "NULL";
        }
        break;
    case 2:
        if(my_Widget->stat_info.connDevStat) {
            my_detWidget->lab_conn_icon2->setEnabled(true);
        }else {
            my_detWidget->lab_conn_icon2->setEnabled(false);
        }
        if(my_Widget->stat_info.connServerStat) {
            my_detWidget->lab_datadir2->setEnabled(true);
        }else {
            my_detWidget->lab_datadir2->setEnabled(false);
        }
        if(!my_Widget->stat_info.devIP.isEmpty()) {
            my_detWidget->dev_ip2 = my_Widget->stat_info.devID;
        }
        break;
    case 3:
        if(my_Widget->stat_info.connDevStat) {
            my_detWidget->lab_conn_icon3->setEnabled(true);
        }else {
            my_detWidget->lab_conn_icon3->setEnabled(false);
        }
        if(my_Widget->stat_info.connServerStat) {
            my_detWidget->lab_datadir3->setEnabled(true);
        }else {
            my_detWidget->lab_datadir3->setEnabled(false);
        }
        if(!my_Widget->stat_info.devIP.isEmpty()) {
            my_detWidget->dev_ip3 = my_Widget->stat_info.devID;
        }
        break;
    case 4:
        if(my_Widget->stat_info.connDevStat) {
            my_defWidget->l_conn_icon1->setEnabled(true);
        }else {
            my_defWidget->l_conn_icon1->setEnabled(false);
        }
        if(my_Widget->stat_info.connServerStat) {
            my_defWidget->l_datadir1->setEnabled(true);
        }else {
            my_defWidget->l_datadir1->setEnabled(false);
        }
        if(!my_Widget->stat_info.devID.isEmpty()) {
            my_defWidget->dev_id1 = my_Widget->stat_info.devID;
        }
        break;
    case 5:
        if(my_Widget->stat_info.connDevStat) {
            my_defWidget->l_conn_icon2->setEnabled(true);
        }else {
            my_defWidget->l_conn_icon2->setEnabled(false);
        }
        if(my_Widget->stat_info.connServerStat) {
            my_defWidget->l_datadir2->setEnabled(true);
        }else {
            my_defWidget->l_datadir2->setEnabled(true);
        }
        if(!my_Widget->stat_info.devID.isEmpty()) {
            my_defWidget->dev_id2 = my_Widget->stat_info.devID;
        }
        break;
    case 6:
        if(my_Widget->stat_info.connDevStat) {
            my_defWidget->l_conn_icon3->setEnabled(true);
        }else {
            my_defWidget->l_conn_icon3->setEnabled(false);
        }
        if(my_Widget->stat_info.connServerStat) {
            my_defWidget->l_datadir3->setEnabled(true);
        }else {
            my_defWidget->l_datadir3->setEnabled(true);
        }
        if(!my_Widget->stat_info.devID.isEmpty()) {
            my_defWidget->dev_id3 = my_Widget->stat_info.devID;
        }
        break;
    default:
        break;
    }
    if(my_Widget->dev_info.att_info.att_stat) {
        my_compassWidget->tar_angle = my_Widget->dev_info.att_info.dir_angle;
        my_compassWidget->pit_angle = my_Widget->dev_info.att_info.pit_angle;
        my_compassWidget->roll_angle = my_Widget->dev_info.att_info.roll_angle;
    }
    tim_stateup->start(2000);
}
void MainWindow::on_rb_display_debug_clicked(bool checked)
{
    if(checked) {
#ifdef DEBUG_EN
        qDebug()<<tr("The Radio button is pressed!")<<endl;
#endif
        ui->pbut_Network->icon().Disabled;
        ui->pbut_Location->icon().Disabled;
        ui->pbut_Detectors->icon().Disabled;
        ui->pbut_Shields->icon().Disabled;
        ui->stackedWidget->setStyleSheet("background-color:black;");
        ui->stackedWidget->setCurrentWidget(my_Widget);
        ui->stackedWidget->show();
    }else {
        ui->pbut_Network->icon().Normal;
        ui->pbut_Location->icon().Disabled;
        ui->pbut_Detectors->icon().Disabled;
        ui->pbut_Shields->icon().Disabled;
        ui->stackedWidget->setStyleSheet("background-color:black;");
        ui->stackedWidget->setCurrentWidget(my_netWidget);
        ui->stackedWidget->show();
    }
}
//ms延时
bool MainWindow::sleep_ms(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);

    while (QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    return true;
}
//检查固件更新
void MainWindow::check_upfirware(QString path)
{
    QFile mf_upInfo(path);
    if(!mf_upInfo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<tr("Can't Open The Update Infomation File!")<<endl;
        return;
    }
    while(!mf_upInfo.atEnd()) {
        QByteArray line = mf_upInfo.readLine();
        QString str(line);
        if(str.contains("FirmwareName:")) {
            QStringList str_list = str.split(":");
            for(int i = 1; i < str_list.count(); i++)  {
                if(true == str_list[i].contains("RK3399")) {
                    m_upInfo_stc.firware_name = str_list[i].trimmed();
                    qDebug()<<"----------------------"<<endl;
                    qDebug()<<"Update FirmwareName:"<<m_upInfo_stc.firware_name<<endl;                   
                }
            }
        }
        if(str.contains("UpdateFlag:")) {
            QStringList str_list = str.split(":");
            for(int i = 1; i < str_list.count(); i++) {
                if(false == str_list[i].isNull()) {
                    QString flag = str_list[i].trimmed();
                    m_upInfo_stc.up_flag = flag.toInt();
                    qDebug()<<"Update Flag:"<<m_upInfo_stc.up_flag<<endl;
                }
            }
        }
        if(str.contains("FirmwarePath:")) {
            QStringList str_list = str.split(":");
            for(int i = 1; i < str_list.count(); i++) {
                if(false == str_list[i].isNull()) {
                    m_serverInfo.server_path = str_list[i].trimmed();
                    qDebug()<<"Update Firmware Server Path:"<<m_serverInfo.server_path<<endl;
                }
            }
        }
        if(str.contains("FirmwareVer:")) {
            QStringList str_list = str.split(":");
            for(int i = 1; i < str_list.count(); i++) {
                if(false == str_list[i].isNull()) {
                    m_upInfo_stc.firware_ver = str_list[i].trimmed();
                    qDebug()<<tr("Update Firmware Version:%1, Current Firmware Version:%2")
                              .arg(m_upInfo_stc.firware_ver).arg(FIRWAREVER)<<endl;
                    qDebug()<<"----------------------"<<endl;
                }
            }
        }
    }
    //需要更新的版本和当前版本不同
    if((m_upInfo_stc.firware_ver != FIRWAREVER) && (m_upInfo_stc.up_flag == 1)) {
        if(QMessageBox::information(this, tr("Update Tip Information"), tr("The application detected a new firmware.\n Are you sure you want to upgrade?"),
                                 QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes) == QMessageBox::Yes) {
            m_upInfo_stc.up_enable = true;
            qDebug()<<tr("Enter Update Firware.")<<endl;

            My_Dialog *m_dialog = new My_Dialog(this);
            m_dialog->setAttribute(Qt::WA_DeleteOnClose);
            m_dialog->show();

            connect(m_dialog, &My_Dialog::sig_isOk, this, &MainWindow::slot_diaLog_process);
            connect(m_dialog, &My_Dialog::sig_isCencel, this, &MainWindow::slot_diaLog_process);
        }else {
            m_upInfo_stc.up_enable = true;
            qDebug()<<tr("Cancel Update Firware.")<<endl;
        }
    }
}
//
void MainWindow::slot_diaLog_process(int index)
{
    if(index == 1) {
        if(QMessageBox::information(this, tr("Information"), tr("Are you sure you want to upgrade?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes)
                == QMessageBox::Yes) {

            connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(slot_shell_finished(int, QProcess::ExitStatus)));
            QString cmd_pro;
            QString app_path = QString(QDir::currentPath());
            cmd_pro = app_path + "/ftp_downfirmware.sh " +
                    m_serverInfo.server_ip + " " +
                    m_serverInfo.server_user + " " +
                    m_serverInfo.server_passwd + " " +
                    m_upInfo_stc.firware_dir + " " +
                    m_serverInfo.server_path + " " +
                    m_upInfo_stc.firware_name + " " +
                    "112233";
            qDebug()<<tr("Update Firware Cmommand:%1").arg(cmd_pro)<<endl;
            m_process->start(cmd_pro);
            if(!m_process->waitForStarted()) {
                QMessageBox::information(this, tr("Shell Started"), tr("Shell Start Failled."), QMessageBox::Yes, QMessageBox::Yes);
            }else {
                m_message = new QMessageBox(QMessageBox::Information, tr("Firmware update"),
                                            "<font size='20' color='black'>应用程序正在更新中，请耐心等待几分钟...</font>",
                                            QMessageBox::Close,this);
                connect(this, &MainWindow::signal_closeMessage, m_message, &QMessageBox::close);
                connect(this, &MainWindow::signal_uncompress, this, &MainWindow::slot_decrypt_file);
                m_message->exec();
            }
            m_process->waitForFinished(-1);
        }
    }else {
        QMessageBox::information(this, tr("Information"), tr("Are you sure you want to cancel the upgrade?"), QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
    }
}
void MainWindow::slot_shell_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug()<<tr("Shell finished.")<<endl;

    qDebug()<<tr("Shell exitCode = %1, exitStatus = %2").arg(exitCode).arg(exitStatus)<<endl;
    QString ret_str = QString::fromLocal8Bit(m_process->readAll());
    qDebug()<<tr("finished-output-readAll:%1").arg(ret_str)<<endl;
    qDebug()<<tr("finished-output-readAllStandardOutput:%1").arg(QString::fromLocal8Bit(m_process->readAllStandardOutput()));

    if(ret_str.contains("download done!")) {
        m_process->close();
        m_upInfo_stc.up_enable = true;
        if(QMessageBox::information(this, tr("Information"), tr("Congratulations to you! The application update is complete."), QMessageBox::Yes, QMessageBox::Yes) == QMessageBox::Yes)
            emit signal_closeMessage();
    }else {
        m_upInfo_stc.up_enable = false;
    }
    QStringList str_list = m_upInfo_stc.firware_name.split(".");
    QString firware_absname = str_list[0];
    qDebug()<<tr("Unzip File Path = %1, New File Path = %2")
              .arg(m_upInfo_stc.firware_dir+"/"+m_upInfo_stc.firware_name).arg(m_upInfo_stc.firware_dir+"/"+firware_absname+".tar")<<endl;
    emit signal_uncompress(m_upInfo_stc.firware_dir+"/"+m_upInfo_stc.firware_name, m_upInfo_stc.firware_dir+"/"+firware_absname+".tar");
}

void MainWindow::slot_decrypt_file(QString sourFile_path, QString saveFile_path)
{
    if(sourFile_path.isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Error decrypting file path!"),
                                 QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    if(saveFile_path.isEmpty()) {
        QMessageBox::information(this, tr("Information"), tr("Error save firware file path!"),
                                 QMessageBox::Yes, QMessageBox::Yes);
        return;
    }
    QFile file_source(sourFile_path);
    if(!file_source.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Information"), tr("Open Firware File Failed!"),
                                 QMessageBox::Yes, QMessageBox::Yes);
        qDebug()<<tr("Open Firware File Failed!")<<endl;
        return;
    }

    QByteArray tmp_arr = QByteArray::fromBase64(file_source.readAll());
    QString input_head = f_head;
    QString input_end = f_end;
    QByteArray head_hash, end_hash;

    head_hash = QCryptographicHash::hash(input_head.toUtf8(), QCryptographicHash::Md5);
    end_hash = QCryptographicHash::hash(input_end.toUtf8(), QCryptographicHash::Md5);

    md5_head = head_hash.toHex().toUpper();
    md5_end = end_hash.toHex().toUpper();
    file_source.close();

//    QMessageBox::information(NULL, tr("Information"), tr("Header MD5 = %1 \r\n End MD5 = %2").arg(md5_head).arg(md5_end), QMessageBox::Yes, QMessageBox::Yes);
    qDebug()<<"File Header MD5 = "+md5_head<<endl<<"File End MD5 = "+md5_end<<endl;
    if(tmp_arr.contains(md5_head.toUtf8()) && (tmp_arr.contains(md5_end.toUtf8()))) {
        tmp_arr.remove(0, 32);
        tmp_arr.remove(tmp_arr.size()-32, 32);
        qDebug()<<tr("Decrypt file header MD5 check file.")<<endl;
    }else {
        if(QMessageBox::information(this, tr("Information"), tr("No eligible file information was found!"), QMessageBox::Yes, QMessageBox::Yes) == QMessageBox::Yes)
            return;
    }
    QFile file_save(saveFile_path);
    if(!file_save.open(QIODevice::WriteOnly)) {
        QMessageBox::information(this,tr("Information"),tr("Unable to create target file"),
                                                 QMessageBox::Yes, QMessageBox::Yes);
        qDebug()<<tr("Create Save Firware File Failed!")<<endl;
        return;
    }
    file_save.write(tmp_arr);
    file_save.close();

    QProcess *m_unzip_pro = new QProcess(this);
    QFileInfo file_Info(file_save);
    QString unzip_path = file_Info.absoluteFilePath();
    QString app_path = QString(QDir::currentPath());
    QString update_sh = app_path + "/update_app.sh ";
    QDir tar_dir = QDir::currentPath();
    QString target_path;
    if(tar_dir.cdUp()) {
       target_path = tar_dir.path();
    }
    QString comm_unzip = (update_sh + " " + unzip_path + " " + target_path + " pi " + "112233");
    qDebug()<<tr("Uncompress Firware File Command: %1").arg(comm_unzip)<<endl;
    m_unzip_pro->start(comm_unzip);
    if (!m_unzip_pro->waitForStarted(2000))
    {
        qDebug() << "启动失败\n";
        return;
    }
    m_unzip_pro->closeWriteChannel();
    this->close();
}


