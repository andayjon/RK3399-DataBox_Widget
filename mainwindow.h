#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "sub_widget.h"
#include "compass_widget.h"
#include "defense_widget.h"
#include "detect_widget.h"
#include "location_widget.h"
#include "net_widget.h"
#include "widget.h"
#include "my_dialog.h"

#include <QMainWindow>
#include <QWidget>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QtCore/QDebug>
#include <QApplication>
#include <QProcess>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPixmap>
#include <QIcon>
#include <QPalette>
#include <QRect>
#include <QFont>
#include <QString>
#include <QDateTime>
#include <QUuid>
//固件版本号 升级固件时需要刷新
#define FIRWAREVER "V1.1.1"
typedef struct {
    QString firware_name;
    QString firware_dir;
    QString firware_ver;
    QDateTime up_time;
    int up_flag;
    bool up_enable = false;
}Update_Info_stc;

typedef struct
{
    QString server_ip;
    QString server_path;
    QString server_user;
    QString server_passwd;
}ServerInfo_stc;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void reboot();
private:
    Ui::MainWindow *ui;

    Widget *my_Widget;
    Detect_Widget *my_detWidget;
    Net_Widget *my_netWidget;
    location_Widget *my_localWidget;
    defense_Widget *my_defWidget;
    Compass_widget *my_compassWidget;

    QTimer *tim_time;
    QTimer *tim_mapupdate;
    QTimer *tim_stateup;
    QFont ft_lab;

    QProcess *m_process;
    QMessageBox *m_message;
    Update_Info_stc m_upInfo_stc;
    ServerInfo_stc m_serverInfo;

    bool butNet_stat;
    bool butLoc_stat;
    bool butDet_stat;
    bool butShi_stat;
    bool sleep_ms(int msec);
    void check_upfirware(QString);

    QString md5_head, md5_end;
signals:
    void signal_closeMessage();
    void signal_uncompress(QString, QString);
public slots:

private slots:
    void on_pbut_Network_clicked();
    void on_pbut_Location_clicked();
    void on_pbut_Detectors_clicked();
    void on_pbut_Shields_clicked();
    void on_pbut_Compass_clicked();
    void on_horizontalSlider_valueChanged(int value);
    void slot_timeUpdate();
    void slot_mapUpdate();
    void slot_stateUp();
    void on_rb_display_debug_clicked(bool checked);
    void slot_getDeviceID(quint8);
    void slot_cencelProcess(quint8);
    void slot_diaLog_process(int index);
    void slot_shell_finished(int, QProcess::ExitStatus);
    void slot_decrypt_file(QString, QString);
};

#endif // MAINWINDOW_H
