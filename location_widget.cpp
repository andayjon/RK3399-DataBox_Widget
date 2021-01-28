#include "location_widget.h"
#define DEBUG_EN

location_Widget::location_Widget(QWidget *parent) : QWidget(parent)
{
    this->setGeometry(0,75,800,280);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color:white");

    QHBoxLayout *hb_layout = new QHBoxLayout(this);
    m_view = new QWebEngineView(this);
//    setCentralWidget(m_view);
    m_view->resize(800, 280);
    hb_layout->addWidget(m_view);
    this->setLayout(hb_layout);

    channel = new QWebChannel(this);        //创建通道对象用于与JS交互
    bridge *mybride=new bridge();           //创建通道对象用于与JS交互
    channel->registerObject("bridge_name",(QObject*)mybride);

    // 这里注册对象名"bridge_name"需要与JS函数用到的名称一致
    //见baidumap.html文件第94行 window.bridge_js = channel.objects.bridge_name;
    // C++对象"bridge_name", JS对象bridge_js。101行，JS调用QT中C++函数只需
    //  bridge_js.getCoordinate(e.point.lng, e.point.lat);网页向QT传递经纬度坐标
    //这就实现了JS调用C++函数，实现网页向QT传递数据
    m_view->page()->setWebChannel(channel);

//    QDir temDir("../map_file/baidumap.html");
//    QString absDir = temDir.absolutePath();
//    QString filePath = "file:///" + absDir;
#ifdef DEBUG_EN
//    qDebug()<<filePath;
#endif
    connect(m_view, &QWebEngineView::loadFinished, this, &location_Widget::loadFinished);
    connect(this, &location_Widget::sn_updatemap, this, &location_Widget::slot_updateLocaltion);
   /* m_view->page()->load(QUrl(filePath));*/   //load the map in relevant path

    gps_bd = new gps2bd();
//    QString command = QString("doLocal(%0,%1)").arg(QString::number(116.404, 'f', 6)).arg(QString::number(39.915, 'f', 6));
//    m_view->page()->runJavaScript(command);

}

location_Widget::~location_Widget()
{
    delete this;
}
//
void location_Widget::on_pushButton_clicked()
{
    QString cmd=QString("addmarker(%0,%1)")
            .arg(lat)
            .arg(lon);
    //把要调用的JS命令当做QString传递给网页
    //对应baidumap.html 第77行
    //这就实现了QT通过C++调用JS函数
    m_view->page()->runJavaScript(cmd);

    //test 2020-11-6

//    Location_st target_loc = ConvertDistanceToLogLat(curr_loc, 0, 1000);
//    QString comm_addmaker = QString("addmarker('%0','%1')").arg(QString::number(target_loc.lon, 'f', 6))
//            .arg(QString::number(target_loc.lat, 'f', 6));
//    m_view->page()->runJavaScript(comm_addmaker);
    //test 2020-11-6
}
//
void location_Widget::loadFinished(bool a)
{
    if (a == true)
    {
        double lon[4], lat[4];
        lon[0] = 121.50866; lon[1] = 121.50863; lon[2] = 121.50899; lon[3] = 121.50902;
        lat[0] = 31.28867; lat[1] = 31.28860; lat[2] = 31.28845; lat[3] = 31.28855;
//        QString command = QString("doLocal('%0','%1')").arg(QString::number(116.400244, 'f', 6)).arg(QString::number(39.92556, 'f', 6));
//        m_view->page()->runJavaScript(command);
    }
}

void location_Widget::slot_updateLocaltion(double lon, double lat)
{
    if((lon != 0.0) && (lat != 0.0)) {
        double tmp_lon = double(quint16(lon)+(lon-(quint16(lon)))*100/60);
        double tmp_lat = double(quint16(lat)+(lat-(quint16(lat)))*100/60);
#ifdef DEBUG_EN
        qDebug()<<tr("-->>location Widget:tmp_lon = %1, tmp_lat = %2").arg(tmp_lon).arg(tmp_lat)<<endl;
#endif
        double location[2];
        gps_bd->wgs2bd(location, tmp_lon, tmp_lat);
        //qDebug()<<tr("-->>location Widget: Conventor Location lon = %1, lat = %2").arg(location[0]).arg(location[1])<<endl;
        QString command = QString("doLocal('%0','%1')").arg(QString::number(tmp_lon, 'f', 6)).arg(QString::number(tmp_lat, 'f', 6));
        m_view->page()->runJavaScript(command);
        //test 2020-11-6
//        curr_loc.lat = tmp_lat;
//        curr_loc.lon = tmp_lon;
//        Location_st target_loc = ConvertDistanceToLogLat(curr_loc, 270, 1000);
//        QString comm_addmaker = QString("addmarker('%0','%1')").arg(QString::number(target_loc.lon, 'f', 6))
//                .arg(QString::number(target_loc.lat, 'f', 6));
//        m_view->page()->runJavaScript(comm_addmaker);
//        qDebug()<<tr("Target Location: lon = %1, lat = %2").arg(QString::number(target_loc.lon, 'f', 6))
//                  .arg(QString::number(target_loc.lat, 'f', 6))<<endl;
        //test 2020-11-6
    }
}
/**************************************************************************
*从指定的原点出发，输入偏移角度后，向此方向延伸输入距离，返回此时的位置
**************************************************************************/
Location_st location_Widget::ConvertDistanceToLogLat(Location_st location, int azimuth, double distance)
{
    Location_st tmp_loc;
    if(location.lat != 0.0 && location.lon != 0.0 && distance > 0) {
        tmp_loc.lon = location.lon+distance*qSin(azimuth*M_PI/180)*180/(M_PI*6371229*qCos(location.lat*M_PI/180));
        tmp_loc.lat = location.lat+distance*qCos(azimuth*M_PI/180)/(M_PI*6371229/180);
    }else {
        tmp_loc.lon = location.lon;
        tmp_loc.lat = location.lat;
    }

    return tmp_loc;
}
