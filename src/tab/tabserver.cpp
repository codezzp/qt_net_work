#include "tab/tabserver.h"
#include "ui_tabserver.h"
#include <QMessageBox>
#include "other/form.h"
#include <QScrollBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QDomDocument>

tabserver::tabserver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tabserver)
{
    ui->setupUi(this);
//    this->setAttribute(Qt::WA_DeleteOnClose,true);
    Sett.length=-1;
    Sett.position=-1;
    con=true;
    connect(ui->textEdit_2, &QTextEdit::textChanged, [=]() {
        ui->textEdit_2->moveCursor(QTextCursor::End);
    });
    connect(ui->textEdit_3, &QTextEdit::textChanged, [=]() {
        ui->textEdit_3->moveCursor(QTextCursor::End);
    });
}

tabserver::~tabserver()
{
    if(from!=NULL){
        from->deleteLater();
        from=NULL;
    }
    if(udpsocket!=NULL){
        delete udpsocket;
        udpsocket=NULL;
    }
    if(tcpsocket!=NULL){
        delete tcpsocket;
        tcpsocket=NULL;
    }
    if(server!=NULL){
        delete server;
        server=NULL;
    }
    if(multiUdp!=NULL){
        delete multiUdp;
        multiUdp=NULL;
    }
    if(senderTCP!=NULL){
        senderTCP->close();
        delete senderTCP;
    }
    if(senderUDP!=NULL){
        senderUDP->close();
        delete senderUDP;
    }
    if(senderBroadUDP!=NULL){
        senderBroadUDP->close();
        delete senderBroadUDP;
    }
    if(senderMultiUDP!=NULL){
        senderMultiUDP->close();
        delete senderMultiUDP;
    }
    if(entity!=NULL){
        delete entity;
        entity=NULL;
    }
    delete ui;
}

void tabserver::setInfor(QStringList listStr)
{
    ui->comboBox_2->setCurrentText(listStr[0]);
    ui->lineEdit_6->setText(listStr[1]);
    ui->lineEdit_5->setText(listStr[2]);
}


