#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QTextEdit>
#include <QTextCodec>
#include <QCheckBox>
#include <QDomDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    checkDir();
    checkFiles();
    if(temTables==NULL){
        temTables=new templates();
    }
    this->setAttribute(Qt::WA_DeleteOnClose,true);
    ui->treeWidget_2->setColumnCount(1);
    FindgroupXml();
    bool isFind=false;
    for(int i=0;i<ui->comboBox->count();i++){
        if(ui->comboBox->itemText(i)=="default"){
            isFind=true;
            break;
        }
    }
    if(isFind==false){
        addGroupnode("default");
        ui->comboBox->clear();
        FindgroupXml();
    }
    connect(ui->action_10, &QAction::triggered, this, [=](){
        if (ui->action_10->isChecked()) {
            ui->toolBar_2->show();
        } else {
            ui->toolBar_2->hide();
        }
    });
    connect(ui->action_13, &QAction::triggered, this, [=](){
        if (ui->action_13->isChecked()) {
            ui->widget->show();
        } else {
            ui->widget->hide();
        }
    });
    connect(ui->action_11, &QAction::triggered, this, [=]() {
        if (ui->action_11->isChecked()) {
            ui->actionEnglish->setChecked(false);
        } else {
            ui->actionEnglish->setChecked(true);
        }
    });
    connect(ui->actionEnglish, &QAction::triggered, this, [=]() {
        if (ui->actionEnglish->isChecked()) {
            ui->action_11->setChecked(false);
        } else {
            ui->action_11->setChecked(true);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
    for(int i=0;i<tabConList.count();i++){
        tabConList.at(i)->deleteLater();
        tabConList.clear();
    }
    for(int i=0;i<tabSerList.count();i++){
        tabSerList.at(i)->deleteLater();
        tabSerList.clear();
    }
    if(temTables!=NULL){
        delete temTables;
        temTables=NULL;
    }
    exit(0);
}

void MainWindow::checkDir()
{
    if (!QDir("./favorites").exists())
    {
        if (QDir().mkdir("./favorites"))
        {}
        else
        {
            QMessageBox::warning(nullptr, "提示", "目录创建失败", QMessageBox::Ok);
        }
    }
}

void MainWindow::checkFiles()
{
    QFile tabfile("./favorites/tabs.xml");
    QFile groupfile("./favorites/groups.xml");
    QFile templatefile("./favorites/templates.xml");
    if (!tabfile.exists()) {
        if (tabfile.open(QIODevice::WriteOnly)) {
            QDomDocument xmlDocument;
            QDomNode xmlDeclaration = xmlDocument.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
            xmlDocument.appendChild(xmlDeclaration);
            QDomElement rootElement = xmlDocument.createElement("tabs");
            xmlDocument.appendChild(rootElement);
            QTextStream textStream(&tabfile);
            xmlDocument.save(textStream, 4);
            tabfile.close();
        } else {
            QMessageBox::warning(this,"失败","创建tab页文件失败",QMessageBox::Ok);
        }
    }
    if (!groupfile.exists()) {
        if (groupfile.open(QIODevice::WriteOnly)) {
            QDomDocument xmlDocument;
            QDomNode xmlDeclaration = xmlDocument.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
            xmlDocument.appendChild(xmlDeclaration);
            QDomElement rootElement = xmlDocument.createElement("groups");
            QDomElement groupElement = xmlDocument.createElement("group");
            groupElement.setAttribute("name", "default");
            rootElement.appendChild(groupElement);
            xmlDocument.appendChild(rootElement);
            QTextStream textStream(&groupfile);
            xmlDocument.save(textStream, 4);
            groupfile.close();
        } else {
            QMessageBox::warning(this,"失败","创建组文件失败",QMessageBox::Ok);
        }
    }
    if (!templatefile.exists()) {
        if (templatefile.open(QIODevice::WriteOnly)) {
            QDomDocument xmlDocument;
            QDomNode xmlDeclaration = xmlDocument.createProcessingInstruction("xml", "version='1.0' encoding='UTF-8'");
            xmlDocument.appendChild(xmlDeclaration);
            QDomElement rootElement = xmlDocument.createElement("templates");
            xmlDocument.appendChild(rootElement);
            QTextStream textStream(&templatefile);
            xmlDocument.save(textStream, 4);
            templatefile.close();
        } else {
            QMessageBox::warning(this,"失败","创建模板文件失败",QMessageBox::Ok);
        }
    }
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    bool isFind=false;
    for(int i=0;i<tabConList.count();i++){
        if(tabConList.at(i)==ui->tabWidget->widget(index)){
            ui->tabWidget->removeTab(index);
            QTreeWidgetItem *item = ui->treeWidget->topLevelItem(0);
            item->removeChild(item->child(i));
            Tab *tab=tabConList.at(i);
            tabConList.remove(i);
            tab->deleteLater();
            isFind=true;
            break;
        }
    }
    if(!isFind){
        for(int i=0;i<tabSerList.count();i++){
            if(tabSerList.at(i)==ui->tabWidget->widget(index)){
                ui->tabWidget->removeTab(index);
                QTreeWidgetItem *item = ui->treeWidget->topLevelItem(1);
                tabserver *tabserver=tabSerList.at(i);
                item->removeChild(item->child(i));
                tabSerList.remove(i);
                tabserver->deleteLater();
                break;
            }
        }
    }
}

void MainWindow::appendTab(QStringList listStr)
{
    Tab *tab=new Tab();
    connect(tab, SIGNAL(sendAddStore(QString)), this, SLOT(recvAddStore(QString)));
    connect(tab, SIGNAL(sendUpdteInfo(QString)), this, SLOT(recvConUpdateInfo(QString)));
    connect(this,SIGNAL(sendConTabInfo(QStringList)),tab,SLOT(setInfor(QStringList)));
    ui->tabWidget->addTab(tab,"（网络未连接）");
    tabConList.append(tab);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    emit sendConTabInfo(listStr);
    disconnect(this, SIGNAL(sendConTabInfo(QStringList)), 0, 0);
    this->disconnect(SIGNAL(sendConTabInfo(QStringList)));
}

void MainWindow::recvAddStore(QString name)
{
    if(name.isEmpty()){
        QMessageBox::warning(this,"提示","连接名不能为空",QMessageBox::Ok);
    }else{
        for(int i=0;i<ui->treeWidget_2->topLevelItemCount();i++){
            if(ui->treeWidget_2->topLevelItem(i)->text(0)==name){
                QMessageBox::warning(this,"提示","连接名不能重复",QMessageBox::Ok);
                return ;
            }
        }
        QTreeWidgetItem *item = new QTreeWidgetItem();
        item->setText(0,name);        
        item->setIcon(0,QIcon(":/new/prefix1/icon/savecon.png"));
        ui->treeWidget_2->addTopLevelItem(item);
        AddXmlFile(ui->tabWidget->currentIndex());
    }
}

void MainWindow::AddXmlFile(int index)
{
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
    file.close();
    QDomElement root = doc.documentElement();
    QDomElement group = doc.createElement("group");
    group.setAttribute("name",ui->comboBox->currentText());
    QDomElement name = doc.createElement("name");
    QDomText text = doc.createTextNode(ui->treeWidget_2->topLevelItem(ui->treeWidget_2->topLevelItemCount()-1)->text(0));
    name.appendChild(text);
    group.appendChild(name);
    QDomElement title = doc.createElement("title");
    QDomText text1 = doc.createTextNode(ui->tabWidget->tabText(index));
    title.appendChild(text1);
    group.appendChild(title);
    QWidget *widget = ui->tabWidget->currentWidget();
    QList<QLineEdit *> lineEdits = widget->findChildren<QLineEdit *>();
    foreach(QLineEdit *lineEdit, lineEdits) {
        QDomElement lineedit = doc.createElement("lineedit");
        lineedit.setAttribute("name",lineEdit->objectName());
        text = doc.createTextNode(lineEdit->text());
        lineedit.appendChild(text);
        group.appendChild(lineedit);
    }
    QList<QTextEdit *> textEdits = widget->findChildren<QTextEdit *>();
    foreach(QTextEdit *textEdit, textEdits) {
        QDomElement textedit = doc.createElement("textedit");
        textedit.setAttribute("name",textEdit->objectName());
        text = doc.createTextNode(textEdit->toPlainText());
        textedit.appendChild(text);
        group.appendChild(textedit);
    }
    QList<QCheckBox *> checkBoxs = widget->findChildren<QCheckBox *>();
    foreach(QCheckBox *checkBox, checkBoxs) {
        QDomElement checkbox = doc.createElement("checkbox");
        checkbox.setAttribute("name",checkBox->objectName());
        text = doc.createTextNode(QString::number(checkBox->isChecked()));
        checkbox.appendChild(text);
        group.appendChild(checkbox);
    }
    root.appendChild(group);
    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QTextStream stream(&file);
    doc.save(stream, 4);
    file.close();
}

void MainWindow::on_action_7_triggered()
{
    exit(0);
}


void MainWindow::on_action_triggered()
{
    CreateConnect *con = new CreateConnect();
    connect(con, SIGNAL(sendConInfo(QString)), this, SLOT(recvConInfo(QString)));
    con->show();
}

void MainWindow::recvConInfo(QString data)
{
    QStringList listStr=data.split(" ");
    appendTab(listStr);
    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(0);
    QTreeWidgetItem *item1 = new QTreeWidgetItem;
    item1->setIcon(0,QIcon(":/new/prefix1/icon/connect.png"));
    item1->setText(0,listStr[1]+":"+listStr[2]);
    item->addChild(item1);
    item->setExpanded(true);
}

void MainWindow::recvConUpdateInfo(QString data)
{
    QStringList lists=data.split(" ");
    if(lists.size()>1){
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),"（已连接）"+lists[0]);
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(0);
        for(int i=0;i<tabConList.count();i++){
            if(tabConList.at(i)==ui->tabWidget->widget(ui->tabWidget->currentIndex())){
                item->child(i)->setText(0,lists[1]+":"+lists[2]);
                break;
            }
        }
    }else{
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),"（网络未连接）"+lists[0]);
    }
}

