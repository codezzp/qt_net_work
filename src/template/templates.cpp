#include "templates.h"
#include "ui_templates.h"

templates::templates(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::templates)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->expandAll();
    ui->widget->hide();
    QSizePolicy sp_retain = ui->widget->sizePolicy();
    sp_retain.setRetainSizeWhenHidden(true);
    ui->widget->setSizePolicy(sp_retain);
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(1);
    QTableWidgetItem * item=new QTableWidgetItem("选择原数据字节段");
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0,0,item);
    ui->tableWidget->setSpan(0,0,1,2);
    QTableWidgetItem * item1=new QTableWidgetItem("选择模板数据字节段");
    item1->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0,2,item1);
    ui->tableWidget->setSpan(0,2,1,2);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    AllReadXmlDatas();
}

templates::~templates()
{
    delete ui;
}

void templates::on_pushButton_clicked()
{
    int rowCount=ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowCount+1);
    QTableWidgetItem *item=new QTableWidgetItem(ui->lineEdit->text());
    QTableWidgetItem *item1=new QTableWidgetItem(ui->lineEdit_2->text());
    QTableWidgetItem *item2=new QTableWidgetItem(ui->lineEdit_3->text());
    QTableWidgetItem *item3=new QTableWidgetItem(ui->lineEdit_4->text());
    ui->tableWidget->setItem(rowCount,0,item);
    ui->tableWidget->setItem(rowCount,1,item1);
    ui->tableWidget->setItem(rowCount,2,item2);
    ui->tableWidget->setItem(rowCount,3,item3);
}


void templates::on_pushButton_2_clicked()
{
    int rowCount=ui->tableWidget->rowCount();
    if(ui->tableWidget->rowCount()!=1){
        ui->tableWidget->removeRow(rowCount-1);
    }
}


void templates::on_pushButton_3_clicked()
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget->topLevelItem(0));
    item->setText(0,"未命名");
    item->setIcon(0,QIcon(":/new/prefix1/icon/sloveall.png"));
    item->setExpanded(true);
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    ui->widget->show();
    ui->lineEdit_11->setText("未命名");
    ui->lineEdit_14->setText("");
    int rowCount=ui->tableWidget->rowCount();
    while(rowCount>1){
        ui->tableWidget->removeRow(rowCount-1);
        rowCount=ui->tableWidget->rowCount();
    }
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->radioButton->setChecked(false);
    ui->radioButton_2->setChecked(false);
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->lineEdit_12->setText("");
    ui->lineEdit_13->setText("");
    ui->lineEdit_15->setText("");
    addTemplate();
    ui->treeWidget->setCurrentItem(item);
    emit refreshInfo();
}


void templates::on_pushButton_5_clicked()
{
    int index = ui->treeWidget->currentIndex().row();
    deleteTemplate(index);
    emit refreshInfo();
}


void templates::on_pushButton_4_clicked()
{
    updateTemplate(ui->treeWidget->currentIndex().row());
    emit refreshInfo();
}

void templates::AllReadXmlDatas()
{
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
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidget->topLevelItem(0));
        item->setText(0,name);
        item->setIcon(0,QIcon(":/new/prefix1/icon/sloveall.png"));
        item->setExpanded(true);
    }
}


void templates::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    ui->widget->show();
    ui->pushButton_5->setEnabled(true);
    ui->pushButton_4->setEnabled(true);
    int index = ui->treeWidget->currentIndex().row();
    int rowCount=ui->tableWidget->rowCount();
    while(rowCount>1){
        ui->tableWidget->removeRow(rowCount-1);
        rowCount=ui->tableWidget->rowCount();
    }
    getTemplate(index);
}

