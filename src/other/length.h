#ifndef LENGTH_H
#define LENGTH_H

#include <QDialog>

namespace Ui {
class Length;
}

class Length : public QDialog
{
    Q_OBJECT

public:
    explicit Length(QWidget *parent = nullptr);
    ~Length();

private:
    Ui::Length *ui;
signals:
   void sendLengthInfo(QString);
private slots:
   void on_pushButton_2_clicked();
   void on_pushButton_clicked();
};

#endif // LENGTH_H
