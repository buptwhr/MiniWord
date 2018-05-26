#include "d_save.h"
#include "ui_d_save.h"

d_save::d_save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_save)
{
    ui->setupUi(this);
    this->setWindowTitle("保存");
}

d_save::~d_save()
{
    delete ui;
}

void d_save::lineedit_settext(QString s){
    ui->lineEdit->setText(s);
}

void d_save::on_buttonBox_accepted()
{
    QString filename= ui->lineEdit->text();
    emit sendfilename(filename);
}
