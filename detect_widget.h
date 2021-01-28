#ifndef DETECT_WIDGET_H
#define DETECT_WIDGET_H

#include "slider_button.h"
#include "dialog_subdevlist.h"
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDebug>
#include <QFont>
#include <QIcon>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>

//侦测设备信息界面
class Detect_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Detect_Widget(QWidget *parent = nullptr);
    ~Detect_Widget();
    void detect_Init();
    QString pro_ver;
    //按钮
    QPushButton *pbt1;
    QPushButton *pbt2;
    //单选
    QRadioButton *rbut1;
    QRadioButton *rbut2;
    QRadioButton *rbut3;
    //连接图标
    QLabel *lab_conn_icon1;
    QLabel *lab_conn_icon2;
    QLabel *lab_conn_icon3;
    //数据方向指示
    QLabel *lab_datadir1;
    QLabel *lab_datadir2;
    QLabel *lab_datadir3;
    //设备IP
    QString dev_ip1;
    QString dev_ip2;
    QString dev_ip3;

protected:

private:
    int wid_x1;
    int wid_y1;
    quint8 id = 0;
    //设备名称
    QLabel *lab_name1;
    QLabel *lab_name2;
    QLabel *lab_name3;
    //设备图标
    QLabel *lab_icon1;
    QLabel *lab_icon2;
    QLabel *lab_icon3;

    //连接提示
    QLabel *lab_conn1;
    QLabel *lab_conn2;
    QLabel *lab_conn3;

    QButtonGroup *bt_group;
    QTimer *tim_upip;
signals:
    void sn_dwSelectDevice(quint8);
    void sn_dwCencleSelect(quint8);
private slots:
    void on_pbt1_Clicked();
    void on_pbt2_Clicked();
    void onButtonClicked(QAbstractButton *);
    void slot_updateip();
    void slot_getProver(int);
};

#endif // DETECT_WIDGET_H
