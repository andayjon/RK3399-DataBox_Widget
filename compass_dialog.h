#ifndef COMPASS_DIALOG_H
#define COMPASS_DIALOG_H

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

class Compass_Dialog : public QDialog
{
    Q_OBJECT
public:
    explicit Compass_Dialog(QWidget *parent = nullptr);
    ~Compass_Dialog();

protected:
    QPushButton m_pbEnter;
    QPushButton m_pbCencel;
    QRadioButton m_rbHorCali;
    QRadioButton m_rbMulitCali;
    QButtonGroup *m_pbGroup;
    QLabel m_labInfo;
    int ret;
signals:
    void signal_CompassCali(int);

public slots:
    void on_pbEnter_clicked();
    void on_pbCencel_clicked();
};

#endif // COMPASS_DIALOG_H
