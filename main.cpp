#include "mainwindow.h"

#include "local_cmdinterface.h"
#include <QObject>
#include <QLabel>
#include <QApplication>
#include <QDesktopWidget>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPalette>
#include <QRect>
#include <QFont>
#include <QString>
#include <QSplashScreen>
#include <QLabel>
#include <QPixmap>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QMovie>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVideoWidget>

#define LOG_ENABLE
//#define MEDIAPLAY_EN
#define PICPLAY_EN

QString file_name;
//写日志到文件
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    QString text;
    bool offlimit_stat = false;
    switch (type) {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
        break;
    }
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(text).arg(context_info).arg(msg).arg(current_date);

    QFile file(file_name);
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream<<message<<"\r\n";
    file.flush();
    //检测日志文件超过10M清理 ~10M/H
    if(file.size() > 1024*1024*5) {    //10485760--10M
        offlimit_stat = true;
    }
    file.close();
    if(offlimit_stat) {
        QFile file(file_name);
        if(file.exists()) {
            file.remove();
        }
    }
    mutex.unlock();
}
int main(int argc, char *argv[])
{
    int time_delay;
    QApplication a(argc, argv);
#ifdef LOG_ENABLE
    QString curr_date_time =  QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    file_name = QString("%1%2%3%4").arg("../").arg("log_").arg(curr_date_time).arg
            (".txt");
    //打印日志到文件
    qInstallMessageHandler(outputMessage);
#endif

#ifdef PICPLAY_EN
    QSplashScreen splash(QPixmap(":/Icon/welcome_interface.jpg"));
    QMovie mv("/home/pi/movie/soho1.mp4");
    splash.resize(800,480);

    //显示启动动画
//    QLabel lbl(&splash);
//    lbl.setFixedSize(800,480);
//    lbl.resize(800,480);
//    lbl.setMovie(&mv);
//    mv.start();
    splash.show();
    QFile f_num("./number.bat");
    if(!f_num.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString str_info;
        str_info.append("Can't open ");
        str_info.append(f_num.fileName());
        str_info.append(" file!");
        qDebug()<<str_info<<endl;
        return 0;
    }
    while(!f_num.atEnd()) {
        //qDebug()<<"open the file successful!"<<endl;
        QByteArray line = f_num.readLine();
        QString str(line);
        if(str.toInt() > 0) {
            time_delay = str.toInt()*10;

        }else {
            time_delay = 5;
        }
        qDebug()<<"Read Logo Display time = "<<time_delay<<"S."<<endl;
//        qDebug()<<"-->>Main Function:Read Number = "+str<<endl;
    }
    QDateTime pre = QDateTime::currentDateTime();
    QDateTime now;
    do
    {
          now = QDateTime::currentDateTime();

          a.processEvents();
    } while (pre.secsTo(now) <= time_delay);     //time_delay为需要延时的秒数
    //停止启动动画
    //mv.stop();
    //splash.close();
#endif
#ifdef MEDIAPLAY_EN
    QMediaPlayer *m_player;
    QMediaPlaylist *m_playlist;
    QVideoWidget *m_video_wdg;

    m_player = new QMediaPlayer();
    m_playlist = new QMediaPlaylist();
    m_video_wdg = new QVideoWidget();

    m_video_wdg->resize(800, 480);
    //m_player->setMedia(QUrl::fromLocalFile("/home/zld/movie/soho1.mp4"));
    m_playlist->addMedia(QUrl::fromLocalFile("/home/pi/movie/soho.mp4"));
    m_playlist->addMedia(QUrl::fromLocalFile("/home/pi/movie/soho.wmv"));
    m_playlist->addMedia(QUrl::fromLocalFile("/home/pi/movie/soho.wmv"));
    m_playlist->setCurrentIndex(1);
    m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    m_player->setPlaylist(m_playlist);
//    video_wdg->setFullScreen(true);
    m_video_wdg->setWindowFlag(Qt::FramelessWindowHint);
    m_video_wdg->show();
    m_player->setVideoOutput(m_video_wdg);
    m_player->play();
    QDateTime pre = QDateTime::currentDateTime();
    QDateTime now;
    do
    {
          now = QDateTime::currentDateTime();
          QApplication::processEvents();
    } while (pre.secsTo(now) <= 20);

    m_player->stop();
    m_player->setMedia(QUrl::fromLocalFile("/home/pi/movie/soho2.mp4"));
    m_player->play();
    m_player->stop();
    m_video_wdg->setWindowFlag(Qt::SubWindow);
    m_video_wdg->showNormal();
    m_video_wdg->close();
    m_video_wdg->deleteLater();
#endif
    Local_CmdInterface *m_localCmdInterface = new Local_CmdInterface();
    MainWindow win;
    win.setWindowFlag(Qt::WindowStaysOnTopHint);
    win.show();
    splash.close();
    //win.showMaximized();    //界面最大

    return a.exec();
}
