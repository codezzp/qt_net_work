#ifndef CREATECHARTFORMDIALOG_H
#define CREATECHARTFORMDIALOG_H

#include <QDialog>
#include "fmodchart/fmodchart.h"
namespace Ui {
class CreatechartFormDialog;
}

class CreatechartFormDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreatechartFormDialog(QWidget *parent = nullptr);
    ~CreatechartFormDialog();
    static CreatechartFormDialog *Getinstances();
    static void ReleaseInstance();
    //修改创建窗口参数
    bool ChangeChartFormParam(const FChartFormParam &cfp);
private:
    Ui::CreatechartFormDialog *ui;
    FChartFormParam cfp;
signals:
    void Signal_CreatechartForm(const FChartFormParam& cfp,Bitno no);
private slots:
    //确定按钮按下
    void on_pushButton_clicked();
    //取消按钮按下
    void on_pushButton_2_clicked();
    void on_checkBox_clicked();
    void on_checkBox_2_clicked();
};

#endif // CREATECHARTFORMDIALOG_H
