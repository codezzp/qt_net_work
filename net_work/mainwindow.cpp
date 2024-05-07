#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
//数据库依赖
#include <qt_mysql.h>
#include <QVariant>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <QFileDialog>
#include "conf.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    time.setHMS(0,0,0,0);//设置初始值
    timer = new QTimer(this);//创建一个定时器
    time.setHMS(0,0,0,0);//初始化
    ui->duration->setText(time.toString("hh:mm:ss"));
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));//手动连接槽函数
    //db = sql_connetction();

    on_btnrefresh_2_clicked();
    timSend=new QTimer(this);
    timCount=new QTimer(this);
    timCount->start(10);
    connect(timSend,&QTimer::timeout,this,&MainWindow::handleTimeout);
    connect(timCount,&QTimer::timeout,this,&MainWindow::TimerEvent);
    ui->checkBox_2->setCheckState(Qt::Checked);
    SendByte=ReceByte=0;
    Times=0;
    lineEditData=ui->lineInterval->text().toInt();
    ui->textsend_2->blockSignals(true);
    timCount->blockSignals(true);
    //将输入类型限制为5位非负数
    QRegularExpression regExp( "0|[1-9]\\d{0,4}" );
    ui->lineInterval->setValidator(new QRegularExpressionValidator(regExp,this));
    ui->sendCount->setValidator(new QRegularExpressionValidator(regExp,this));
    //禁止修改文本框
    ui->lineEdit_2->setReadOnly(true);
    ui->lineEdit->setReadOnly(true);
    ui->times->setReadOnly(true);
    ui->duration->setReadOnly(true);
    ui->receive_textEdit->setReadOnly(true);
    connect(&m_serial,&QSerialPort::readyRead,this,&MainWindow::serialPort_readyRead);
    ui->checkBox_3->setChecked(true);
    //connect(&rec_serial,&QSerialPort::readyRead,this,[=]{qDebug()<<rec_serial.portName()<<rec_serial.baudRate()<<" "<<rec_serial.dataBits()<<" "<<rec_serial.parity()<<" "<<rec_serial.stopBits();});
    //connect(&m_serial,&QSerialPort::readyRead,this,[=]{qDebug()<<rec_serial.portName()<<rec_serial.baudRate()<<" "<<rec_serial.dataBits()<<" "<<rec_serial.parity()<<" "<<rec_serial.stopBits();});
}

MainWindow::~MainWindow()
{
    delete ui;
}

QSqlDatabase MainWindow::sql_connetction(){
    /*//创建数据库连接
    if(!createConnection()) qDebug()<<("数据库连接失败");

        //指定某个数据库连接
    QSqlDatabase db1 = QSqlDatabase::database("connection1");
    return db1;*/

}
void MainWindow::sql_insert(){
    QSqlQuery query(db);//建立query对象

    // query.exec("SELECT * FROM qt_uart");
    // while (query.next()) {
    //     int id = query.value(0).toInt();
    //     QString date = query.value(1).toString();
    //     QString config = query.value(2).toString();
    //     QString file = query.value(3).toString();
    //     qDebug() << id << ": " <<date <<"----config_name: "<<config<<"------text:"<<file;
    // }//测试是否成功写入数据库


    QString config_name = ui->fileName->text();
    QString file_text = ui->textsend_2->toPlainText();
    qDebug()<<"config_name: "+config_name<<"text: "+file_text;
    QString sql = QString("insert into qt_uart(config_name,file) values('%1','%2')").arg(config_name).arg(file_text);
    if(!query.exec(sql))
    {qDebug() <<"位置绑定：" <<query.lastError() ;//如果执行不成功执行
    }
    else{qDebug() <<"写入数据库成功" ;}
    }

void MainWindow::update(){//计时器

    static quint32 time_out=0;
    time_out++;
    time=time.addSecs(1);
    ui->duration->setText(time.toString("hh:mm:ss"));
}
void MainWindow::restart(){//重置计时器
    timer->stop();
    time.setHMS(0,0,0,0);
    ui->duration->setText(time.toString("hh:mm:ss"));
}

