#ifndef MY_DIALOG_H
#define MY_DIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QLayout>
#include <QSpacerItem>
#include <QHBoxLayout>
#include <QVBoxLayout>


class My_Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit My_Dialog(QWidget *parent = nullptr);
    ~My_Dialog();

protected:
    QPushButton Btn_Ok;
    QPushButton Btn_Cencel;
    QLabel lab_info;
signals:
    void sig_isOk(int);
    void sig_isCencel(int);
public slots:
    void Btn_Ok_Clicked();
    void Btn_Cencel_Clicked();
};

#endif // MY_DIALOG_H
