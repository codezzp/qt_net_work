#include "addseriesdialog.h"
#include "ui_addseriesdialog.h"

AddSeriesDialog::AddSeriesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSeriesDialog)
{
    ui->setupUi(this);
    setWindowTitle("添加折线");
    setWindowIcon(QIcon(":/view.png"));
}

AddSeriesDialog::~AddSeriesDialog()
{
    delete ui;
}

void AddSeriesDialog::on_pushButton_2_clicked()
{
    this->close();
}


void AddSeriesDialog::on_pushButton_clicked()
{
    Bitno no = ui->lineEdit->text().toInt();
    emit Signal_CreateSeries(no);
    this->close();
}

