#ifndef SLECTE_DIALOG_H
#define SLECTE_DIALOG_H

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

class Slecte_Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Slecte_Dialog(QWidget *parent = nullptr);
    ~Slecte_Dialog();

protected:
    QPushButton Btn_Enter;
    QPushButton Btn_Cencel;
    QRadioButton rBtn_Old;
    QRadioButton rBtn_New;
    QButtonGroup *Btn_group;
    QLabel lab_Info;
    int ret;
signals:
    void sig_isok(int);
    void sig_isCencel(int);
public slots:
    void Btn_Enter_clicked();
    void Btn_Cencel_clicked();
};

#endif // SLECTE_DIALOG_H