void MainWindow::on_save_clicked() //保存当前编辑的报文到文本
{
    if (ui->fileName->text() == "fileName") {
        std::string fileName = ConfCore::StartConfigWrite(Command::ConfigPath, ui->textsend_2->toPlainText().toStdString());
        ui->fileName->setText("current file:"+QString(fileName.c_str()));
        QMessageBox::information(this, tr("提示"), tr("保存成功"));
    } else {
        QFile file(QString(Command::ConfigPath.c_str()));
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            return;

        QTextStream out(&file);
        out << ui->textsend_2->toPlainText();
        QMessageBox::information(this, tr("提示"), tr("保存成功"));
    }
}



void MainWindow::on_openDir_clicked()//打开文件夹
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("打开文件"), "./", tr("*.txt"));

    if (!fileName.isEmpty()) {
        Command::ConfigPath = fileName.toStdString();
        // 在这里，你可以使用fileName变量，它包含了用户选择的文件名
        QFileInfo fileInfo(fileName);
        QString name = fileInfo.fileName();
        ui->fileName->setText("current file:"+name);
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
        QString content = in.readAll();
        ui->textsend_2->setText(content);
    }

}



//刷新串口
void MainWindow::on_btnrefresh_2_clicked()
{
    //清空串口名
    ui->cmbportname_3->clear();
    //遍历串口信息
    /*foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        ui->cmbportname_3->addItem("/dev/"+info.portName());
    }*/
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())   //通过 QSerialPortInfo 查询 LInux 系统下的可用串口号；forreach 是增强的for循环，适用于循环次数未知的情况
    {
        ui->cmbportname_3->addItem("/dev/"+info.portName());
    }

    ui->cmbportbaud_3->setCurrentText("9600");
    ui->databits_3->setCurrentText("8");
    ui->cmbportparity_3->setCurrentText("0");
    ui->cmbportstop_3->setCurrentText("1");
    ui->status->setText("串口列表已刷新");
}


//打开串口
void MainWindow::on_btnopenport_2_clicked()
{
    //设置端口名
    m_serial.setPortName(ui->cmbportname_3->currentText());
    //设置波特率
    m_serial.setBaudRate(ui->cmbportbaud_3->currentText().toInt());
    //设置数据位
    switch(ui->databits_3->currentIndex())
    {
    case 5:
        m_serial.setDataBits(QSerialPort::Data5);
        break;
    case 6:
        m_serial.setDataBits(QSerialPort::Data6);
        break;
    case 7:
        m_serial.setDataBits(QSerialPort::Data7);
        break;
    case 8:
        m_serial.setDataBits(QSerialPort::Data8);
        break;
    default: break;
    }
    //设置校验位
    switch(ui->cmbportparity_3->currentIndex())
    {
    case 0:
        m_serial.setParity(QSerialPort::NoParity);
        break;
    case 1:
        m_serial.setParity(QSerialPort::OddParity);
        break;
    case 2:
        m_serial.setParity(QSerialPort::EvenParity);
        break;
    default: break;
    }
    //设置停止位
    switch(ui->cmbportstop_3->currentIndex())
    {
    case 1: m_serial.setStopBits(QSerialPort::OneStop); break;
    case 2: m_serial.setStopBits(QSerialPort::TwoStop); break;
    default: break;
    }
    //qDebug()<<m_serial.portName()<<m_serial.baudRate()<<" "<<m_serial.dataBits()<<" "<<m_serial.parity()<<" "<<m_serial.stopBits();
    //qDebug()<<rec_serial.portName()<<rec_serial.baudRate()<<" "<<rec_serial.dataBits()<<" "<<rec_serial.parity()<<" "<<rec_serial.stopBits();
    m_serial.open(QIODevice::ReadWrite);
    if(m_serial.isOpen()){
        ui->status->setText("串口成功打开");
    }
    else{
        ui->status->setText("串口打开失败");
        QByteArray lasttext;
        lasttext.append(("open fail，error info: "+m_serial.errorString().toStdString()+"\n"));
        ui->receive_textEdit->setText(lasttext);
        m_serial.close();
    }
}

//关闭串口
void MainWindow::on_btncloseport_2_clicked()
{
    m_serial.close();
    if(m_serial.isOpen()){
        ui->status->setText("串口关闭失败");
        QByteArray lasttext;
        lasttext.append(("open fail，error info: "+m_serial.errorString().toStdString()+"\n"));
        ui->receive_textEdit->setText(lasttext);
    }
    else{
        ui->status->setText("串口已关闭");
    }

}