void tabserver::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text()=="启动"){
        emit(sendUpdteInfo(ui->comboBox_2->currentText()+" "+ui->lineEdit->text()+" "
                           +ui->lineEdit_6->text()+" "+ui->lineEdit_5->text()));
        ui->pushButton_7->setDisabled(false);
        ui->lineEdit->setDisabled(true);
        ui->lineEdit_2->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->checkBox->setDisabled(true);
        ui->checkBox_2->setDisabled(true);
        ui->comboBox_2->setDisabled(true);
        if(ui->comboBox_2->currentText()=="UDP单播")
        {
            udpsocket=new QUdpSocket();
            udpsocket->bind(QHostAddress(ui->lineEdit_6->text()),ui->lineEdit_5->text().toInt());
            connect(udpsocket,&QUdpSocket::readyRead,this,&tabserver::dowork);
        }else if(ui->comboBox_2->currentText()=="UDP组播"){
            multiUdp=new QUdpSocket();
            QHostAddress groupAddress=QHostAddress(ui->lineEdit_6->text());
//            multiUdp->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
            multiUdp->bind(groupAddress,ui->lineEdit_5->text().toInt());
            multiUdp->joinMulticastGroup(groupAddress);
            connect(multiUdp, &QUdpSocket::readyRead, this, [=]() {
                QString receivedMessage;
                while (multiUdp->hasPendingDatagrams()) {
                    QByteArray datagram;
                    datagram.resize(multiUdp->pendingDatagramSize());
                    QHostAddress senderAddress;
                    quint16 senderPort;
                    multiUdp->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
                    if(ui->checkBox_9->isChecked()){
                        sendForwardText(datagram);
                    }
                    if(ui->checkBox_8->isChecked()){
                        recvFindData(datagram);
                    }
                    receivedMessage = QString::fromLocal8Bit(datagram.data());
                    if(ui->checkBox_6->isChecked())
                    {
                        receivedMessage="";
                        QByteArray buff=datagram;
                        QString t="";
                        for(int i=0;i<datagram.length();i++){
                            t=QString("%1").arg((uint8_t)buff[i],2,16,QLatin1Char('0'));
                            receivedMessage=receivedMessage+t+" ";
                        }
                    }
                    ui->lineEdit_2->setText(senderAddress.toString());
                    ui->lineEdit_3->setText(QString::number(senderPort));
                    if(ui->pushButton_2->text()=="表格显示"&&ui->checkBox_8->isChecked()==false){
                        ui->textEdit_3->insertPlainText(receivedMessage);
                    }else{
                        insertTable(receivedMessage);
                    }
                }
            });
        }else if(ui->comboBox_2->currentText()=="TCP服务端"){
            server=new QTcpServer();
            server->listen(QHostAddress(ui->lineEdit_6->text()),ui->lineEdit_5->text().toInt());
            tcpsocket=new QTcpSocket();
            connect(server,&QTcpServer::newConnection,this,[=](){
                tcpsocket=server->nextPendingConnection();
                ui->lineEdit_2->setText(tcpsocket->peerAddress().toString());
                ui->lineEdit_3->setText(QString::number(tcpsocket->peerPort()));
                connect(tcpsocket,&QTcpSocket::readyRead,this,[=](){
                    QByteArray buf = tcpsocket->readAll();
                    char *sendData=new char[buf.length()+1];
                    strcpy(sendData,buf.data());
                    emit sendDatas(sendData);
                    delete[] sendData;
                    QFile f(ui->lineEdit_4->text());
                    if(ui->checkBox_9->isChecked()){
                        sendForwardText(buf);
                    }
                    QString message=QString::fromLocal8Bit(buf.data());
                    if(ui->checkBox_8->isChecked()){
                        recvFindData(buf);
                    }
                    if(ui->checkBox_6->isChecked())
                    {
                        message="";
                        QByteArray buff=buf;
                        QString t="";
                        for(int i=0;i<buf.length();i++){
                            t=QString("%1").arg((uint8_t)buff[i],2,16,QLatin1Char('0'));
                            message=message+t+" ";
                        }
                    }
                    if(ui->checkBox_7->isChecked())
                    {
                        if(ui->lineEdit_4->text().isEmpty())
                        {QMessageBox::question(this,tr("提示"),tr("找不到文件"),QMessageBox::Ok,QMessageBox::Ok);}
                        else
                        {
                            f.open(QIODevice::WriteOnly|QIODevice::Append);
//                            f.write(str.toUtf8());
                        }
                    }
                    if(ui->checkBox_7->isChecked()==false)
                    {f.close();}
                    if(ui->pushButton_10->text()=="暂停显示"&&ui->checkBox_8->isChecked()==false)
                    {
                        if(ui->pushButton_2->text()=="表格显示"){
                            ui->textEdit_3->insertPlainText(message);
                        }else{
                            insertTable(message);
                        }
                        ui->lineEdit_7->setText(QString::number(ui->lineEdit_7->text().toInt()+message.length()));
                    }
                });
            });
        }else{
            broadudp = new QUdpSocket();
            broadudp->bind(QHostAddress(ui->lineEdit_6->text()), ui->lineEdit_5->text().toInt());
            connect(broadudp, &QUdpSocket::readyRead, this, [=](){
                QByteArray datagram;
                while (broadudp->hasPendingDatagrams()) {
                    datagram.resize(broadudp->pendingDatagramSize());
                    QHostAddress senderAddress;
                    quint16 senderPort;
                    broadudp->readDatagram(datagram.data(), datagram.size(),
                                                   &senderAddress, &senderPort);
                    if(ui->checkBox_9->isChecked()){
                        sendForwardText(datagram);
                    }
                    if(ui->checkBox_8->isChecked()){
                        recvFindData(datagram);
                    }
                    QString message = QString::fromLocal8Bit(datagram.data());
                    if(ui->checkBox_6->isChecked())
                    {
                        message="";
                        QByteArray buff=datagram;
                        QString t="";
                        for(int i=0;i<datagram.length();i++){
                            t=QString("%1").arg((uint8_t)buff[i],2,16,QLatin1Char('0'));
                            message=message+t+" ";
                        }
                    }
                    if(ui->pushButton_2->text()=="表格显示"&&ui->checkBox_8->isChecked()==false){
                        ui->textEdit_3->insertPlainText(message);
                    }else{
                        insertTable(message);
                    }
                    ui->lineEdit_2->setText(senderAddress.toString());
                    ui->lineEdit_3->setText(QString::number(senderPort));
                }
            });
        }
        ui->pushButton_3->setText("关闭");
    }else{
        emit(sendUpdteInfo(ui->lineEdit->text()));
        if(ui->comboBox_2->currentText()=="UDP单播")
        {
            udpsocket->close();
        }
        else if(ui->comboBox_2->currentText()=="TCP服务端")
        {
            tcpsocket->close();
        }else if(ui->comboBox_2->currentText()=="UDP组播"){
            multiUdp->close();
        }else{
            broadudp->close();
        }
        ui->pushButton_7->setDisabled(true);
        ui->lineEdit->setDisabled(false);
        ui->lineEdit_2->setDisabled(false);
        ui->lineEdit_3->setDisabled(false);
        ui->lineEdit_6->setDisabled(false);
        ui->lineEdit_5->setDisabled(false);
        ui->checkBox->setDisabled(false);
        ui->checkBox_2->setDisabled(false);
        ui->comboBox_2->setDisabled(false);
        ui->pushButton_3->setText("启动");
    }
}

