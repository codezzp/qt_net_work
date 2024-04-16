#include "form.h"
#include "ui_form.h"

form::form(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::form)
{
    ui->setupUi(this);
//    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->tableWidget->setColumnCount(3);
    QStringList headers;
    headers << "序号" << "偏移位" << "字节数";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

form::~form()
{

    delete ui;
    if(charForm!=NULL){
        charForm->StopDraw();
        delete charForm;
        charForm=NULL;
    }
}

void form::on_pushButton_2_clicked()
{
    ui->groupBox->setEnabled(true);
    ui->pushButton_5->setEnabled(false);
    if(charForm!=NULL){
        charForm->StopDraw();
        delete charForm;
        charForm=NULL;
    }
    ui->widget_4->setEnabled(true);
    ui->checkBox_3->setEnabled(true);
    ui->tableWidget->setEnabled(true);
    ui->groupBox->setEnabled(true);
    memset(BuffDatas, 0, sizeof(BuffDatas));
    BuffDatas[0]='\0';
    type.dataType=0;
    memset(type.dataHeader, 0, sizeof(type.dataHeader));
    type.dataHeader[0]='\0';
    type.fixedLength=-1;
    type.moveLength=-1;
    type.items.clear();
    used=type;
    FindDatas.isFind=false;
    memset(FindDatas.findedData, 0, sizeof(FindDatas.findedData));
    FindDatas.findedData[0]='\0';
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(true);
}

void form::recvDatas(char *data){
    if(strlen(BuffDatas)+strlen(data)>=99999){
        memmove(BuffDatas, BuffDatas + strlen(data), strlen(BuffDatas) - strlen(data) + 1);
    }
    // memmove(original, original + n, strlen(original) - n + 1);去除前面n个字符
    strcat(BuffDatas,data);
    dealDatas();
}

void form::dealDatas(){
    if(ui->radioButton->isChecked()){
        dealFixedLength();
    }else{
        dealMoveLength();
    }
}

void form::dealFixedLength(){
    if(!FindDatas.isFind&&(int)strlen(used.dataHeader)<=used.fixedLength){
        if(strlen(BuffDatas)>=strlen(used.dataHeader)){
            int i=0,j=0,m=0,n=0;
            for(i=0;i<(int)strlen(BuffDatas);i++){
                if(used.dataHeader[0]==BuffDatas[i]){
                    n=i;
                    for(j=0;j<(int)strlen(used.dataHeader);j++){
                        if(used.dataHeader[j]!=BuffDatas[n]){
                            break;
                        }
                        n++;
                    }
                    if(j>=(int)strlen(used.dataHeader)){
                        FindDatas.isFind=true;
                        break;
                    }
                }
                m++;
            }
            memmove(BuffDatas, BuffDatas + m, strlen(BuffDatas) - m + 1);
        }
    }
    if(FindDatas.isFind){
        if(used.fixedLength>=(int)strlen(BuffDatas)){
            used.fixedLength-=strlen(BuffDatas);
            strncat(FindDatas.findedData, BuffDatas, strlen(BuffDatas));
            FindDatas.findedData[strlen(FindDatas.findedData)] = '\0';
            memset(BuffDatas, 0, sizeof(BuffDatas));
        }else{
            strncat(FindDatas.findedData, BuffDatas, used.fixedLength);
            FindDatas.findedData[strlen(FindDatas.findedData)] = '\0';
            memmove(BuffDatas, BuffDatas + used.fixedLength, strlen(BuffDatas) - used.fixedLength + 1);
            used.fixedLength=0;
        }
    }
    if(used.fixedLength==0){
        if(ui->checkBox_3->isChecked()){
            for(int i=0;i<used.items.length();i++){
                if((int)strlen(FindDatas.findedData)+1>=used.items.at(i).moveLength){
                    if(FindDatas.findedData[used.items.at(i).moveLength]==used.items.at(i).moveChar){
//                        for(int i=0;i<(int)strlen(FindDatas.findedData);i++){
//                            qDebug("%02X",FindDatas.findedData[i]);
//                        }
                        emit sendFindData(FindDatas.findedData);
                        charForm->DrawData(FindDatas.findedData);
                    }
                }
            }
        }else{
//            for(int i=0;i<(int)strlen(FindDatas.findedData);i++){
//                qDebug("%02X",FindDatas.findedData[i]);
//            }
            emit sendFindData(FindDatas.findedData);
            charForm->DrawData(FindDatas.findedData);
        }
        used.fixedLength=type.fixedLength;
        used.items=type.items;
        FindDatas.isFind=false;
        memset(FindDatas.findedData, 0, sizeof(FindDatas.findedData));
        FindDatas.findedData[0]='\0';
    }
}

void form::dealMoveLength(){
    if(!FindDatas.isFind&&(int)strlen(used.dataHeader)<=used.dataType){
        if(strlen(BuffDatas)>=strlen(used.dataHeader)){
            int i=0,j=0,m=0,n=0;
            for(i=0;i<(int)strlen(BuffDatas);i++){
                if(used.dataHeader[0]==BuffDatas[i]){
                    n=i;
                    for(j=0;j<(int)strlen(used.dataHeader);j++){
                        if(used.dataHeader[j]!=BuffDatas[n]){
                            break;
                        }
                        n++;
                    }
                    if(j>=(int)strlen(used.dataHeader)){
                        FindDatas.isFind=true;
                        break;
                    }
                }
                m++;
            }
            memmove(BuffDatas, BuffDatas + m, strlen(BuffDatas) - m + 1);
        }
    }
    if(FindDatas.isFind&&used.moveLength!=0){
        if(used.moveLength>=(int)strlen(BuffDatas)){
            used.moveLength-=strlen(BuffDatas);
            memset(BuffDatas, 0, sizeof(BuffDatas));
        }else{
            memmove(BuffDatas, BuffDatas + used.moveLength, strlen(BuffDatas) - used.moveLength + 1);
            used.moveLength=0;
        }
    }
    if(FindDatas.isFind&&used.moveLength==0){
        if(used.dataType>=(int)strlen(BuffDatas)){
            used.dataType-=strlen(BuffDatas);
            strncat(FindDatas.findedData, BuffDatas, strlen(BuffDatas));
            FindDatas.findedData[strlen(FindDatas.findedData)] = '\0';
            memset(BuffDatas, 0, sizeof(BuffDatas));
        }else{
            strncat(FindDatas.findedData, BuffDatas, used.dataType);
            FindDatas.findedData[strlen(FindDatas.findedData)] = '\0';
            memmove(BuffDatas, BuffDatas + used.dataType, strlen(BuffDatas) - used.dataType + 1);
            used.dataType=0;
        }
    }
    if(used.dataType==0){
        if(ui->checkBox_3->isChecked()){
            for(int i=0;i<used.items.length();i++){
                if((int)strlen(FindDatas.findedData)+1>=used.items.at(i).moveLength){
                    if(FindDatas.findedData[used.items.at(i).moveLength]==used.items.at(i).moveChar){
//                        for(int i=0;i<(int)strlen(FindDatas.findedData);i++){
//                            qDebug("%02X",FindDatas.findedData[i]);
//                        }
                        emit sendFindData(FindDatas.findedData);
                        charForm->DrawData(FindDatas.findedData);
                    }
                }
            }
        }else{
//            for(int i=0;i<(int)strlen(FindDatas.findedData);i++){
//                qDebug("%02X",FindDatas.findedData[i]);
//            }
            emit sendFindData(FindDatas.findedData);
            charForm->DrawData(FindDatas.findedData);
        }
        used.dataType=type.dataType;
        used.moveLength=type.moveLength;
        used.items=type.items;
        FindDatas.isFind=false;
        memset(FindDatas.findedData, 0, sizeof(FindDatas.findedData));
        FindDatas.findedData[0]='\0';
    }
}

void form::on_checkBox_stateChanged(int arg1)
{
    if(arg1==2){
        ui->lineEdit->setEnabled(true);
    }else{
        ui->lineEdit->setEnabled(false);
    }
}


void form::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1==2){
        ui->widget_3->setEnabled(true);
    }else{
        ui->widget_3->setEnabled(false);
    }
}


