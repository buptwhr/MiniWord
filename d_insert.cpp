#include "d_insert.h"
#include "ui_d_insert.h"

d_insert::d_insert(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::d_insert)
{
    ui->setupUi(this);
    this->setWindowTitle("插入");
}

d_insert::~d_insert()
{
    delete ui;
}
