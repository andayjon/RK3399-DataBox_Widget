#ifndef CARD_DIALOG_H
#define CARD_DIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include <QTimer>
namespace Ui {
class Card_Dialog;
}

class Card_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Card_Dialog(QWidget *parent = nullptr);
    ~Card_Dialog();
    bool m_stat;
    QString m_card1Info;
    QString m_card2Info;
    QString m_card3Info;
private slots:
    void on_pbtn_enter_clicked();

    void on_tabWidget_currentChanged(int index);
    void slot_timerTimeout();
signals:
    void sig_closeDialog(int);

private:
    Ui::Card_Dialog *ui;
    QTimer *m_timer;
};

#endif // CARD_DIALOG_H
