#include "compass_widget.h"

Compass_widget::Compass_widget(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(0, 75, 800, 280);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:black");
    wid_x1 = this->x();
    wid_y1 = this->y();

    m_timer = new QTimer(this);
    m_compass = new Compass(this);
    m_compass->setMinimumSize(220, 220);
//    m_compass->setGeometry(250, 75, 550, 375);
//    m_compass->resize(200, 200);
    QFont ft_pb;
    ft_pb.setFamily("黑体");
    ft_pb.setPointSize(14);

    m_pb1 = new QPushButton(this);
//    m_pb1->resize(60, 35);
    m_pb1->setMinimumSize(300,35);
    m_pb1->setText("校  准");
    m_pb1->setStyleSheet("QPushButton{border-style: none; background-color: silver; font:bold 28px;}"
                        "QPushButton:hover{background-color:gray;color: white;}"
                        "QPushButton:pressed{background-color:gray; border-style: inset; }");
    //m_pb1->setGeometry(200, 30, 80, 80);
    m_lab_angle = new QLabel(this);
//    m_lab_angle->resize(60,35);
//    m_lab_angle->setMinimumSize(200,35);
    m_lab_angle->setFont(ft_pb);
    m_lab_angle->setText("正北夹角:" + QString::number(angle, 'f', 0) + "°" + "  " +"俯仰角:" + QString::number(pit_angle, 'f', 0));
    m_lab_angle->setAlignment(Qt::AlignCenter);
    m_lab_angle->setStyleSheet("color:white");

    QVBoxLayout *m_vbLayout = new QVBoxLayout(this);

    //QHBoxLayout *m_hbLayout1 = new QHBoxLayout(this);
    //QHBoxLayout *m_hbLayout2 = new QHBoxLayout(this);
    //QHBoxLayout *m_hbLayout3 = new QHBoxLayout(this);
    m_vbLayout->addWidget(m_pb1);
    m_vbLayout->addWidget(m_lab_angle);
    m_vbLayout->addWidget(m_compass);
//    m_hbLayout1->addWidget(m_pb1);
//    m_hbLayout2->addWidget(m_compass);
//    m_hbLayout3->addWidget(m_lab_angle);
//    m_vbLayout->addLayout(m_hbLayout1);
//    m_vbLayout->addLayout(m_hbLayout2);
//    m_vbLayout->addLayout(m_hbLayout3);
    this->setLayout(m_vbLayout);

    connect(m_timer, &QTimer::timeout, this, &Compass_widget::slot_timerOut);
    connect(m_pb1, &QPushButton::clicked, this, &Compass_widget::on_m_pb1_clicked);
    m_timer->start(100);
}

Compass_widget::~Compass_widget()
{
    delete this;
}

void Compass_widget::slot_timerOut()
{
    m_timer->stop();
    if(tar_angle >= 360.0) {
        tar_angle -= 360.0;
    }
    if(angle < tar_angle)
        angle += 1;
    else if(angle > tar_angle)
        angle -= 1;
    else
        ;
    m_lab_angle->setText("正北夹角:" + QString::number(angle, 'f', 0) + "°" + "  " + "俯仰角:" + QString::number(pit_angle, 'f', 0) + "°");
#ifdef DEBUG_EN
    qDebug()<<tr("--->>Commpass Module: HorAngle = %1, PitAngle = %2, RollAngle = %3")
             .arg(angle).arg(pit_angle).arg(roll_angle)<<endl;
#endif
    emit m_compass->disp_angle(angle);
    m_timer->start(100);
}

void Compass_widget::on_m_pb1_clicked()
{
    Compass_Dialog *m_compdialog = new Compass_Dialog(this);
    m_compdialog->setAttribute(Qt::WA_DeleteOnClose);
    m_compdialog->show();

    connect(m_compdialog, &Compass_Dialog::signal_CompassCali, this, &Compass_widget::slot_CompCaliProcess);
//    if(QMessageBox::information(this, tr("Information"), "<font size='10' color='white'>请将罗盘绕X轴或者Y轴进行旋转，旋转过程可以采用慢速并近匀速旋转，\n"
//                                "绕每个轴旋转2-3圈，旋转一周的时间约为15秒.\n 校准完成后请保存校准数据！</font>",
//                                QMessageBox::Cancel|QMessageBox::Save, QMessageBox::Save) == QMessageBox::Save)
//    {
//        //保存校准数据
//    }
}
//校准方式选择处理
void Compass_widget::slot_CompCaliProcess(int index)
{
    QMessageBox *m_meg = new QMessageBox(this);
    m_meg->setStyleSheet("background-color:white; font:bold 16px;");
    QPushButton *btn_cancel = m_meg->addButton("取消", QMessageBox::RejectRole);
    QPushButton *btn_save = m_meg->addButton("保存", QMessageBox::AcceptRole);
    btn_save->setChecked(true);
    switch (index) {
    case 1:
        //水平面校准
        emit signal_startCaliData(1);
        m_meg->setWindowTitle("水平面校准");
        m_meg->setText("请将设备绕水平轴或者垂直轴进行旋转，旋转过程可以采用慢速并近匀速旋转,"
                       "绕每个轴旋转2-3圈,旋转一周的时间约为15秒.\n 校准完成后请保存校准数据!");
        if(1 == m_meg->exec())
            emit signal_saveCaliData(3);  //保存水平面数据
        else
            return;
        break;
    case 2:
        //多面校准
        emit signal_startCaliData(2);
        m_meg->setWindowTitle("多面校准");
        m_meg->setText("请将设备绕三条轴进行旋转，旋转过程可以采用慢速并近匀速旋转,"
                       "绕每个轴旋转2-3圈,旋转一周的时间约为15秒.\n 校准完成后请保存校准数据!");
        if(1 == m_meg->exec())
            emit signal_saveCaliData(4);  //保存多面数据
        else
            return;
        break;
    default:
        break;
    }
}
