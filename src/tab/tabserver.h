#ifndef TABSERVER_H
#define TABSERVER_H

#include <QWidget>
#include <QtNetwork>
#include "other/form.h"
#include "other/setting.h"
#include "other/forward.h"
#include "template/templateentity.h"
#include <QMessageBox>
#include "other/form.h"
#include <QScrollBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QDomDocument>

struct set{
    int position=-1;
    int length=-1;
};

namespace Ui {
class tabserver;
}

class tabserver : public QWidget
{
    Q_OBJECT

public:
    explicit tabserver(QWidget *parent = nullptr);
    ~tabserver();

private:
    form *from=NULL;
    Ui::tabserver *ui;
    QUdpSocket *udpsocket=NULL;
    QTcpSocket *tcpsocket=NULL;
    QTcpServer *server=NULL;
    QUdpSocket *multiUdp=NULL;
    QUdpSocket *broadudp=NULL;
    Setting *set=NULL;
    Forward *forward=NULL;
    bool con;
    struct set Sett;
    struct sendType foa;
    QTcpSocket *senderTCP=NULL;
    QUdpSocket *senderUDP=NULL;
    QUdpSocket *senderBroadUDP=NULL;
    QUdpSocket *senderMultiUDP=NULL;
    templateEntity *entity=NULL;
    void insertTable(QString message);
    void sendForDatas(QByteArray datas,QString sendType,QString IpAddr);
    QByteArray templateToSend(QByteArray data);

private slots:
    void setInfor(QStringList listStr);
    void on_pushButton_3_clicked();
    void dowork();
    void on_pushButton_7_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void recvFindData(QByteArray data);

    void on_pushButton_12_clicked();

    void recvSet(QString);

    void recvForward(struct sendType);

    void on_pushButton_5_clicked();

    void sendForwardText(QByteArray datas);

    void recvCheckTemplate(int id);

signals:
    void sendUpdteInfo(QString infor);
    void sendDatas(char *data);
};

#endif // TABSERVER_H
