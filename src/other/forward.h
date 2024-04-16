#ifndef FORWARD_H
#define FORWARD_H

#include <QDialog>
#include "template/templatetables.h"
#include <QInputDialog>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QAbstractSocket>

namespace Ui {
class Forward;
}

struct type{
    QString sendType="";
    QString ipAddr="";
};

struct sendType{
    int select=-1;
    int templateId=-1;
    QList<struct type> sendLOcations;
};

class Forward : public QDialog
{
    Q_OBJECT

public:
    explicit Forward(QWidget *parent = nullptr);
    ~Forward();

private:
    Ui::Forward *ui;
    templateTables *table=NULL;
    void getCon();
    struct sendType type;

signals:
    void sendForward(struct sendType);
    void openTemTable();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_radioButton_2_clicked();
    void recvCheckTemplate(int id);
};

#endif // FORWARD_H
