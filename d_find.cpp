#include "d_find.h"
#include "ui_d_find.h"

d_find::d_find(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_find)
{
    ui->setupUi(this);
}

d_find::~d_find()
{
    delete ui;
}
