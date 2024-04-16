#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    static SettingDialog *GetInstance();
    static void ReleaseInstance();
    ~SettingDialog();
signals:
    void Signal_Hopdata(bool isscancurrentdata,bool isappointdata,QVector<double> appointdata);
    void Signal_Hopdata(bool isscancurrentdata,bool isappointdata,double maxval,double minval);
private slots:
    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SettingDialog *ui;
    bool isscancurrentdata = true;
    bool isappointdata = false;
};

#endif // SETTINGDIALOG_H
