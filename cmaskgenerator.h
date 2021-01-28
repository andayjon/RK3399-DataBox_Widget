#ifndef CMASKGENERATOR_H
#define CMASKGENERATOR_H
#include <QtWebSockets>
#include <QWebSocket>
#include <QMaskGenerator>

class CMaskGenerator:public QMaskGenerator
{
    Q_DISABLE_COPY(CMaskGenerator)
public:
    CMaskGenerator(QObject *parent = Q_NULLPTR) :QMaskGenerator(parent)
    {

    }
    virtual quint32 nextMask()
    {
        quint32 val = quint32((double(qrand()) / RAND_MAX) * std::numeric_limits<quint32>::max());
        while (val == 0)
        {
        seed();
        val = quint32((double(qrand()) / RAND_MAX) * std::numeric_limits<quint32>::max());
        }
        return val;
    }
    virtual bool seed()
    {
        qsrand(QDateTime::currentDateTime().toTime_t());
        return true;
    }
    private:
};

#endif // CMASKGENERATOR_H
