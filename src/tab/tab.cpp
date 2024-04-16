#include "tab.h"
#include "ui_tab.h"

Tab::Tab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tab)
{
    ui->setupUi(this);
    udp=new Udp();
    tcp=new Tcp();
    multudp=new MuliteUdp();
    broadudp=new broadcastudp();
    connect(ui->textEdit_2, &QTextEdit::textChanged, [=]() {
        ui->textEdit_2->moveCursor(QTextCursor::End);
    });
    connect(ui->textEdit_3, &QTextEdit::textChanged, [=]() {
        ui->textEdit_3->moveCursor(QTextCursor::End);
    });
}

Tab::~Tab()
{
    if(thread!=NULL){
        timer->deleteLater();
        thread->quit();
        thread->wait();
        timer->stop();
        timer=NULL;
        delete thread;
        thread=NULL;
    }
    if(from!=NULL){
        from->deleteLater();
        from=NULL;
    }
    if(udp!=NULL){
        delete udp;
        udp=NULL;
    }
    if(tcp!=NULL){
        delete tcp;
        tcp=NULL;
    }
    if(multudp!=NULL){
        delete multudp;
        multudp = NULL;
    }
    if(broadudp!=NULL){
        delete broadudp;
        broadudp=NULL;
    }
    if(entity!=NULL){
        delete entity;
        entity=NULL;
    }
    if(table!=NULL){
        delete table;
        table=NULL;
    }
    delete ui;
}

void Tab::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->lineEdit_6->setEnabled(false);
    }else{
        ui->lineEdit_6->setEnabled(true);
    }
}


void Tab::on_checkBox_2_stateChanged(int arg1)
{
    if(arg1 == 2)
    {
        ui->lineEdit_5->setEnabled(false);
    }else{
        ui->lineEdit_5->setEnabled(true);
    }
}

void Tab::setInfor(QStringList listStr)
{
    ui->comboBox_2->setCurrentText(listStr[0]);
    ui->lineEdit_2->setText(listStr[1]);
    ui->lineEdit_3->setText(listStr[2]);
    ui->lineEdit_6->setText(listStr[3]);
    ui->lineEdit_5->setText(listStr[4]);
    if(!listStr[3].isEmpty()){
        ui->checkBox->setChecked(true);
        ui->lineEdit_6->setEnabled(false);
    }
    if(!listStr[4].isEmpty()){
        ui->checkBox_2->setChecked(true);
        ui->lineEdit_5->setEnabled(false);
    }

}

void Tab::on_pushButton_3_clicked()
{
    if(ui->pushButton_3->text()=="连接"){
        emit(sendUpdteInfo(ui->lineEdit->text()+" "+ui->lineEdit_2->text()+" "
                           +ui->lineEdit_3->text()+" "+ui->lineEdit_6->text()+" "
                           +ui->lineEdit_5->text()+" "+ui->comboBox_2->currentText()));
        ui->pushButton_7->setDisabled(false);
        ui->checkBox_3->setEnabled(true);
        ui->lineEdit->setDisabled(true);
        ui->lineEdit_2->setDisabled(true);
        ui->lineEdit_3->setDisabled(true);
        ui->lineEdit_6->setDisabled(true);
        ui->lineEdit_5->setDisabled(true);
        ui->comboBox_2->setDisabled(true);
        ui->checkBox->setDisabled(true);
        ui->checkBox_2->setDisabled(true);
        if(ui->comboBox_2->currentText()=="UDP组播")
        {
            multudp->create_Mulitudp(ui->lineEdit_3->text().toInt(),ui->lineEdit_2->text());
        }
        else if(ui->comboBox_2->currentText()=="UDP单播")
        {
            udp->create_udp(ui->lineEdit_6->text(),ui->lineEdit_5->text().toInt());
        }
        else if(ui->comboBox_2->currentText()=="TCP客户端")
        {
            tcp->create_tcp(ui->lineEdit_6->text(),ui->lineEdit_5->text().toInt());
        }else{
            broadudp->create_broadudp(ui->lineEdit_6->text(),ui->lineEdit_5->text().toInt());
        }
        ui->pushButton_6->setDisabled(false);
        ui->pushButton_3->setText("关闭");
    }else{
        emit(sendUpdteInfo(ui->lineEdit->text()));
        ui->checkBox_3->setEnabled(false);
        ui->pushButton_7->setDisabled(true);
        ui->lineEdit->setDisabled(false);
        ui->lineEdit_2->setDisabled(false);
        ui->lineEdit_3->setDisabled(false);
        ui->lineEdit_6->setDisabled(false);
        ui->lineEdit_5->setDisabled(false);
        ui->comboBox_2->setDisabled(false);
        ui->checkBox->setDisabled(false);
        ui->checkBox_2->setDisabled(false);
        if(ui->comboBox_2->currentText()=="UDP组播")
        {
            multudp->exit_Mulitudp();
        }
        else if(ui->comboBox_2->currentText()=="UDP单播")
        {
            udp->exit_udp();
        }
        else if(ui->comboBox_2->currentText()=="TCP客户端")
        {
            tcp->exit_tcp();
        }
        else{
            broadudp->exit_broadudp();
        }
        ui->checkBox_3->setChecked(false);
        ui->pushButton_3->setText("连接");
        ui->pushButton_6->setDisabled(true);
        ui->lineEdit_4->setText("0");
        ui->lineEdit_7->setText("0");
    }
}


