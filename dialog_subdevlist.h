#ifndef DIALOG_SUBDEVLIST_H
#define DIALOG_SUBDEVLIST_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>

class dialog_Subdevlist : public QDialog
{
    Q_OBJECT
public:
    explicit dialog_Subdevlist(QWidget *parent = nullptr);
    ~dialog_Subdevlist();
protected:
    QPushButton btn_Enter;
    QPushButton btn_Cencel;
    QRadioButton rbtn_tcp;
    QRadioButton rbtn_http;
    QRadioButton rbtn_zmq;
    QButtonGroup *btn_group;
    QLabel lab_Info;
    int ret;
signals:
    void sig_isOk(int);
    void sig_isCencel(int);

public slots:
    void Btn_Enter_clicked();
    void Btn_Cencel_clicked();
};

#endif // DIALOG_SUBDEVLIST_H