void tabserver::dowork()
{
    QHostAddress targetaddr;
    quint16 targetport;
    QFile f(ui->lineEdit_9->text());
    QByteArray datagram;
    datagram.resize(udpsocket->pendingDatagramSize());
    udpsocket->readDatagram(datagram.data(),datagram.size(),&targetaddr,&targetport);
    ui->lineEdit_2->setText(targetaddr.toString());
    ui->lineEdit_3->setText(QString::number(targetport));
    if(ui->checkBox_9->isChecked()){
        sendForwardText(datagram);
    }
    if(ui->checkBox_8->isChecked()){
        recvFindData(datagram);
    }
    QString message=QString::fromLocal8Bit(datagram.data());
    if(ui->checkBox_6->isChecked())
    {
        message="";
        QByteArray buff=datagram;
        QString t="";
        for(int i=0;i<datagram.length();i++){
            t=QString("%1").arg((uint8_t)buff[i],2,16,QLatin1Char('0'));
            message=message+t+" ";
        }
    }
    if(ui->checkBox_7->isChecked())
    {
        if(ui->lineEdit_9->text().isEmpty())
        {QMessageBox::question(this,tr("提示"),tr("找不到文件"),QMessageBox::Ok,QMessageBox::Ok);}
        else
        {
            f.open(QIODevice::WriteOnly|QIODevice::Append);
//            f.write(str.toUtf8());
        }
    }
    if(ui->checkBox_7->isChecked()==false)
    {f.close();}
    if(ui->pushButton_10->text()=="暂停显示"&&(ui->checkBox_8->isChecked()==false))
    {
        if(ui->pushButton_2->text()=="表格显示"){
            ui->textEdit_3->insertPlainText(message);
        }else{
            insertTable(message);
        }
        ui->lineEdit_7->setText(QString::number(ui->lineEdit_7->text().toInt()+message.length()));
    }
}


void tabserver::on_pushButton_7_clicked()
{
    QByteArray message=ui->textEdit_2->toPlainText().toLocal8Bit();
    if(ui->comboBox_2->currentText()=="UDP单播"){
        udpsocket->writeDatagram(message.data(),message.size(),QHostAddress(ui->lineEdit_2->text()),ui->lineEdit_3->text().toInt());
    }else if(ui->comboBox_2->currentText()=="TCP服务端"){
        tcpsocket->write(message);
    }else if(ui->comboBox_2->currentText()=="UDP组播"){
        multiUdp->writeDatagram(message, QHostAddress(ui->lineEdit_2->text()), ui->lineEdit_3->text().toInt());
    }else{
        broadudp->writeDatagram(message, QHostAddress(ui->lineEdit_2->text()),ui->lineEdit_3->text().toInt() );
    }
}


void tabserver::on_pushButton_4_clicked()
{
    ui->lineEdit_4->setText("0");
    ui->lineEdit_7->setText("0");
}


void tabserver::on_pushButton_14_clicked()
{
    ui->textEdit_3->clear();
}


void tabserver::on_pushButton_13_clicked()
{
    if(from==NULL){
        from=new form();
    }
    from->show();
    connect(this,SIGNAL(sendDatas(char*)),from,SLOT(recvDatas(char*)));
    connect(from,SIGNAL(sendFindData(char*)),this,SLOT(recvFindData(char*)));
}


