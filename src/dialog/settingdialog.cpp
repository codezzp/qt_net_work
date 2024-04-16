#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    setWindowTitle("跳变设置");
    setWindowIcon(QIcon(":/view.png"));
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
    {
        ui->checkBox_2->setChecked(false);
        ui->lineEdit->setDisabled(false);
        ui->lineEdit_2->setDisabled(false);
        isappointdata = false;
    }
    else
    {
        ui->lineEdit->setDisabled(true);
        ui->lineEdit_2->setDisabled(true);
    }
}


void SettingDialog::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked())
    {
        ui->checkBox->setChecked(false);
        isappointdata = true;
    }
}


void SettingDialog::on_checkBox_3_clicked()
{
    if(ui->checkBox_3->isChecked())
    {
        ui->checkBox_4->setChecked(false);
        isscancurrentdata = true;
    }
}


void SettingDialog::on_checkBox_4_clicked()
{
    if(ui->checkBox_4->isChecked())
    {
        ui->checkBox_3->setChecked(false);
        isscancurrentdata = false;
    }
}


void SettingDialog::on_pushButton_3_clicked()
{
    if(isappointdata == true)
    {
        if(ui->tableWidget->rowCount() != 0)
        {
            QVector<double> datagroup;
            for(int i=0;i<ui->tableWidget->rowCount();i++)
            {
                double data =ui->tableWidget->item(i,0)->text().toDouble();
                datagroup.append(data);
            }
            emit Signal_Hopdata(isscancurrentdata,isappointdata,datagroup);
        }
        else
        {
            //如果没有设置任何指定数据,则转为范围数据
            isappointdata = false;
            ui->checkBox->setChecked(true);
            ui->checkBox_2->setChecked(false);
        }
    }
    else
    {
        double minval = ui->lineEdit->text().toDouble();
        double maxval = ui->lineEdit_2->text().toDouble();
        emit Signal_Hopdata(isscancurrentdata,isappointdata,maxval,minval);
    }
    close();
}


void SettingDialog::on_pushButton_4_clicked()
{
    close();
}


void SettingDialog::on_pushButton_clicked()
{
    QTableWidgetItem *it = new QTableWidgetItem;
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-1,0,it);
}


void SettingDialog::on_pushButton_2_clicked()
{
    if(ui->tableWidget->rowCount() !=0)
    {
        QTableWidgetItem * it = ui->tableWidget->currentItem();
        ui->tableWidget->removeRow(it->row());
    }
}

