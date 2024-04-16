#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include "connect/udp.h"
#include "connect/muliteudp.h"
#include "connect/tcp.h"
#include "other/length.h"
#include "connect/broadcastudp.h"
#include "template/templateentity.h"
#include "template/templatetables.h"
#include "other/form.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDomDocument>

namespace Ui {
class Tab;
}

class Tab : public QWidget
{
    Q_OBJECT

public:
    explicit Tab(QWidget *parent = nullptr);
    ~Tab();
    QString readfile();

private slots:
    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);
    void setInfor(QStringList listStr);
    void on_pushButton_3_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_clicked();

    void on_pushButton_9_clicked();

    void recvLengthInfo(QString length);
    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void recvRestore(QString str);

    void on_checkBox_3_stateChanged(int arg1);

    void on_pushButton_13_clicked();

    void onReadyRead();

    void recvCheckTemplate(int id);

    void on_checkBox_8_stateChanged(int arg1);

private:
    Ui::Tab *ui;
    Tcp *tcp=NULL;
    Udp *udp=NULL;
    broadcastudp *broadudp=NULL;
    MuliteUdp *multudp=NULL;
    int length=-1;
    QTimer *timer=NULL;
    form *from=NULL;
    QThread *thread=NULL;
    templateEntity *entity=NULL;
    templateTables *table=NULL;
    QByteArray templateToSend(QByteArray data);
signals:
    void sendUpdteInfo(QString infor);
    void sendAddStore(QString name);
    void sendDatas(char *data);
};

#endif // TAB_H