void Tab::on_pushButton_7_clicked()
{
    QString str=ui->textEdit_2->toPlainText();
    QByteArray ba;
    QByteArray str1="";
    QFile f(ui->lineEdit_9->text());
    if(ui->checkBox_5->isChecked())
    {
        str=readfile();
        if(!str.isEmpty())
        {QMessageBox::question(this,tr("提示"),tr("发送成功"),QMessageBox::Ok,QMessageBox::Ok);}
    }
    ba=str.toLocal8Bit();
    if(ui->checkBox_4->isChecked())
    {
        ba.clear();
        QStringList lst=str.split(" ");
        uint8_t temp;
        bool ok;
        foreach(QString s,lst){
            temp=(uint8_t)s.toInt(&ok,16);
            ba.append((char)temp);
        }
    }
    if(ui->checkBox_8->isChecked()){
        if(entity==NULL){
            QMessageBox::warning(this,tr("提示"),"未选择模板",QMessageBox::Ok);
            ui->checkBox_8->setChecked(false);
            ui->checkBox_3->setChecked(false);
            return ;
        }else{
            ba=templateToSend(ba);
        }
    }
    if(ui->comboBox_2->currentText()=="UDP组播")
    {
        connect(multudp->getMultiSocket(), &QUdpSocket::readyRead, this, [=]() {
            QString receivedMessage;
            while (multudp->getMultiSocket()->hasPendingDatagrams()) {
                QByteArray datagram;
                datagram.resize(multudp->getMultiSocket()->pendingDatagramSize());
                QHostAddress senderAddress;
                quint16 senderPort;
                multudp->getMultiSocket()->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
                receivedMessage = QString::fromLocal8Bit(datagram);
                ui->textEdit_3->insertPlainText(receivedMessage);
            }
        });
        multudp->send(ba,ui->lineEdit_5->text().toInt());
        ui->lineEdit_4->setText(QString::number(ui->lineEdit_4->text().toInt()+ba.length()));
    }else if(ui->comboBox_2->currentText()=="UDP单播"){
        connect(udp->getUdpSocket(), &QTcpSocket::readyRead, this, &Tab::onReadyRead);
        udp->send(ba,ui->lineEdit_3->text().toInt(),ui->lineEdit_2->text());
        ui->lineEdit_4->setText(QString::number(ui->lineEdit_4->text().toInt()+ba.length()));
    }else if(ui->comboBox_2->currentText()=="TCP客户端"){
        connect(tcp->getTcpSOcket(), &QTcpSocket::readyRead, this, [=]() {
            QByteArray receiveData = tcp->getTcpSOcket()->readAll();
            QString response = QString::fromLocal8Bit(receiveData);
            ui->textEdit_3->insertPlainText(response);
        });
        tcp->send(ba,ui->lineEdit_3->text().toInt(),ui->lineEdit_2->text());
        ui->lineEdit_4->setText(QString::number(ui->lineEdit_4->text().toInt()+ba.length()));
    }
    else{
        connect(broadudp->getBroadUdpSocket(), &QTcpSocket::readyRead, this, [=]{
            QString receivedMessage;
            while (broadudp->getBroadUdpSocket()->hasPendingDatagrams())
            {
                QByteArray datagram;
                datagram.resize(broadudp->getBroadUdpSocket()->pendingDatagramSize());
                QHostAddress senderAddress;
                quint16 senderPort;
                broadudp->getBroadUdpSocket()->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
                receivedMessage = QString::fromLocal8Bit(datagram);
                ui->textEdit_3->insertPlainText(receivedMessage);
            }
        });
        broadudp->send(ba,ui->lineEdit_3->text().toInt(),ui->lineEdit_2->text());
        ui->lineEdit_4->setText(QString::number(ui->lineEdit_4->text().toInt()+ba.length()));
    }

    if(ui->comboBox_2->currentText()=="UDP组播")
    {
        str1=multudp->recv();
        ui->lineEdit_7->setText(QString::number(ui->lineEdit_7->text().toInt()+str1.length()));
    }else if(ui->comboBox_2->currentText()=="UDP单播"){
        str1=udp->receive();
        ui->lineEdit_7->setText(QString::number(ui->lineEdit_7->text().toInt()+str1.length()));
    }else if(ui->comboBox_2->currentText()=="TCP客户端"){
        str1=tcp->recv();
        char *sendData=new char[str1.length()+1];
        strcpy(sendData,str1.data());
        emit sendDatas(sendData);
        delete[] sendData;
        ui->lineEdit_7->setText(QString::number(ui->lineEdit_7->text().toInt()+str1.length()));
    }else{
        str1=broadudp->receive();
        ui->lineEdit_7->setText(QString::number(ui->lineEdit_7->text().toInt()+str1.length()));
    }
    QString message=QString::fromLocal8Bit(str1.data());
    if(ui->checkBox_6->isChecked())
    {
        message="";
        QByteArray buff=str1;
        QString t="";
        for(int i=0;i<str1.length();i++){
            t=QString("%1").arg((uint8_t)buff[i],2,16,QLatin1Char('0'));
            message=message+t+" ";
        }
    }
    if(ui->pushButton_10->text()=="暂停显示")
    {
        ui->textEdit_3->insertPlainText(message);
    }
    if(ui->checkBox_7->isChecked())
    {
        if(ui->lineEdit_9->text().isEmpty())
        {QMessageBox::question(this,tr("提示"),tr("找不到文件"),QMessageBox::Ok,QMessageBox::Ok);}
        else
        {
            f.open(QIODevice::WriteOnly|QIODevice::Append);
//            f.write(str1.toUtf8());
        }
    }
    if(ui->checkBox_3->isChecked()==false)
    {f.close();}
}