void MainWindow::recvSerUpdateInfo(QString data)
{
    QStringList lists=data.split(" ");
    if(lists.size()>1){
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),"（已启动）"+lists[1]);
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(1);
        for(int i=0;i<tabSerList.count();i++){
            if(tabSerList.at(i)==ui->tabWidget->widget(ui->tabWidget->currentIndex())){
                item->child(i)->setText(0,lists[2]+":"+lists[3]);
                break;
            }
        }
    }else{
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),"（未启动）"+lists[0]);

    }
}

void MainWindow::on_action_2_triggered()
{
    CreateServer *server = new CreateServer();
    connect(server, SIGNAL(sendSerInfo(QString)), this, SLOT(recvSerInfo(QString)));
    server->show();
}

void MainWindow::recvSerInfo(QString data)
{
    QStringList listStr=data.split(" ");
    appendServerTab(listStr);
    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(1);
    QTreeWidgetItem *item1 = new QTreeWidgetItem;
    item1->setIcon(0,QIcon(":/new/prefix1/icon/connenctserver.png"));
    item1->setText(0,listStr[1]+":"+listStr[2]);
    item->addChild(item1);
    item->setExpanded(true);
}

void MainWindow::appendServerTab(QStringList listStr)
{
    tabserver *server=new tabserver();
    connect(server, SIGNAL(sendUpdteInfo(QString)), this, SLOT(recvSerUpdateInfo(QString)));
    connect(this,SIGNAL(sendSerTabInfo(QStringList)),server,SLOT(setInfor(QStringList)));
    ui->tabWidget->addTab(server,"（服务器未启动）");
    tabSerList.append(server);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    emit sendSerTabInfo(listStr);
    disconnect(this, SIGNAL(sendSerTabInfo(QStringList)), 0, 0);
    this->disconnect(SIGNAL(sendSerTabInfo(QStringList)));
}

