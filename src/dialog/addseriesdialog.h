#ifndef ADDSERIESDIALOG_H
#define ADDSERIESDIALOG_H

#include <QDialog>
#include "fmodchart/fmodchart.h"
namespace Ui {
class AddSeriesDialog;
}

class AddSeriesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddSeriesDialog(QWidget *parent = nullptr);
    ~AddSeriesDialog();

private:
    Ui::AddSeriesDialog *ui;
signals:
    void Signal_CreateSeries(Bitno no);
private slots:
    //取消按钮按下
    void on_pushButton_2_clicked();
    //确定按钮按下
    void on_pushButton_clicked();
};

#endif // ADDSERIESDIALOG_H
