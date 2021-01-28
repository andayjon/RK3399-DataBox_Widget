#ifndef DEFINE_WIDGET_H
#define DEFINE_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QFont>
#include <QLayout>
#include <QObject>
#include <QSpacerItem>

class define_widget : public QWidget
{
    Q_OBJECT
public:
    explicit define_widget(QWidget *parent = nullptr);
    ~define_widget();

    QPlainTextEdit *pt_Edit_Info;
    QString my_strInfo;
signals:

public slots:

protected:

private:
    QLabel *lab_Info;
};

#endif // DEFINE_WIDGET_H