void MainWindow::on_action_4_triggered()
{
    for(int i=0;i<tabSerList.count();i++){
        QPushButton *button=tabSerList.at(i)->findChild<QPushButton *>("pushButton_3");
        ui->tabWidget->setCurrentWidget(tabSerList.at(i));
        if(button->text()=="启动"){
            button->click();
        }
    }
    for(int i=0;i<tabConList.count();i++){
        QPushButton *button=tabConList.at(i)->findChild<QPushButton *>("pushButton_3");
        ui->tabWidget->setCurrentWidget(tabConList.at(i));
        if(button->text()=="连接"){
            button->click();
        }
    }
}


void MainWindow::on_action_6_triggered()
{
    for(int i=0;i<tabSerList.count();i++){
        QPushButton *button=tabSerList.at(i)->findChild<QPushButton *>("pushButton_3");
        ui->tabWidget->setCurrentWidget(tabSerList.at(i));
        if(button->text()=="关闭"){
            button->click();
        }
    }
    for(int i=0;i<tabConList.count();i++){
        QPushButton *button=tabConList.at(i)->findChild<QPushButton *>("pushButton_3");
        ui->tabWidget->setCurrentWidget(tabConList.at(i));
        if(button->text()=="关闭"){
            button->click();
        }
    }
}


