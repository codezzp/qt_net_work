#include "createconnect.h"
#include "ui_createconnect.h"

CreateConnect::CreateConnect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateConnect)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint
                         | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

CreateConnect::~CreateConnect()
{
    delete ui;
}

void CreateConnect::on_pushButton_clicked()
{
    QTcpServer server;
    if((ui->comboBox_2->currentText()=="TCP客户端"||ui->comboBox_2->currentText()=="UDP单播")&&(ui->lineEdit_4->text().isEmpty()||ui->lineEdit_3->text().isEmpty())){
        QMessageBox::critical(this,"错误","目标地址和端口不能为空");
    }else if(!isIpOk(ui->lineEdit_4->text())){
        QMessageBox::critical(this,"错误","目标IP地址不合法");
    }else if(ui->checkBox->isChecked()&&ui->checkBox_2->isChecked()&&(ui->comboBox->currentText().isEmpty()||ui->comboBox_3->currentText().isEmpty())){
        QMessageBox::critical(this,"错误","本地地址和端口不能为空");
    }else if(ui->checkBox->isChecked()&&isIpOk(ui->comboBox->currentText())==false){
        QMessageBox::critical(this,"错误","本地IP地址不合法");
    }else if(ui->checkBox_2->isChecked()&&server.listen(QHostAddress::LocalHost,ui->comboBox_3->currentText().toInt())==false){
        QMessageBox::critical(this,"错误","端口已被占用");
        server.close();
    }else{
        emit sendConInfo(ui->comboBox_2->currentText()+" "+ui->lineEdit_4->text()+" "
                  +ui->lineEdit_3->text()+" "+ui->comboBox->currentText()+" "+ui->comboBox_3->currentText());
        delete(this);
    }
}


void CreateConnect::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->comboBox->setEnabled(true);
        QStringList lists=FindIpAddr();
        for(int i=2;i<lists.count();i++){
            ui->comboBox->addItem(lists.at(i));
        }
    }else{
        ui->comboBox->setEnabled(false);
        ui->comboBox->clear();
    }
}


void CreateConnect::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->comboBox_3->setEnabled(true);
    }else{
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_3->clear();
    }
}


void CreateConnect::on_pushButton_2_clicked()
{
    delete(this);
}

QStringList CreateConnect::FindIpAddr()
{
    QStringList ipAddrs;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces) {
        QList<QNetworkAddressEntry> addresses = interface.addressEntries();
            ipAddrs.append(addresses.at(1).ip().toString());
    }
    return ipAddrs;
}

bool CreateConnect::isIpOk(QString ip)
{
    QStringList list = ip.split('.');
    if (list.size() != 4)
    {
        return false;
    }
    for (const auto& num : list)
    {
        bool ok = false;
        int temp = num.toInt(&ok);
        if (!ok || temp < 0 || temp > 255)
        {
            return false;
        }
    }
    return true;
}

