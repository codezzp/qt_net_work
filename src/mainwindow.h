#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "tab/tab.h"
#include "other/createconnect.h"
#include "other/createserver.h"
#include "tab/tabserver.h"
#include <QTreeWidgetItem>
#include "template/templates.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    templates *temTables=NULL;
    void appendTab(QStringList listStr);
    void appendServerTab(QStringList listStr);
    QList<Tab *> tabConList;
    QList<tabserver *> tabSerList;
    void FindfromXml(QString groupname);
    void FindgroupXml();
    void deletegroup(QString groupname);
    void addGroupnode(QString groupname);
    void deletegroupnames(QString gtoupname);
    void checkFiles();
    void checkDir();
    void deleteOne(QString nodeName);
private slots:
    void on_tabWidget_tabCloseRequested(int index);
    void on_action_7_triggered();
    void on_action_triggered();
    void recvConInfo(QString data);
    void recvConUpdateInfo(QString data);
    void on_action_2_triggered();
    void recvSerInfo(QString data);
    void recvSerUpdateInfo(QString data);
    void on_action_4_triggered();

    void on_action_6_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

    void on_action_12_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void recvAddStore(QString name);
    void AddXmlFile(int index);

    void on_treeWidget_2_itemClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_treeWidget_2_customContextMenuRequested(const QPoint &pos);

    void on_action_14_triggered();

signals:
    void sendConTabInfo(QStringList);
    void sendSerTabInfo(QStringList);
    void sendRestore(QString);
};
#endif // MAINWINDOW_H