void MainWindow::on_action_8_triggered()
{
    for(int i=tabConList.count()-1;i>=0;i--){
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabConList.at(i)));
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(0);
        item->removeChild(item->child(i));
        tabConList.remove(i);
    }
}


void MainWindow::on_action_9_triggered()
{
    for(int i=tabSerList.count()-1;i>=0;i--){
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(tabSerList.at(i)));
        QTreeWidgetItem *item = ui->treeWidget->topLevelItem(1);
        item->removeChild(item->child(i));
        tabSerList.remove(i);
    }
}


void MainWindow::on_action_12_triggered()
{
    QMessageBox box;
    box.setWindowIcon(QIcon(":/new/prefix1/icon/title.png"));
    box.setWindowTitle("About");
    box.setStandardButtons(QMessageBox::Ok);
    QString html="<html>"
                 "<table>"
                 "<tr>"
                 "<td><img src=':/new/prefix1/icon/title.png'></td>"
                 "<td><div></div></td>"
                 "</tr>"
                 "</table><p>本软件适用于TCP/IP、UDP/IP<br>power by LX</p>"
                 "</html>";
    box.setText(html);
    box.exec();
}


void MainWindow::on_pushButton_clicked()
{
    bool bOk=false;
    QString sName = QInputDialog::getText(this,"分组","请输入组名",QLineEdit::Normal,"",&bOk);
    for(int i=0;i<ui->comboBox->count();i++){
        if(sName==ui->comboBox->itemText(i)){
            QMessageBox::warning(this,"提示","已有该组名",QMessageBox::Ok);
            return ;
        }
    }
    if(sName==""){
        if(bOk){
            QMessageBox::warning(this,"提示","组名不能为空",QMessageBox::Ok);
        }
        return ;
    }
    ui->comboBox->addItem(sName);
    ui->comboBox->setCurrentText(sName);
    addGroupnode(sName);
}

void MainWindow::addGroupnode(QString groupname)
{
    QFile file("./favorites/groups.xml");
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
    QDomElement book = doc.createElement("group");
    book.setAttribute("name",groupname);
    root.appendChild(book);
    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QTextStream stream(&file);
    doc.save(stream, 4);
    file.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    if(ui->comboBox->currentText()!="default"){
        deletegroup(ui->comboBox->currentText());
        deletegroupnames(ui->comboBox->currentText());
        ui->comboBox->removeItem(ui->comboBox->currentIndex());        
    }else{
        ui->treeWidget_2->clear();
        deletegroupnames("default");
    }
}

void MainWindow::deletegroupnames(QString groupname)
{
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
    file.close();
    QDomElement root = doc.documentElement();
    QDomNodeList list = doc.elementsByTagName("group");
    for (int i = 0; i < list.size(); i++) {
        QDomElement element = list.at(i).toElement();
        if(element.attribute("name")==groupname){
            root.removeChild(element);
        }
    }
    if (!file.open(QFileDevice::WriteOnly | QFileDevice::Truncate)) {
        QMessageBox::information(NULL, "提示", "文件打开失败！");
        return;
    }
    QTextStream stream(&file);
    doc.save(stream, 4);
    file.close();
}

void MainWindow::on_treeWidget_2_itemClicked(QTreeWidgetItem *item, int column)
{
    Tab *tab=new Tab();
    connect(tab, SIGNAL(sendAddStore(QString)), this, SLOT(recvAddStore(QString)));
    connect(tab, SIGNAL(sendUpdteInfo(QString)), this, SLOT(recvConUpdateInfo(QString)));
    connect(this,SIGNAL(sendRestore(QString)),tab,SLOT(recvRestore(QString)));
    ui->tabWidget->addTab(tab,"（网络未连接）");
    tabConList.append(tab);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);
    QString str=ui->comboBox->currentText()+" "+item->text(column);
    emit sendRestore(str);
    QTreeWidgetItem *item1=ui->treeWidget->topLevelItem(0);
    QTreeWidgetItem *child=new QTreeWidgetItem();
    child->setText(0,item->text(column));
    child->setIcon(0,QIcon(":/new/prefix1/icon/connect.png"));
    item1->addChild(child);
    item1->setExpanded(true);
}


