#include "gps2bd.h"
#include <QtMath>

static double pi = 3.14159265358979324;
static double a = 6378245.0;
static double ee = 0.00669342162296594323;
static double x_pi = 3.14159265358979324 * 3000.0 / 180.0;

gps2bd::gps2bd()
{

}
//transformLon
double gps2bd::transformLon(double lat, double lon)
{
    double ret = 300.0 + lat + 2.0 * lon + 0.1 * lat * lat + 0.1 * lat * lon + 0.1 * qSqrt(qFabs(lat));
    ret += (20.0 * qSin(6.0 * lat * pi) + 20.0 * qSin(2.0 * lat * pi)) * 2.0 / 3.0;
    ret += (20.0 * qSin(lat * pi) + 40.0 * qSin(lat / 3.0 * pi)) * 2.0 / 3.0;
    ret += (150.0 * qSin(lat / 12.0 * pi) + 300.0 * qSin(lat / 30.0 * pi)) * 2.0 / 3.0;
    return ret;
}

//transformLat
double gps2bd::transformLat(double lat, double lon)
{
    double ret = -100.0 + 2.0 * lat + 3.0 * lon + 0.2 * lon * lon + 0.1 * lat * lon + 0.2 * qSqrt(qFabs(lat));
    ret += (20.0 * qSin(6.0 * lat * pi) + 20.0 * qSin(2.0 * lat * pi)) * 2.0 / 3.0;
    ret += (20.0 * qSin(lon * pi) + 40.0 * qSin(lon / 3.0 * pi)) * 2.0 / 3.0;
    ret += (160.0 * qSin(lon / 12.0 * pi) + 320 * qSin(lon * pi / 30.0)) * 2.0 / 3.0;
    return ret;
}

//wgs2gcj
void gps2bd::wgs2gcj(double *result, double lon, double lat)
{
    double dLat = transformLat(lon - 105.0, lat - 35.0);
    double dLon = transformLon(lon - 105.0, lat - 35.0);
    double radLat = lat / 180.0 * pi;

    double magic = qSin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtMagic = qSqrt(magic);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
    dLon = (dLon * 180.0) / (a / sqrtMagic * qCos(radLat) * pi);
    double mgLat = lat + dLat;
    double mgLon = lon + dLon;

    result[0] = mgLon;
    result[1] = mgLat;
}

void gps2bd::gcj2bd(double *result, double lon, double lat)
{
    double x = lon, y = lat;
    double z = qSqrt(x * x + y * y) + 0.00002 * qSin(y * x_pi);
    double theta = qAtan2(y, x) + 0.000003 * qCos(x * x_pi);
    double bd_lon = z * qCos(theta) + 0.0065;
    double bd_lat = z * qSin(theta) + 0.006;

    result[0] = bd_lon;
    result[1] = bd_lat;
}

void gps2bd::wgs2bd(double *result, double lon, double lat)
{
    double gcj[2];
    wgs2gcj(gcj, lon, lat);
    gcj2bd(result, gcj[0], gcj[1]);
}

void gps2bd::bd2gcj(double *result, double lon, double lat)
{
    double x = lon - 0.0065, y = lat - 0.006;
    double z = qSqrt(x * x + y * y) - 0.00002 * qSin(y * x_pi);
    double theta = qAtan2(y, x) - 0.000003 * qCos(x * x_pi);
    double gg_lon = z * qCos(theta);
    double gg_lat = z * qSin(theta);

    result[0] = gg_lon;
    result[1] = gg_lat;
    //return new double[] { gg_lat, gg_lon };
}
