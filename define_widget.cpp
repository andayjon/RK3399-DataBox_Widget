#include "define_widget.h"

define_widget::define_widget(QWidget *parent) : QWidget(parent)
{
    QFont font_lab;
    font_lab.setPointSize(16);
    font_lab.setFamily("SimHei");

    lab_Info = new QLabel(this);
    lab_Info->setText(tr("%1 Information:").arg(this->my_strInfo));

    QVBoxLayout *vb_layout = new QVBoxLayout(this);



}

define_widget::~define_widget()
{

}