void templates::getTemplate(int index)
{
    QFile file("./favorites/templates.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNodeList templateNodes = root.elementsByTagName("template");
    QDomElement element=templateNodes.at(index).toElement();
    ui->lineEdit_11->setText(element.attribute("name"));
    ui->lineEdit_14->setText(element.elementsByTagName("define").at(0).toElement().text());
    QDomElement check=element.elementsByTagName("check").at(0).toElement();
    QDomElement sumCheck=check.elementsByTagName("sumCheck").at(0).toElement();
    QDomElement CRCCheck=check.elementsByTagName("CRCCheck").at(0).toElement();
    ui->checkBox->setChecked(check.attribute("checked").toInt());
    ui->lineEdit_15->setText(CRCCheck.elementsByTagName("byteCount").at(0).toElement().text());
    ui->radioButton->setChecked(sumCheck.attribute("checked").toInt());
    ui->radioButton_2->setChecked(CRCCheck.attribute("checked").toInt());
    ui->lineEdit_5->setText(sumCheck.elementsByTagName("beginLocation").at(0).toElement().text());
    ui->lineEdit_6->setText(sumCheck.elementsByTagName("endLocation").at(0).toElement().text());
    ui->lineEdit_12->setText(sumCheck.elementsByTagName("location").at(0).toElement().text());
    ui->lineEdit_7->setText(CRCCheck.elementsByTagName("beginLocation").at(0).toElement().text());
    ui->lineEdit_8->setText(CRCCheck.elementsByTagName("endLocation").at(0).toElement().text());
    ui->lineEdit_13->setText(CRCCheck.elementsByTagName("location").at(0).toElement().text());
    QDomElement dataCount=element.elementsByTagName("dataCount").at(0).toElement();
    ui->lineEdit_9->setText(dataCount.elementsByTagName("loaction").at(0).toElement().text());
    ui->lineEdit_10->setText(dataCount.elementsByTagName("byteCount").at(0).toElement().text());
    ui->checkBox_2->setChecked(dataCount.attribute("checked").toInt());
    QDomElement initLocation=element.elementsByTagName("initLocation").at(0).toElement();
    QDomElement endLocation=element.elementsByTagName("templateLocation").at(0).toElement();
    QDomNodeList initList=initLocation.elementsByTagName("location");
    QDomNodeList endList=endLocation.elementsByTagName("location");
    for(int i=0;i<initList.count();i++){
        QDomElement initElement=initList.at(i).toElement();
        QTableWidgetItem *item=new QTableWidgetItem(initElement.elementsByTagName("beginLocation").at(0).toElement().text());
        QTableWidgetItem *item1=new QTableWidgetItem(initElement.elementsByTagName("endLocation").at(0).toElement().text());
        int rowCount=ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(rowCount+1);
        ui->tableWidget->setItem(i+1,0,item);
        ui->tableWidget->setItem(i+1,1,item1);
    }
    for(int i=0;i<endList.count();i++){
        QDomElement endElement=endList.at(i).toElement();
        QTableWidgetItem *item=new QTableWidgetItem(endElement.elementsByTagName("beginLocation").at(0).toElement().text());
        QTableWidgetItem *item1=new QTableWidgetItem(endElement.elementsByTagName("endLocation").at(0).toElement().text());
        ui->tableWidget->setItem(i+1,2,item);
        ui->tableWidget->setItem(i+1,3,item1);
    }
}

void templates::updateTemplate(int index)
{
    addTemplate();
    QFile file("./favorites/templates.xml");
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNodeList templateNodes = root.elementsByTagName("template");
    QDomElement lastElement=templateNodes.at(templateNodes.count()-1).toElement();
    QDomElement element=templateNodes.at(index).toElement();
    QString name=lastElement.attribute("name");
    ui->treeWidget->topLevelItem(0)->child(index)->setText(0,name);
    root.replaceChild(lastElement,element);
    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QTextStream stream(&file);
    doc.save(stream, 4);
    file.close();
    QMessageBox::information(this,"提示","保存成功",QMessageBox::Ok);
}

void templates::addTemplate()
{
    QFile file("./favorites/templates.xml");
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomElement tem=doc.createElement("template");
    tem.setAttribute("name",ui->lineEdit_11->text());
    QDomElement define=doc.createElement("define");
    QDomText defineText=doc.createTextNode(ui->lineEdit_14->text());
    define.appendChild(defineText);
    tem.appendChild(define);
    QDomElement initLocation=doc.createElement("initLocation");
    for(int i=1;i<ui->tableWidget->rowCount();i++){
        QDomElement locations=doc.createElement("location");
        QDomElement beginLocation=doc.createElement("beginLocation");
        QDomText beginText=doc.createTextNode(ui->tableWidget->item(i,0)->text());
        beginLocation.appendChild(beginText);
        QDomElement endLocation=doc.createElement("endLocation");
        QDomText endText=doc.createTextNode(ui->tableWidget->item(i,1)->text());
        endLocation.appendChild(endText);
        locations.appendChild(beginLocation);
        locations.appendChild(endLocation);
        initLocation.appendChild(locations);

    }
    QDomElement templateLocation=doc.createElement("templateLocation");
    for(int i=1;i<ui->tableWidget->rowCount();i++){
        QDomElement locations=doc.createElement("location");
        QDomElement beginLocation=doc.createElement("beginLocation");
        QDomText beginText=doc.createTextNode(ui->tableWidget->item(i,2)->text());
        beginLocation.appendChild(beginText);
        QDomElement endLocation=doc.createElement("endLocation");
        QDomText endText=doc.createTextNode(ui->tableWidget->item(i,3)->text());
        endLocation.appendChild(endText);
        locations.appendChild(beginLocation);
        locations.appendChild(endLocation);
        templateLocation.appendChild(locations);
    }
    tem.appendChild(initLocation);
    tem.appendChild(templateLocation);
    QDomElement check=doc.createElement("check");
    check.setAttribute("checked",ui->checkBox->isChecked());
    QDomElement sumCheck=doc.createElement("sumCheck");
    sumCheck.setAttribute("checked",ui->radioButton->isChecked());
    QDomElement beginLocation=doc.createElement("beginLocation");
    QDomText beginText=doc.createTextNode(ui->lineEdit_5->text());
    beginLocation.appendChild(beginText);
    QDomElement endLocation=doc.createElement("endLocation");
    QDomText endText=doc.createTextNode(ui->lineEdit_6->text());
    endLocation.appendChild(endText);
    QDomElement location=doc.createElement("location");
    QDomText text=doc.createTextNode(ui->lineEdit_12->text());
    location.appendChild(text);
    QDomElement byteCount0=doc.createElement("byteCount");
    QDomText byteText=doc.createTextNode(ui->lineEdit_15->text());
    byteCount0.appendChild(byteText);
    sumCheck.appendChild(beginLocation);
    sumCheck.appendChild(endLocation);
    sumCheck.appendChild(location);
    sumCheck.appendChild(byteCount0);
    check.appendChild(sumCheck);
    QDomElement CRCCheck=doc.createElement("CRCCheck");
    CRCCheck.setAttribute("checked",ui->radioButton_2->isChecked());
    QDomElement beginLocation1=doc.createElement("beginLocation");
    QDomText beginText1=doc.createTextNode(ui->lineEdit_7->text());
    beginLocation1.appendChild(beginText1);
    QDomElement endLocation1=doc.createElement("endLocation");
    QDomText endText1=doc.createTextNode(ui->lineEdit_8->text());
    endLocation1.appendChild(endText1);
    QDomElement location1=doc.createElement("location");
    QDomText text1=doc.createTextNode(ui->lineEdit_13->text());
    location1.appendChild(text1);
    CRCCheck.appendChild(beginLocation1);
    CRCCheck.appendChild(endLocation1);
    CRCCheck.appendChild(location1);
    check.appendChild(CRCCheck);
    tem.appendChild(check);
    QDomElement dataCount=doc.createElement("dataCount");
    dataCount.setAttribute("checked",ui->checkBox_2->isChecked());
    QDomElement countLocation=doc.createElement("loaction");
    QDomText countText=doc.createTextNode(ui->lineEdit_9->text());
    countLocation.appendChild(countText);
    QDomElement byteCount=doc.createElement("byteCount");
    QDomText byteCountText=doc.createTextNode(ui->lineEdit_10->text());
    byteCount.appendChild(byteCountText);
    dataCount.appendChild(countLocation);
    dataCount.appendChild(byteCount);
    tem.appendChild(dataCount);
    root.appendChild(tem);
    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QTextStream stream(&file);
    doc.save(stream, 4);
    file.close();
}

void templates::deleteTemplate(int index)
{
    QFile file("./favorites/templates.xml");
    if (!file.open(QFileDevice::ReadOnly)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        QMessageBox::information(NULL, "提示", "操作的文件不是XML文件！");
        file.close();
        return;
    }
    file.close();
    int ret = QMessageBox::question(nullptr, "提示", "正在删除该模板", QMessageBox::Ok | QMessageBox::Cancel);
        if (ret == QMessageBox::Ok) {
            QDomElement root = doc.documentElement();
            QDomNodeList templateNodes = root.elementsByTagName("template");
            QDomElement deleteElement=templateNodes.at(index).toElement();
            root.removeChild(deleteElement);
            if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
                QMessageBox::information(NULL, "提示", "文件打开失败！");
                return;
            }
            QTextStream stream(&file);
            doc.save(stream, 4);
            file.close();
            QTreeWidgetItem *item = ui->treeWidget->topLevelItem(0);
            item->removeChild(item->child(index));
            ui->widget->hide();
            ui->pushButton_5->setEnabled(false);
            ui->pushButton_4->setEnabled(false);
        }
        else if (ret == QMessageBox::Cancel) {}
}
