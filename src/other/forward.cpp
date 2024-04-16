#include "forward.h"
#include "ui_forward.h"
#include <QInputDialog>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>

Forward::Forward(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Forward)
{
    ui->setupUi(this);
    getCon();
    ui->tableWidget->setColumnCount(2);
    QStringList headers;
    headers << "发送类型" << "目标地址";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

Forward::~Forward()
{
    if(table!=NULL){
        delete table;
        table=NULL;
    }
    delete ui;
}


void Forward::on_pushButton_clicked()
{
    type.sendLOcations.clear();
    type.select=-1;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        struct type Type;
        Type.sendType=ui->tableWidget->item(i,0)->text();
        Type.ipAddr=ui->tableWidget->item(i,1)->text();
        type.sendLOcations.append(Type);
    }
    if(ui->radioButton->isChecked()){
        type.select=1;
    }else{
        type.select=2;
    }
    emit sendForward(type);
    this->close();
}


void Forward::on_pushButton_2_clicked()
{
    this->close();
}


void Forward::on_pushButton_5_clicked()
{
    bool ok;
    QString ModelStr = QInputDialog::getText(this, tr("自定义模板"), tr("创建模板："), QLineEdit::Normal,tr(""),&ok);
    if(ok){
        ui->comboBox_2->addItem(ModelStr);
    }
}

void Forward::getCon()
{
    QList<QHostAddress> ipAddresses;
    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, interfaces) {
        if (interface.flags().testFlag(QNetworkInterface::IsLoopBack) || !interface.flags().testFlag(QNetworkInterface::IsRunning))
            continue;
        QList<QNetworkAddressEntry> addressEntries = interface.addressEntries();
        foreach (QNetworkAddressEntry entry, addressEntries) {
            if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol || entry.ip().isLinkLocal())
                continue;
            ipAddresses.append(entry.ip());
            }
        }
    foreach (QHostAddress address, ipAddresses) {
        ui->comboBox->addItem(address.toString()+":9003");
    }
}


void Forward::on_pushButton_3_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowCount + 1);
    QTableWidgetItem *item1 = new QTableWidgetItem(ui->comboBox_2->currentText());
    QTableWidgetItem *item2 = new QTableWidgetItem(ui->comboBox->currentText());
    ui->tableWidget->setItem(rowCount, 0, item1);
    ui->tableWidget->setItem(rowCount, 1, item2);
}


void Forward::on_pushButton_4_clicked()
{
    int rowCount=ui->tableWidget->rowCount();
    if(ui->tableWidget->rowCount()!=0){
        ui->tableWidget->removeRow(rowCount-1);
    }
}




void Forward::on_radioButton_2_clicked()
{
    if(table==NULL){
        table=new templateTables();
        connect(table,SIGNAL(sendCheckId(int)),this,SLOT(recvCheckTemplate(int)));

    }
    table->show();
    emit openTemTable();
    ui->radioButton_2->setChecked(false);
    ui->radioButton->setChecked(true);
}

void Forward::recvCheckTemplate(int id)
{
    if(table!=NULL){
        delete table;
        table=NULL;
    }
    type.templateId=id;
    if(type.templateId!=-1){
        ui->radioButton_2->setChecked(true);
        ui->radioButton->setChecked(false);
    }
}

