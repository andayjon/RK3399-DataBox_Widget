#ifndef GPS2BD_H
#define GPS2BD_H

#include <QObject>

/*
 WGS-84：是国际标准，GPS坐标（Google Earth使用、或者GPS模块）
 GCJ-02：中国坐标偏移标准，Google Map、高德、腾讯使用
 BD-09：百度坐标偏移标准，Baidu Map使用

代码参考：http://bbs.lbsyun.baidu.com/forum.php?mod=viewthread&tid=10923
 */

class gps2bd
{
public:
    gps2bd();

    static void wgs2gcj(double *result, double lat, double lon);
    static void gcj2bd(double *result, double lat, double lon);
    static void wgs2bd(double *result, double lat, double lon);
    static void bd2gcj(double *result, double lat, double lon);

private:
    static double transformLon(double lat, double lon);
    static double transformLat(double lat, double lon);

};

#endif // GPS2BD_H
