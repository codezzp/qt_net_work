#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <qt_mysql.h>
#include <QMessageBox>
#include<QSerialPort>
#include <QRegularExpressionValidator>
#include <QSerialPortInfo>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer *timer;
    QTime time;
    QSqlDatabase db;
    int Times,lineEditData;
private slots:


    void on_save_clicked();

    void on_openDir_clicked();

    void update();
    void restart();
    QSqlDatabase sql_connetction();
    void sql_insert();

    void on_btnopenport_2_clicked();

    void on_btncloseport_2_clicked();

    void on_btncleartext_2_clicked();

    void on_send_clicked();

    void on_textsend_2_textChanged();

    void on_stop_clicked();


    void on_sustain_2_clicked();

    void handleTimeout();

    void on_btnrefresh_2_clicked();

    void on_recv_save_btn_clicked();

    void on_recv_clear_btn_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_clicked();

    void on_checkBox_5_clicked();

    void TimerEvent();

    void serialPort_readyRead();

    void on_checkBox_4_clicked();

    void on_checkBox_6_clicked();

    void on_sendCount_editingFinished();

    void on_checkBox_3_clicked();

    void on_recv_pause_btn_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort m_serial;
    QSerialPort rec_serial;
    int nums=0; //累计发送次数
    int update_num;
    QTimer *timSend;
    QTimer *timCount;
    long int SendByte,ReceByte;//发送、接收字节数
    QStringList portStringLine;     //端口链表
    QByteArray Sendtext,Receivetext; //发送、接收缓存区
    QByteArray temp_buffer;

};
#endif // MAINWINDOW_H
