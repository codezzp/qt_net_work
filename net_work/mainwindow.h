#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <Qtime>
#include <qt_mysql.h>
#include <QMessageBox>
#include<QTest>
#include<QSerialPort>
#include<qserialportinfo>
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

    void on_checkcycle_2_clicked();

    void on_sustain_2_clicked();

    void handleTimeout();

    void on_btnrefresh_2_clicked();

    void on_recv_save_btn_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort m_serial;
    int nums=0;
    QTimer *timSend;
};
#endif // MAINWINDOW_H