//重置
void MainWindow::on_btncleartext_2_clicked()
{
    ui->textsend_2->clear();
    ui->times->setText("0");
    time.setHMS(0,0,0,0);//初始化
    ui->duration->setText(time.toString("hh:mm:ss"));
    nums=0;
    ui->lineEdit->setText("0");
    ui->lineEdit_2->setText("0");
    SendByte=ReceByte=0;


}

//发送报文
void MainWindow::on_send_clicked()
{

    if(m_serial.isOpen()){
        //是否周期发送
        if(ui->sustain_2->isChecked()==true){
            lineEditData=ui->lineInterval->text().toInt();
            timCount->blockSignals(false);
        }
        //次数是否启用
        if(ui->sustain_2->isChecked()==false&&ui->checkcycle_2->isChecked()==true){
            lineEditData=1000;
            timCount->blockSignals(false);
        }
        //更新剩余发送次数
        if(ui->checkcycle_2->isChecked()==true){
            ui->sendCount->setText(QString::number(ui->sendCount->text().toInt()-1));
        }
        QByteArray array1 = ui->textsend_2->toPlainText().toUtf8();
        if(ui->checkBox->checkState()==Qt::Checked)
            array1  = QByteArray::fromHex(array1);
        m_serial.write(array1);
        SendByte+=array1.length();
        //sql_insert();//写入数据库
        nums++;
        ui->times->setText(QString::number(nums));
        ui->lineEdit->setText(QString::number(SendByte));
        qDebug()<<m_serial.portName()<<m_serial.baudRate()<<" "<<m_serial.dataBits()<<" "<<m_serial.parity()<<" "<<m_serial.stopBits();
    }
    else{
        ui->status->setText("串口未打开");
    }
}



//监视文本框
void MainWindow::on_textsend_2_textChanged()
{
    //周期发送复选框
    bool valuesustain_2=ui->sustain_2->isChecked();
    //发送次数复选框
    bool valuetimes=ui->checkcycle_2->isChecked();
    int time=ui->lineInterval->text().toInt();
    int times=ui->sendCount->text().toInt();
    //判断串口是否打开，之后判断是否周期发送
    if(m_serial.isOpen()){
        if(valuesustain_2){
            //阻塞信号发送，避免周期过长时程序崩溃
            ui->textsend_2->blockSignals(true);
            timSend->start(time);
            if(valuetimes){
                if(times>0){
                    times--;
                    ui->sendCount->setText(QString::number(times));
                    QByteArray array1 = ui->textsend_2->toPlainText().toUtf8();
                    if(ui->checkBox->checkState()==Qt::Checked)
                        array1  = QByteArray::fromHex(array1);
                    m_serial.write(array1);
                    sql_insert();//写入数据库
                    SendByte+=array1.length();
                    ui->lineEdit->setText(QString::number(SendByte));
                    nums++;
                    ui->times->setText(QString::number(nums));
                }
            }
            else{
                QByteArray array1 = ui->textsend_2->toPlainText().toUtf8();
                m_serial.write(array1);
                sql_insert();//写入数据库
                SendByte+=array1.length();
                ui->lineEdit->setText(QString::number(SendByte));
                nums++;
                ui->times->setText(QString::number(nums));
            }
        }
    }

}

//停止发送
void MainWindow::on_stop_clicked()
{
    restart();//重置计时器

    timCount->blockSignals(true);
    m_serial.close();
    ui->status->setText("中止发送,串口已关闭");
}


//周期发送时禁止修改周期
void MainWindow::on_sustain_2_clicked()
{
    bool valuesustain_2=ui->sustain_2->isChecked();
    if(valuesustain_2){
        //对输入的值大小的限制，小于10会弹出对话框提示
        if(ui->lineInterval->text().toInt()>=10)
        {
            timSend->start(ui->lineInterval->text().toInt());//设置时长，重新计数
            //让时间输入框为不可选择。就是输入功能不可用，控件背景为灰色
            ui->lineInterval->setEnabled(false);

        }
        else {
            ui->sustain_2->setCheckState(Qt::Unchecked);
            QMessageBox::information(this,"错误提示","定时发送最小间隔为10ms!");
        }
    }
    else{
        timSend->stop();
        //时间输入框为可输入状态
        ui->lineInterval->setEnabled(true);
    }
}
//时间超时函数
void MainWindow::handleTimeout()
{
    ui->textsend_2->blockSignals(false);    //不阻塞信号
}




