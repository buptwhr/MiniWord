#include "d_exit.h"
#include "ui_d_exit.h"

d_exit::d_exit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_exit)
{
    ui->setupUi(this);
}

d_exit::~d_exit()
{
    delete ui;
}