void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    QTreeWidgetItem *finditem=ui->treeWidget->topLevelItem(0);
    for(int i=0;i<finditem->childCount();i++){
        if(item==finditem->child(i)){
            for(int j=0;j<ui->tabWidget->count();j++){
                if(tabConList[i]==ui->tabWidget->widget(j)){
                    ui->tabWidget->setCurrentIndex(j);
                    return ;
                }
            }
        }
    }
    QTreeWidgetItem *finditem1=ui->treeWidget->topLevelItem(1);
    for(int i=0;i<finditem1->childCount();i++){
        if(item==finditem1->child(i)){
            for(int j=0;j<ui->tabWidget->count();j++){
                if(tabSerList[i]==ui->tabWidget->widget(j)){
                    ui->tabWidget->setCurrentIndex(j);
                    return ;
                }
            }
        }
    }
    qDebug()<<item->text(column);
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->treeWidget_2->clear();
    FindfromXml(arg1);
}

void MainWindow::FindfromXml(QString groupname)
{
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
            if (element.attribute("name") == groupname) {
                QDomNode name = element.namedItem("name");
                QTreeWidgetItem *item=new QTreeWidgetItem();
                item->setText(0,name.firstChild().nodeValue());
                item->setIcon(0,QIcon(":/new/prefix1/icon/savecon.png"));
                ui->treeWidget_2->addTopLevelItem(item);
            }
        }
    file.close();
}

void MainWindow::FindgroupXml()
{
    QFile file("./favorites/groups.xml");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QXmlStreamReader xmlReader(&file);
        while(!xmlReader.atEnd() && !xmlReader.hasError()) {
            QXmlStreamReader::TokenType token = xmlReader.readNext();
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if(token == QXmlStreamReader::StartElement) {
                if(xmlReader.name().toString() == "group") {
                    ui->comboBox->addItem(xmlReader.attributes().value("name").toString());
                }
            }
        }
    }
    file.close();
}

void  MainWindow::deletegroup(QString groupname)
{
    QFile file("./favorites/groups.xml");
    if(!file.open(QFile::ReadOnly| QIODevice::Text))
    {return;}
    QDomDocument doc;
    if(!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();
    QDomElement root=doc.documentElement();
    QDomNodeList list=doc.elementsByTagName("group");
    for(int i=0;i<list.count();i++)
    {
        QDomElement e=list.at(i).toElement();
        if(e.attribute("name")==groupname)
        {root.removeChild(e);}
    }
    if(!file.open(QFile::WriteOnly|QFile::Truncate))
    {return;}
    QTextStream out_stream(&file);
    doc.save(out_stream,4);
    file.close();
}

void MainWindow::deleteOne(QString nodeName)
{
    QFile xmlFile("./favorites/tabs.xml");
    if (!xmlFile.open(QIODevice::ReadOnly)) {
        return;
    }
    QDomDocument xmlDoc;
    if (!xmlDoc.setContent(&xmlFile)) {
        return;
    }
    xmlFile.close();
    QDomElement root = xmlDoc.documentElement();
    QDomNodeList groupNodes = root.elementsByTagName("group");
    for (int i = groupNodes.count() - 1; i >= 0; i--) {
        QDomElement groupElement = groupNodes.at(i).toElement();
        if (groupElement.attribute("name") == ui->comboBox->currentText()) {
            QDomNodeList nameNodes = groupElement.elementsByTagName("name");
            for (int j = nameNodes.count() - 1; j >= 0; j--) {
                QDomElement nameElement = nameNodes.at(j).toElement();
                if (nameElement.text() == nodeName) {
                    root.removeChild(groupElement);
                    break;
                }
            }
        }
    }
    QFile outputFile("./favorites/tabs.xml");
    if (!outputFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }

    QTextStream outputStream(&outputFile);
    xmlDoc.save(outputStream, 4);
    outputFile.close();
}

void MainWindow::on_treeWidget_2_customContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* currentItem = ui->treeWidget_2->itemAt(pos);
    if (currentItem && !currentItem->parent()) {
        QMenu menu;
        QAction* action1 = menu.addAction("删除");
        QAction* selectedAction = menu.exec(ui->treeWidget_2->mapToGlobal(pos));
        if (selectedAction == action1) {
            deleteOne(currentItem->text(0));
            delete currentItem;
        }
    }
}

void MainWindow::on_action_14_triggered()
{
    if(temTables!=NULL){
        temTables->show();
    }
}