QString Tab::readfile()
{
    QByteArray bytes;
    QString path = QFileDialog::getOpenFileName(this,tr("打开"),"",tr("ALL Files(*)"));
    if(path.isEmpty() == false)
    {
        QFile file(path);
        bool isok = file.open(QIODevice::ReadOnly);
        if(isok == true)
        {
            bytes =  file.readAll();
        }
        file.close();
    }
    return QString(bytes);
}

void Tab::on_pushButton_11_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("另存为"),"",tr("ALL Files(*)"));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QString arr = ui->textEdit_3->toPlainText();
        file.write(arr.toUtf8());
        QMessageBox::question(this,tr("提示"),tr("保存成功"),QMessageBox::Ok,QMessageBox::Ok);
        file.close();
    }
    else
    {}
}


void Tab::on_pushButton_14_clicked()
{
    ui->textEdit_3->setText("");
}


void Tab::on_pushButton_8_clicked()
{
    ui->textEdit_2->setText("");
}


void Tab::on_pushButton_10_clicked()
{
    if(ui->pushButton_10->text()=="继续显示"){
        ui->pushButton_10->setText("暂停显示");
    }else{
        ui->pushButton_10->setText("继续显示");
    }
}

void Tab::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this,tr("Open File"),"","", 0);
    ui->lineEdit_9->setText(file_name);
}


void Tab::on_pushButton_9_clicked()
{
    Length *length=new Length();
    connect(length, SIGNAL(sendLengthInfo(QString)), this, SLOT(recvLengthInfo(QString)));
    length->show();
}

void Tab::recvLengthInfo(QString length)
{
    this->length=length.toInt();
}


