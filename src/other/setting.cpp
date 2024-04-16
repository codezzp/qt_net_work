#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_pushButton_2_clicked()
{
    this->close();
}


void Setting::on_pushButton_clicked()
{
    emit sendSet(ui->lineEdit->text()+"-"+ui->lineEdit_2->text());
    this->close();
}

