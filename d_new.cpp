#include "d_new.h"
#include "ui_d_new.h"

d_new::d_new(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_new)
{
    ui->setupUi(this);
}

d_new::~d_new()
{
    delete ui;
}
