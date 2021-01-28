#ifndef LOCATION_WIDGET_H
#define LOCATION_WIDGET_H

#include "bridge.h"
#include "gps2bd.h"
#include <math.h>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QUrl>

#include <QDir>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QFont>
#include <QIcon>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngine>
#include <QWebChannel>
#include <QtMath>
typedef struct
{
    double lon; //jingdu
    double lat; //weidu
}Location_st;

class location_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit location_Widget(QWidget *parent = nullptr);
    ~location_Widget();
    QWebEngineView *m_view;
    QWebChannel *channel;
    Location_st ConvertDistanceToLogLat(Location_st location, int azimuth, double distance);
protected:
    double lon;
    double lat;
private:
    QLabel *lab_lon;     //jingdu
    QLabel *lab_lat;     //weidu
    gps2bd *gps_bd;
    Location_st curr_loc;
signals:
    void sn_updatemap(double, double);
private slots:
    void on_pushButton_clicked();
    void loadFinished(bool);
    void slot_updateLocaltion(double, double);
};

#endif // LOCATION_WIDGET_H
