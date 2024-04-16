#ifndef CREATESERVER_H
#define CREATESERVER_H

#include <QWidget>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QTcpServer>

namespace Ui {
class CreateServer;
}

class CreateServer : public QWidget
{
    Q_OBJECT

public:
    explicit CreateServer(QWidget *parent = nullptr);
    ~CreateServer();

private:
    Ui::CreateServer *ui;
    QStringList FindIpAddr();
    bool isIpOk(QString ip);
signals:
    void sendSerInfo(QString);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // CREATESERVER_H