void tabserver::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text()=="表格显示"){
        ui->textEdit_3->setWordWrapMode(QTextOption::NoWrap);
        ui->pushButton_2->setText("文本显示");
        ui->label_15->hide();
        ui->pushButton_2->setIconSize(QSize(0,0));
        ui->textEdit_3->clear();
        QString initialTable = "<table border=\"1\" cellspacing=\"0\" style=\"text-align:center;vertical-align: middle;white-space: nowrap;overflow: hidden;text-overflow: ellipsis;\">"
                                "<tr><td>次数</td><td>接收时间</td><td>01..........05.............10.............15.............20.............25.............30.............35.............40.............45.............50</td></tr>"
                                "</table>";
        ui->textEdit_3->setHtml(initialTable);
        QScrollBar *horizontalScrollBar = ui->textEdit_3->horizontalScrollBar();
        horizontalScrollBar->setValue(horizontalScrollBar->minimum());
    }else{
        ui->textEdit_3->setWordWrapMode(QTextOption::WrapAnywhere);
        ui->pushButton_2->setText("表格显示");
        ui->label_15->show();
        ui->pushButton_2->setIconSize(QSize(16,16));
        ui->textEdit_3->clear();
    }
}

void tabserver::insertTable(QString message)
{
    if(ui->textEdit_3->toPlainText().isEmpty())
    {
        QString initialTable = "<table border=\"1\" cellspacing=\"0\" style=\"text-align:center;vertical-align: middle;white-space: nowrap;overflow: hidden;text-overflow: ellipsis;\">"
                                "<tr><td>次数</td><td>接收时间</td><td>01..........05.............10.............15.............20.............25.............30.............35.............40.............45.............50</td></tr>"
                                "</table>";
        ui->textEdit_3->setHtml(initialTable);
        QScrollBar *horizontalScrollBar = ui->textEdit_3->horizontalScrollBar();
        horizontalScrollBar->setValue(horizontalScrollBar->minimum());
    }
    QString currentHtml = ui->textEdit_3->toHtml();
    QDateTime datetime = QDateTime::currentDateTime();
    quint8 hour = datetime.time().hour();
    quint8 minute = datetime.time().minute();
    quint8 second = datetime.time().second();
    QString date=QString::number(hour)+":"+QString::number(minute)+":"+QString::number(second);
    QString newTableRow = "<tr><td>"
            +QString::number(ui->textEdit_3->document()->lineCount()/4)+"</td><td>"+date+"</td><td>"+message
            +"</td></tr>";
    QString updatedHtml = currentHtml.mid(0, currentHtml.length() - 22)
                               + newTableRow
                               + "</table>";
    ui->textEdit_3->setHtml(updatedHtml);
    QScrollBar *horizontalScrollBar = ui->textEdit_3->horizontalScrollBar();
    horizontalScrollBar->setValue(horizontalScrollBar->minimum());
}

void tabserver::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,tr("Open File"),"","", 0);
    ui->lineEdit_9->setText(file_name);
}

void tabserver::recvFindData(QByteArray data)
{
    if(ui->checkBox_8->isChecked()){
        QByteArray datagram=data;
        QString message=QString::fromLocal8Bit(datagram.data());
        bool isSet=false;
        if(Sett.position>=0&&Sett.position<datagram.length()){
            if(Sett.length+Sett.position<=datagram.length()){
                isSet=true;
            }
        }
        if(ui->checkBox_6->isChecked())
        {
            message="";
            if(isSet==false){
                QByteArray buff=datagram;
                QString t="";
                for(int i=0;i<datagram.length();i++){
                    t=QString("%1").arg((uint8_t)buff[i],2,16,QLatin1Char('0'));
                    message=message+t+" ";
                }
            }else{
                QByteArray buff=datagram;
                QString t="";
                for(int i=Sett.position;i<Sett.position+Sett.length;i++){
                    t=QString("%1").arg((uint8_t)buff[i],2,16,QLatin1Char('0'));
                    message=message+t+" ";
                }
            }
        }
        if(ui->pushButton_10->text()=="暂停显示")
        {
            if(ui->pushButton_2->text()=="表格显示"){
                ui->textEdit_3->insertPlainText(message);
            }else{
                insertTable(message);
            }
            ui->lineEdit_7->setText(QString::number(ui->lineEdit_7->text().toInt()+message.length()));
        }
    }
}


void tabserver::on_pushButton_12_clicked()
{
    if(set==NULL){
        set=new Setting();
    }
    set->show();
    connect(set,SIGNAL(sendSet(QString)),this,SLOT(recvSet(QString)));
}

void tabserver::recvSet(QString data)
{
    QString position=data.split("-")[0];
    QString length=data.split("-")[1];
    if(position==""){
        position="-1";
    }
    if(length==""){
        length="-1";
    }
    Sett.position=position.toInt();
    Sett.length=length.toInt();
}

