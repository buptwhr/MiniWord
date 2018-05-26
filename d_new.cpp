#include "d_new.h"
#include "ui_d_new.h"

d_new::d_new(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_new)
{
    ui->setupUi(this);
    this->setWindowTitle("新建");
}

d_new::~d_new()
{
    delete ui;
}

void d_new::on_buttonBox_accepted()
{
    QString filename= ui->lineEdit->text();
    emit sendfilename(filename);
}
