#ifndef CREATECONNECT_H
#define CREATECONNECT_H

#include <QWidget>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QTcpServer>

namespace Ui {
class CreateConnect;
}

class CreateConnect : public QWidget
{
    Q_OBJECT

public:
    explicit CreateConnect(QWidget *parent = nullptr);
    ~CreateConnect();

private slots:
    void on_pushButton_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_pushButton_2_clicked();

private:
    Ui::CreateConnect *ui;
    QStringList FindIpAddr();
    bool isIpOk(QString ip);

signals:
    void sendConInfo(QString);
};

#endif // CREATECONNECT_H
