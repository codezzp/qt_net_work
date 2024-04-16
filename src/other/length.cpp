#include "length.h"
#include "ui_length.h"

Length::Length(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Length)
{
    ui->setupUi(this);
}

Length::~Length()
{
    delete ui;
}

void Length::on_pushButton_2_clicked()
{
    delete(this);
}


void Length::on_pushButton_clicked()
{
    emit sendLengthInfo(ui->lineEdit->text());
    delete(this);
}

