#ifndef NET_DIALOG_H
#define NET_DIALOG_H

#include <QObject>
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QLayout>
#include <QFont>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTabWidget>


class net_dialog : public QDialog
{
    Q_OBJECT
public:
    explicit net_dialog(QWidget *parent = nullptr);
    ~net_dialog();

    int net_number;
    QTabWidget *tab_widget;
    int tab_num;    //tabWidget tabel number
    QWidget *tab_Card1widget;
    QWidget *tab_Card2widget;
    QWidget *tab_Card3widget;

    QPlainTextEdit *pt_Edit1;
    QPlainTextEdit *pt_Edit2;
    QPlainTextEdit *pt_Edit3;
signals:
    void sign_quitFlag(int);
public slots:
    void disp_card_info(QByteArray*);
    void Btn_Close_Clicked();
protected:
    QPushButton Btn_Close;
private:
    void setTabWidgetStyleSheet(QTabWidget* obj);

};

#endif // NET_DIALOG_H
