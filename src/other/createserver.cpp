#include "createserver.h"
#include "ui_createserver.h"

CreateServer::CreateServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreateServer)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window | Qt::WindowTitleHint
                         | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    QStringList lists=FindIpAddr();
    for(int i=2;i<lists.count();i++){
        ui->comboBox_2->addItem(lists.at(i));
    }
}

CreateServer::~CreateServer()
{
    delete ui;
}



void CreateServer::on_pushButton_2_clicked()
{
    delete(this);
}


void CreateServer::on_pushButton_clicked()
{
    QTcpServer server;
    if(ui->comboBox_2->currentText().isEmpty()||ui->comboBox_3->currentText().isEmpty()){
        QMessageBox::critical(this,"错误","本地地址和端口不能为空");
    }else if(!isIpOk(ui->comboBox_2->currentText())){
        QMessageBox::critical(this,"错误","IP地址不合法");
    }else if(!server.listen(QHostAddress::LocalHost,ui->comboBox_3->currentText().toInt())){
        QMessageBox::critical(this,"错误","端口已被占用");
        server.close();
    }else{
        emit sendSerInfo(ui->comboBox->currentText()+" "+ui->comboBox_2->currentText()+" "
                  +ui->comboBox_3->currentText());
        delete(this);
    }
}

QStringList CreateServer::FindIpAddr()
{
    QStringList ipAddrs;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces) {
        QList<QNetworkAddressEntry> addresses = interface.addressEntries();
            ipAddrs.append(addresses.at(1).ip().toString());
    }
    return ipAddrs;
}

bool CreateServer::isIpOk(QString ip)
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
