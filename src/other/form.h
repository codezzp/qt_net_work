#ifndef FORM_H
#define FORM_H

#include <QDialog>
#include "fmodchart/icharform.h"

namespace Ui {
class form;
}

struct FindedDatas{
    char findedData[9999]="";
    bool isFind=false;
};

struct defineDataType{
    int moveLength=-1;
    char moveChar='\0';
};

struct DataTyepeFrame{
    char dataHeader[9999]="";
    int fixedLength=-1;
    int moveLength=-1;
    int dataType=-1;
    QList<struct defineDataType> items;
};

class form : public QDialog
{
    Q_OBJECT

public:
    explicit form(QWidget *parent = nullptr);
    ~form();
    void dealDatas();
    void dealMoveLength();
    void dealFixedLength();
    QByteArray QStringtoHex(const QString &str);

private slots:
    void on_pushButton_2_clicked();
    void recvDatas(char *data);

    void on_checkBox_stateChanged(int arg1);

    void on_checkBox_2_stateChanged(int arg1);

    void on_checkBox_3_stateChanged(int arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::form *ui;
    char BuffDatas[99999];
    ICharForm *charForm=NULL;
    FChartFormParam cfp;
    QMap<QString, int> dict = {{"无符号单字节", 1},{"有符号单字节",1},{"无符号双字节",2},{"有符号双字节",2},{"无符号四字节",4},{"有符号四字节",4},{"无符号八字节",8},{"有符号八字节",8}};
    struct DataTyepeFrame type;
    struct DataTyepeFrame used;
    struct FindedDatas FindDatas;

signals:
    void sendFindData(char *data);
};

#endif // FORM_H