void Tab::on_pushButton_4_clicked()
{
    ui->lineEdit_4->setText("0");
    ui->lineEdit_7->setText("0");
}


void Tab::on_pushButton_5_clicked()
{
    emit sendAddStore(ui->lineEdit->text());
}

void Tab::recvRestore(QString str)
{
    QString group=str.split(" ")[0];
    QString name=str.split(" ")[1];
    QFile file("./favorites/tabs.xml");
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
    QDomElement root = doc.documentElement();
    QDomNodeList list = root.elementsByTagName("group");
    for (int i = 0; i < list.count(); i++) {
        QDomElement element = list.at(i).toElement();
        if (element.attribute("name") == group) {
            QDomNodeList nodeList = element.childNodes();
            if(nodeList.at(0).firstChild().nodeValue()==name){
                for(int i=2;i<nodeList.count();i++){
                    if(nodeList.at(i).nodeName()=="lineedit"){
                        QLineEdit *line=this->findChild<QLineEdit *>(nodeList.at(i).toElement().attribute("name"));
                        line->setText(nodeList.at(i).firstChild().nodeValue());
                    }else if(nodeList.at(i).nodeName()=="textedit"){
                        QTextEdit *text=this->findChild<QTextEdit *>(nodeList.at(i).toElement().attribute("name"));
                        text->setText(nodeList.at(i).firstChild().nodeValue());
                    }else if(nodeList.at(i).nodeName()=="checkbox"){
                        QCheckBox *check=this->findChild<QCheckBox *>(nodeList.at(i).toElement().attribute("name"));
                        if(nodeList.at(i).firstChild().nodeValue()=="1"){
                            check->setChecked(true);
                        }else{
                            check->setChecked(false);
                        }
                    }
                }
            }
        }
    }
    file.close();
}


void Tab::on_checkBox_3_stateChanged(int arg1)
{
    if(arg1==2){
        thread=new QThread();
        timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(on_pushButton_7_clicked()));
        int inter=ui->lineEdit_8->text().toInt();
        if(ui->lineEdit_8->text()==""){
            inter=1000;
        }
        timer->setInterval(inter);
        timer->setParent(NULL);
        timer->moveToThread(thread);
        connect(thread, &QThread::started, timer, static_cast<void (QTimer::*)()>(&QTimer::start));
        thread->start();
        ui->lineEdit_8->setEnabled(false);
        ui->pushButton_7->setEnabled(false);
    }else{
        timer->deleteLater();
        thread->quit();
        thread->wait();
        timer->stop();
        timer=NULL;
        delete thread;
        thread=NULL;
        ui->lineEdit_8->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
    }
}

void Tab::on_pushButton_13_clicked()
{
    if(from==NULL){
        from=new form();
    }
    from->show();
    connect(this,SIGNAL(sendDatas(char*)),from,SLOT(recvDatas(char*)));
}

void Tab::onReadyRead()
{
    QByteArray datagram;
    datagram.resize(udp->getUdpSocket()->pendingDatagramSize());
    udp->getUdpSocket()->readDatagram(datagram.data(),datagram.size());
    QString message=QString::fromLocal8Bit(datagram.data());
    ui->textEdit_3->insertPlainText(message);
}

void Tab::recvCheckTemplate(int id)
{
    if(table!=NULL){
        delete table;
        table=NULL;
    }
    ui->checkBox_8->setChecked(true);
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

QByteArray Tab::templateToSend(QByteArray data)
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
                QByteArray midData=data.mid(init.beginLocation,init.endLocation-init.beginLocation+1);
                temDataByte.replace(tem.beginLocation,(tem.endLocation-tem.beginLocation)+1,midData);
            }else{
                QByteArray midData=data.mid(init.beginLocation,tem.endLocation-tem.beginLocation+1);
                temDataByte.replace(tem.beginLocation,(tem.endLocation-tem.beginLocation)+1,midData);
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
}

void Tab::on_checkBox_8_stateChanged(int arg1)
{
    if(arg1==2){
        if(table==NULL){
            table=new templateTables();
            connect(table,SIGNAL(sendCheckId(int)),this,SLOT(recvCheckTemplate(int)));
        }
        table->show();
        ui->checkBox_8->setChecked(false);
    }
}