void tabserver::on_pushButton_5_clicked()
{
    if(forward==NULL){
        forward=new Forward();
        connect(forward,SIGNAL(sendForward(struct sendType)),this,SLOT(recvForward(struct sendType)));
    }
    forward->show();
}

void tabserver::recvForward(struct sendType data)
{
    foa=data;
}

void tabserver::sendForwardText(QByteArray datas)
{
    if(ui->checkBox_9->isChecked()){
        if(foa.select==1){
            for(int i=0;i<foa.sendLOcations.count();i++){
                sendForDatas(datas,foa.sendLOcations.at(i).sendType,foa.sendLOcations.at(i).ipAddr);
            }
        }
        if(foa.select==2){
            recvCheckTemplate(foa.templateId);
            if(entity==NULL){
                ui->checkBox_9->setChecked(false);
                return ;
            }
            for(int i=0;i<foa.sendLOcations.count();i++){
                datas=templateToSend(datas);
                sendForDatas(datas,foa.sendLOcations.at(i).sendType,foa.sendLOcations.at(i).ipAddr);
            }
        }
    }
}

void tabserver::sendForDatas(QByteArray datas,QString sendType,QString IpAddr)
{
    if(sendType=="TCP"){
        if(senderTCP==NULL){
            senderTCP=new QTcpSocket();
        }
        senderTCP->connectToHost(IpAddr.split(":")[0], IpAddr.split(":")[1].toInt());
        senderTCP->write(datas);
//        senderTCP->abort();
    }else if(sendType=="UDP单播"){
        if(senderUDP==NULL){
            senderUDP=new QUdpSocket();
        }
        senderUDP->writeDatagram(datas.data(),datas.size(),QHostAddress(IpAddr.split(":")[0]),IpAddr.split(":")[1].toInt());
    }else if(sendType=="UDP广播"){
        if(senderBroadUDP==NULL){
            senderBroadUDP=new QUdpSocket();
        }
        senderBroadUDP->bind(QHostAddress(IpAddr.split(":")[0]));
        senderBroadUDP->writeDatagram(datas.data(),datas.size(),QHostAddress(IpAddr.split(":")[0]),IpAddr.split(":")[1].toInt());
    }else if(sendType=="UDP组播"){
        if(senderMultiUDP==NULL){
            senderMultiUDP=new QUdpSocket();
        }
        senderMultiUDP->bind(QHostAddress(IpAddr.split(":")[0]));
        senderMultiUDP->writeDatagram(datas,QHostAddress(IpAddr.split(":")[0]),IpAddr.split(":")[1].toInt());
    }
}

void tabserver::recvCheckTemplate(int id)
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
    QDomElement element=templateNodes.at(id).toElement();
    QString define="";
    QList<struct locations> initlocation;
    QList<struct locations> templateLocation;
    int check;
    struct check sumCheck;
    struct check CRCCheck;
    struct dataCount dataCount;
    define=element.elementsByTagName("define").at(0).toElement().text();
    QDomNodeList initList=element.elementsByTagName("initLocation").at(0).toElement().elementsByTagName("location");
    for(int i=0;i<initList.count();i++){
        QDomElement init=initList.at(i).toElement();
        struct locations location;
        location.beginLocation=init.elementsByTagName("beginLocation").at(0).toElement().text().toInt();
        location.endLocation=init.elementsByTagName("endLocation").at(0).toElement().text().toInt();
        initlocation.append(location);
    }
    QDomNodeList templateList=element.elementsByTagName("templateLocation").at(0).toElement().elementsByTagName("location");
    for(int i=0;i<templateList.count();i++){
        QDomElement tem=templateList.at(i).toElement();
        struct locations location;
        location.beginLocation=tem.elementsByTagName("beginLocation").at(0).toElement().text().toInt();
        location.endLocation=tem.elementsByTagName("endLocation").at(0).toElement().text().toInt();
        templateLocation.append(location);
    }
    check=element.elementsByTagName("check").at(0).toElement().attribute("checked").toInt();
    QDomElement checkElement=element.elementsByTagName("check").at(0).toElement();
    sumCheck.check=checkElement.elementsByTagName("sumCheck").at(0).toElement().attribute("checked").toInt();
    sumCheck.beginLocation=checkElement.elementsByTagName("sumCheck").at(0).toElement()
            .elementsByTagName("beginLocation").at(0).toElement().text().toInt();
    sumCheck.endLocation=checkElement.elementsByTagName("sumCheck").at(0).toElement()
            .elementsByTagName("endLocation").at(0).toElement().text().toInt();
    sumCheck.location=checkElement.elementsByTagName("sumCheck").at(0).toElement()
            .elementsByTagName("location").at(0).toElement().text().toInt();
    CRCCheck.check=checkElement.elementsByTagName("CRCCheck").at(0).toElement().attribute("checked").toInt();
    CRCCheck.beginLocation=checkElement.elementsByTagName("CRCCheck").at(0).toElement()
            .elementsByTagName("beginLocation").at(0).toElement().text().toInt();
    CRCCheck.endLocation=checkElement.elementsByTagName("CRCCheck").at(0).toElement()
            .elementsByTagName("endLocation").at(0).toElement().text().toInt();
    CRCCheck.location=checkElement.elementsByTagName("CRCCheck").at(0).toElement()
            .elementsByTagName("location").at(0).toElement().text().toInt();
    dataCount.check=element.elementsByTagName("dataCount").at(0).toElement().attribute("checked").toInt();
    dataCount.location=element.elementsByTagName("dataCount").at(0).toElement()
            .elementsByTagName("loaction").at(0).toElement().text().toInt();
    dataCount.byteCount=element.elementsByTagName("dataCount").at(0).toElement()
            .elementsByTagName("byteCount").at(0).toElement().text().toInt();
    if(entity==NULL){
        entity=new templateEntity(define,initlocation,templateLocation,check,sumCheck,CRCCheck,dataCount);
    }else{
        entity->setDefine(define);
        entity->setInitLocation(initlocation);
        entity->setTemplateLocation(templateLocation);
        entity->setCheck(check);
        entity->setSumCheck(sumCheck);
        entity->setCRCCheck(CRCCheck);
        entity->setDataCount(dataCount);
    }
}

