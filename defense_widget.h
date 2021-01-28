#ifndef DEFENSE_WIDGET_H
#define DEFENSE_WIDGET_H

#include "slecte_dialog.h"
#include <QWidget>
#include <QObject>
#include <QStringLiteral>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDebug>
#include <QTimer>
#include <QFont>
#include <QIcon>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
//防御设备信息界面

class defense_Widget :public QWidget
{
    Q_OBJECT
public:
    explicit defense_Widget(QWidget *parent = nullptr);
    ~defense_Widget();
    void defense_Init();

    QString dev_ver;
    quint8 but_number;
    //按钮
    QPushButton *pbt_1;
    QPushButton *pbt_2;
    //单选
    QRadioButton *rbut_1;
    QRadioButton *rbut_2;
    QRadioButton *rbut_3;
    //连接图标
    QLabel *l_conn_icon1;
    QLabel *l_conn_icon2;
    QLabel *l_conn_icon3;
    //数据方向指示
    QLabel *l_datadir1;
    QLabel *l_datadir2;
    QLabel *l_datadir3;

    QString dev_id1;
    QString dev_id2;
    QString dev_id3;
private:

    int wid_x1; //窗口x1
    int wid_y1; //窗口y1
    quint8 id = 0;
    //设备名称
    QLabel *l_name1;
    QLabel *l_name2;
    QLabel *l_name3;
    //设备图标
    QLabel *l_icon1;
    QLabel *l_icon2;
    QLabel *l_icon3;

    //连接提示
    QLabel *l_conn1;
    QLabel *l_conn2;
    QLabel *l_conn3;

    QButtonGroup *bt_group;
    QTimer *tim_upid;

signals:
    void sn_dwSelectDevice(quint8);
    void sn_dwCencleSelect(quint8);
private slots:
    void on_pbt_1_Clicked();
    void on_pbt_2_Clicked();
    void onButtonClicked(QAbstractButton *);
    void slot_updateid();
    void slot_getversion(int);
};

#endif // DEFENSE_WIDGET_H
