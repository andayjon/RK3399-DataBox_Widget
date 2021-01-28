#include "my_dialog.h"

My_Dialog::My_Dialog(QWidget *parent) : QDialog(parent), Btn_Ok(this), Btn_Cencel(this)
{
    lab_info.setText("The application detected a new version.Do you want to update now?");
    //lab_info.move(50,150);
    lab_info.resize(300, 100);


    Btn_Ok.setText("Enter");
    //Btn_Ok.move(50, 100);
    Btn_Ok.resize(100,100);

    Btn_Cencel.setText("Cencel");
    //Btn_Cencel.move(200,100);
    Btn_Cencel.resize(100,100);

    this->resize(400,300);
    this->setWindowTitle("Check Update");

    QHBoxLayout *m_hlayout = new QHBoxLayout;
    m_hlayout->addWidget(&Btn_Cencel);
    m_hlayout->addWidget(&Btn_Ok);

    QVBoxLayout *m_vlayout = new QVBoxLayout;
    m_vlayout->addWidget(&lab_info);
    m_vlayout->addLayout(m_hlayout);

    this->setLayout(m_vlayout);
    connect(&Btn_Ok, &QPushButton::clicked, this, &My_Dialog::Btn_Ok_Clicked);
    connect(&Btn_Cencel, &QPushButton::clicked, this, &My_Dialog::Btn_Cencel_Clicked);
}

My_Dialog::~My_Dialog()
{

}

void My_Dialog::Btn_Ok_Clicked()
{
    qDebug()<<tr("Press the OK key.")<<endl;
    done(Accepted);
    emit sig_isOk(1);
}

void My_Dialog::Btn_Cencel_Clicked()
{
    qDebug()<<tr("Press the Cencel key.")<<endl;
    done(Rejected);
    emit sig_isCencel(0);
}