QByteArray tabserver::templateToSend(QByteArray data)
{
    QString temDataString=entity->getDefine().toLocal8Bit();
    QByteArray temDataByte="";
    QStringList lst=temDataString.split(" ");
    uint8_t temp;
    bool ok;
    foreach(QString s,lst){
        temp=(uint8_t)s.toInt(&ok,16);
        temDataByte.append((char)temp);
    }
    if(entity->getInitLocation().count()!=0){
        int min=qMin(entity->getInitLocation().count(),entity->getTemplateLocation().count());
        for(int i=0;i<min;i++){
            struct locations init=entity->getInitLocation().at(i);
            struct locations tem=entity->getTemplateLocation().at(i);
            if((tem.endLocation-tem.beginLocation)>=(init.endLocation-init.beginLocation)){
                QByteArray midData=data.mid(init.beginLocation,init.endLocation-init.beginLocation);
                temDataByte.replace(tem.beginLocation,(tem.endLocation-tem.beginLocation),midData);
            }else{
                QByteArray midData=data.mid(init.beginLocation,tem.endLocation-tem.beginLocation);
                temDataByte.replace(tem.beginLocation,(tem.endLocation-tem.beginLocation),midData);
            }
        }
    }
    if(entity->getCheck()==1){
        if(entity->getSumCheck().check==1){
            int sumLength=entity->getSumCheck().endLocation-entity->getSumCheck().beginLocation;
            char sum='\0';
            for(int i=entity->getSumCheck().beginLocation;i<sumLength;i++){
                sum+=data.at(i);
            }
            data[entity->getSumCheck().location]=sum;
        }else{
            //CRC检验
        }
    }
    return temDataByte;

//    union uu//联合结构体
//    {
//        int i;
//        char a;
//    };//特点：i和a共用一块内存
//    int CheakSys(){
//        union uu un;
//        un.i = 20;
//        return un.a;
//    }
//    int main(){
//        int ret = CheakSys();
//        if (ret == 0x14)
//             printf("小端\n");
//        else
//    printf("大端\n");
//        return 0;
//    }


//    // ushort 类型网络字节序转为主机字节序
//    #define NTOHS(A) ((((A)&0xFF00)>>8) | (((A)&0x00FF)<<8))
//    // 网络字节序转为主机字节序 short
//    void NetToHost(ushort& h) {
//        h = NTOHS(h);
//    }

//    // 网络字节序转为主机字节序 int
//    void NetToHostInt(uint& h) {
//        h = ((h & 0x000000ff)<<24) | (h&0x0000ff00 << 8) | (h & 0x00ff0000 >> 8) | (h & 0xff000000 >> 24);
//    }

}
