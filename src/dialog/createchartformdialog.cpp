#include "createchartformdialog.h"
#include "ui_createchartformdialog.h"

namespace
{
    CreatechartFormDialog *m_instance=nullptr;
}

CreatechartFormDialog::CreatechartFormDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreatechartFormDialog)
{
    ui->setupUi(this);
    setWindowTitle("创建窗口");
    setWindowIcon(QIcon(":/view.png"));
}

CreatechartFormDialog::~CreatechartFormDialog()
{
    delete ui;
}

CreatechartFormDialog *CreatechartFormDialog::Getinstances()
{
    if(m_instance == nullptr)
    {
        m_instance = new CreatechartFormDialog();
    }

    return m_instance;
}

void CreatechartFormDialog::ReleaseInstance()
{
    if(m_instance != nullptr)
    {
       delete m_instance;
    }
    m_instance = nullptr;
}

bool CreatechartFormDialog::ChangeChartFormParam(const FChartFormParam &cfp)
{
    this->cfp = cfp;
}

void CreatechartFormDialog::on_pushButton_clicked()
{
    cfp.x = 700;
    cfp.y = 20 + 50;
    Bitno no = ui->lineEdit->text().toInt();
    //设置
    cfp._datalong = ui->lineEdit_2->text().toInt();
    cfp.title = ui->lineEdit_3->text();
    QString combox_number=ui->comboBox->currentText().mid(3,1);
    QString combox_sign=ui->comboBox->currentText().mid(0,1);
    if(combox_number == "单")
    {
        cfp.byte = ONE_BYTE;
    }
    else if(combox_number == "双")
    {
        cfp.byte = TWO_BYTE;
    }
    else if(combox_number == "四")
    {
        cfp.byte = FORE_BYTE;
    }
    else if(combox_number == "八")
    {
        cfp.byte = EIGHT_BYTE;
    }
    if(combox_sign == "有")
    {
        cfp.Isunsigned = false;
    }
    else if(combox_sign == "无")
    {
        cfp.Isunsigned = true;
    }
    qDebug() << "[CreateChartFormDialog]on_pushubutton: emit Signal_CreatechartForm";
    emit Signal_CreatechartForm(cfp,no);
    this->close();
}

void CreatechartFormDialog::on_pushButton_2_clicked()
{
    this->close();
}

void CreatechartFormDialog::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked())
    {
        ui->checkBox_2->setChecked(false);
        cfp.IsAxisTimeForm_X = true;
    }

}

void CreatechartFormDialog::on_checkBox_2_clicked()
{
    if(ui->checkBox_2->isChecked())
    {
        ui->checkBox->setChecked(false);
        cfp.IsAxisTimeForm_X = false;
    }
}

