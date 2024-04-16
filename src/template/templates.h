#ifndef TEMPLATES_H
#define TEMPLATES_H

#include <QWidget>
#include <QFile>
#include <QDomDocument>
#include <QTreeWidget>
#include <QMessageBox>

namespace Ui {
class templates;
}

class templates : public QWidget
{
    Q_OBJECT

public:
    explicit templates(QWidget *parent = nullptr);
    ~templates();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

private:
    Ui::templates *ui;
    void AllReadXmlDatas();
    void getTemplate(int index);
    void updateTemplate(int index);
    void addTemplate();
    void deleteTemplate(int index);

signals:
    void refreshInfo();
};

#endif // TEMPLATES_H

