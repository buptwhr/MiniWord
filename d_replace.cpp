#include "d_replace.h"
#include "ui_d_replace.h"


d_replace::d_replace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_replace)
{
    ui->setupUi(this);
}

d_replace::~d_replace()
{
    delete ui;
}

void d_replace::on_pushButton_clicked()
{
    QString s1,s2;
    s1=ui->lineEdit->text();
    s2=ui->lineEdit_2->text();
    emit send_replace_message(s1,s2);
}

void d_replace::on_pushButton_2_clicked()
{
    this->close();

}
