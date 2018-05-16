#include "dialogopen.h"
#include "ui_dialogopen.h"

DialogOpen::DialogOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogOpen)
{
    ui->setupUi(this);
}

DialogOpen::~DialogOpen()
{
    delete ui;
}

void DialogOpen::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    ui->lineEdit->setText(fileName);
}


void DialogOpen::on_buttonBox_accepted()
{
    QString filename;
    filename=ui->lineEdit->text();
    emit sendfilename(filename);
}
