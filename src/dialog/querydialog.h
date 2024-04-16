#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = nullptr);
    static QueryDialog *GetInstance();
    static void ReleaseInstance();
    ~QueryDialog();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

signals:
    void Signal_Querytext(QString querytype,QString queryvalue);
private:
    Ui::QueryDialog *ui;
};

#endif // QUERYDIALOG_H
