#ifndef TEMPLATETABLES_H
#define TEMPLATETABLES_H

#include <QWidget>
#include "templates.h"
#include <QFile>
#include <QDomElement>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QTextEdit>
#include <QRadioButton>
#include <QLineEdit>
#include <QTableWidget>

namespace Ui {
class templateTables;
}

class templateTables : public QWidget
{
    Q_OBJECT

public:
    explicit templateTables(QWidget *parent = nullptr);
    ~templateTables();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::templateTables *ui;
    int updateId=-1;
    int checkId=-1;
    void AllReadTemplates();
    templates *tems=NULL;

signals:
    void sendOpMesg(QString message);
    void sendCheckId(int id);
};

#endif // TEMPLATETABLES_H