void form::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1==2){
        ui->widget_4->setEnabled(true);
        ui->tableWidget->setEnabled(true);
    }else{
        ui->widget_4->setEnabled(false);
        ui->tableWidget->setEnabled(false);
    }
}


void form::on_pushButton_3_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(rowCount + 1);
    QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(rowCount+1));
    QTableWidgetItem *item2 = new QTableWidgetItem(ui->lineEdit_4->text());
    QTableWidgetItem *item3 = new QTableWidgetItem(ui->lineEdit_5->text());
    ui->tableWidget->setItem(rowCount, 0, item1);
    ui->tableWidget->setItem(rowCount, 1, item2);
    ui->tableWidget->setItem(rowCount, 2, item3);
}


void form::on_pushButton_4_clicked()
{
    int rowCount=ui->tableWidget->rowCount();
    if(ui->tableWidget->rowCount()!=0){
        ui->tableWidget->removeRow(rowCount-1);
    }
}


void form::on_pushButton_clicked()
{
    ui->pushButton_2->setEnabled(true);
    ui->groupBox->setEnabled(false);
    ui->pushButton_5->setEnabled(true);
    ui->widget_4->setEnabled(false);
    ui->checkBox_3->setEnabled(false);
    ui->tableWidget->setEnabled(false);
    ui->groupBox_2->setEnabled(false);
    if(charForm==NULL){
        charForm=new ICharForm();
        if(ui->checkBox->isChecked()){
           strcpy(type.dataHeader,QStringtoHex(ui->lineEdit->text()).data());
        }
        if(ui->checkBox_2->isChecked()){
            if(ui->radioButton->isChecked()){
                type.fixedLength=ui->lineEdit_2->text().toInt();
            }else{
                type.moveLength=ui->lineEdit_3->text().toInt();
                type.dataType=dict[ui->comboBox->currentText()];
            }
        }
        if(ui->checkBox_3->isChecked()){
            for(int i=0;i<ui->tableWidget->rowCount();i++){
                struct defineDataType item;
                item.moveLength=ui->tableWidget->item(i,1)->text().toInt();
                strcpy(&item.moveChar,QStringtoHex(ui->tableWidget->item(i,2)->text()).data());
                type.items.append(item);
            }
        }
    }
    FindDatas.isFind=false;
    used=type;
    ui->pushButton->setEnabled(false);
}

QByteArray form::QStringtoHex(const QString &str){
    QStringList lst=str.split(" ");
    QByteArray ba;
    uint8_t temp;
    bool ok;
    foreach (QString s, lst) {
        temp=(uint8_t)s.toInt(&ok,16);
        ba.append((char)temp);
    }
    return ba;
}

void form::on_pushButton_5_clicked()
{
    charForm->CreateChartForm(cfp);
    charForm->StartDraw();
}