void MainWindow::on_recv_save_btn_clicked()
{
    std::string fileName = ConfCore::StartConfigWrite("recv_data.txt", ui->receive_textEdit->toPlainText().toStdString());
    if (!fileName.empty()) {
        QMessageBox::information(this, tr("提示"), tr("保存成功"));
    }
}



void MainWindow::on_recv_clear_btn_clicked()
{
    ui->receive_textEdit->clear();
}





void MainWindow::on_checkBox_clicked()
{
    ui->checkBox_2->setCheckState(Qt::Unchecked);
    ui->checkBox->setCheckState(Qt::Checked);
    QByteArray lasttext;
    temp_buffer=ui->textsend_2->toPlainText().toUtf8();
    lasttext=ui->textsend_2->toPlainText().toUtf8().toHex().toUpper();
    ui->textsend_2->setText(lasttext);
}


void MainWindow::on_checkBox_2_clicked()
{
    ui->checkBox_2->setCheckState(Qt::Checked);
    ui->checkBox->setCheckState(Qt::Unchecked);
    if(temp_buffer.size()!=0){
        ui->textsend_2->setText(temp_buffer);
    }
}


void MainWindow::on_checkBox_5_clicked()
{
    bool state=ui->checkBox_5->isChecked();
    if(state){
        ui->textsend_2->blockSignals(false);
    }
    else{
        ui->textsend_2->blockSignals(true);
    }
}
void MainWindow::TimerEvent()                                       //定时事件，10ms
{

    int times=ui->sendCount->text().toInt();
    Times+=10;
    if(ui->checkBox_5->isChecked()==false)
    {
        if(Times>=lineEditData)                                     //定时发送
        {
            if(ui->sustain_2->isChecked()==true){
                update_num+=lineEditData;
                if(ui->checkcycle_2->isChecked()==true){
                    if(times>0){
                        MainWindow::on_send_clicked();
                    }
                    else{
                        timCount->blockSignals(true);
                    }
                }
                else{
                    MainWindow::on_send_clicked();
                }
                if(update_num>1000){
                    //更新计时器
                    int n=update_num/1000;
                    for(int i=0;i<n;i++){
                        update();
                    }
                    update_num-=n*1000;
                }
            }
            else if(ui->checkcycle_2->isChecked()==true){
                if(times>0){
                    MainWindow::on_send_clicked();
                    update();
                }
                else{
                    timCount->blockSignals(true);
                }
            }
            Times=0;
        }
    }

}

void MainWindow::serialPort_readyRead()                             //串口接收
{
    if(ui->checkBox_6->isChecked()==false){
        int i,length;
        QByteArray lasttext;
        lasttext=ui->receive_textEdit->toPlainText().toUtf8();
        Receivetext = m_serial.readAll();
        qDebug()<<Receivetext;
        ReceByte+=Receivetext.length();
        ui->lineEdit_2->setText(QString::number(ReceByte));
        if(ui->checkBox_4->checkState()==Qt::Checked)
        {
            Receivetext=Receivetext.toHex().toUpper();          //字符串转十六进制
            length=Receivetext.length();
            for(i=0;i<=length/2;i++)
            {
                Receivetext.insert((2+3*i),' ');                //插入空格字符串
            }
        }
        lasttext=lasttext.append(Receivetext);
        ui->receive_textEdit->setText(lasttext);
    }
    m_serial.clear(QSerialPort::AllDirections);
}



void MainWindow::on_checkBox_4_clicked()    //十六进制接收
{
    ui->checkBox_6->setCheckState(Qt::Unchecked);
    ui->checkBox_4->setCheckState(Qt::Checked);
    ui->checkBox_3->setCheckState(Qt::Unchecked);
}


void MainWindow::on_checkBox_6_clicked()
{
    ui->checkBox_3->setCheckState(Qt::Unchecked);
    ui->checkBox_4->setCheckState(Qt::Unchecked);
    ui->checkBox_6->setCheckState(Qt::Checked);

}


void MainWindow::on_sendCount_editingFinished()
{
    lineEditData=ui->lineInterval->text().toInt();
}




void MainWindow::on_checkBox_3_clicked()
{
    ui->checkBox_3->setCheckState(Qt::Checked);
    ui->checkBox_4->setCheckState(Qt::Unchecked);
    ui->checkBox_6->setCheckState(Qt::Unchecked);
}


void MainWindow::on_recv_pause_btn_clicked()
{
    ui->checkBox_6->setCheckState(Qt::Checked);
}

