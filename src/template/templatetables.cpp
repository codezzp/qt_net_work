#include "templatetables.h"
#include "ui_templatetables.h"
#include <QFile>
#include <QDomElement>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QRadioButton>
#include <QLineEdit>
#include <QTableWidget>

templateTables::templateTables(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::templateTables)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(4);
    tems=new templates();
    QStringList headers;
    headers << "编号" << "名称" << "定义" << "选择";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    AllReadTemplates();
}

templateTables::~templateTables()
{
    delete ui;
}

void templateTables::AllReadTemplates()
{
    int rowCount=ui->tableWidget->rowCount();
    while(rowCount>0){
        ui->tableWidget->removeRow(rowCount-1);
        rowCount=ui->tableWidget->rowCount();
    }
    QFile xmlFile("./favorites/templates.xml");
    if (!xmlFile.open(QIODevice::ReadOnly)) {
        return;
    }
    QDomDocument xmlDoc;
    if (!xmlDoc.setContent(&xmlFile)) {
        return;
    }
    xmlFile.close();
    QDomElement root = xmlDoc.documentElement();
    QDomNodeList templateNodes = root.elementsByTagName("template");
    for(int i=0;i<templateNodes.count();i++){
        QDomElement tem=templateNodes.at(i).toElement();
        QString name=tem.attribute("name");
        QString define=tem.elementsByTagName("define").at(0).toElement().text();
        int rowCount=ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        QTableWidgetItem *item=new QTableWidgetItem(QString::number(rowCount+1));
        QTableWidgetItem *item1=new QTableWidgetItem(name);
        QTableWidgetItem *item2=new QTableWidgetItem(define);
        QCheckBox *checkBox=new QCheckBox();
        QWidget *widgetCheck=new QWidget();
        QHBoxLayout *layoutCheck=new QHBoxLayout();
        layoutCheck->addWidget(checkBox);
        layoutCheck->setContentsMargins(0,0,0,0);
        layoutCheck->setAlignment(checkBox, Qt::AlignCenter);
        widgetCheck->setLayout(layoutCheck);
        connect(checkBox,&QCheckBox::clicked,this,[=](){
            int x = checkBox->parentWidget()->frameGeometry().x();
            int y = checkBox->parentWidget()->frameGeometry().y();
            QModelIndex index = ui->tableWidget->indexAt(QPoint(x, y));
            int row = index.row();
            this->checkId=row;
            for(int i=0;i<ui->tableWidget->rowCount();i++){
                if(i==row){
                    continue;
                }
                QWidget *widget = ui->tableWidget->cellWidget(i, 3);
                QCheckBox *checkBox = widget->findChild<QCheckBox *>();
                if(checkBox->isChecked()){
                    checkBox->setChecked(false);
                }
            }
        });
        ui->tableWidget->setItem(rowCount,0,item);
        ui->tableWidget->setItem(rowCount,1,item1);
        ui->tableWidget->setItem(rowCount,2,item2);
        ui->tableWidget->setCellWidget(rowCount,3,widgetCheck);
    }
}

void templateTables::on_pushButton_2_clicked()
{
    this->close();
}


void templateTables::on_pushButton_clicked()
{
    if(this->checkId!=-1){
        emit sendCheckId(this->checkId);
    }
    this->close();
}

