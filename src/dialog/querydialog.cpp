#include "querydialog.h"
#include "ui_querydialog.h"

QueryDialog::QueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
    setWindowTitle("跳变查询");
    setWindowIcon(QIcon(":/view.png"));
}

QueryDialog::~QueryDialog()
{
    delete ui;
}

void QueryDialog::on_pushButton_clicked()
{
    QString curtype =ui->comboBox->currentText();
    QString curvalue =ui->lineEdit_2->text();
    emit Signal_Querytext(curtype,curvalue);
    close();
}


void QueryDialog::on_pushButton_2_clicked()
{
    close();
}

