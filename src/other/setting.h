#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

private:
    Ui::Setting *ui;

signals:
    void sendSet(QString data);
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // SETTING_H
