#ifndef COMPASS_WIDGET_H
#define COMPASS_WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFont>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include "compass.h"
#include "compass_dialog.h"

class Compass_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Compass_widget(QWidget *parent = nullptr);
    ~Compass_widget();
    Compass *m_compass;
    QLabel *m_lab_angle;
    double tar_angle = 0.0;
    double pit_angle = 0.0;
    double roll_angle = 0.0;
private:
    int wid_x1;
    int wid_y1;
    double angle = 0.0;
    QPushButton *m_pb1;
    QTimer *m_timer;
signals:
    void signal_saveCaliData(quint8);     //保存校准数据
    void signal_startCaliData(quint8);    //开启校准
public slots:
    void slot_timerOut();
private slots:
    void on_m_pb1_clicked();
    void slot_CompCaliProcess(int);
};

#endif // COMPASS_WIDGET_H
