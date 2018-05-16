#include "d_save.h"
#include "ui_d_save.h"

d_save::d_save(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_save)
{
    ui->setupUi(this);
}

d_save::~d_save()
{
    delete ui;
}
