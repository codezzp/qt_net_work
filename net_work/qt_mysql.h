#ifndef QT_MYSQL_H
#define QT_MYSQL_H


#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QDebug>

static bool createConnection(){
    /*//连接第一个数据库
    //QMYSQL
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "connection1");//需要使用的数据库驱动和联检建立的名称（方便建立多个数据库连接【使用不同的数据库时】区分）
    db.setHostName("127.0.0.1");//连接地址
    db.setUserName("root");//数据库账户
    db.setPassword("mysqlvipfor(zzp)");//密码
    db.setPort(3306);//端口
    db.setDatabaseName("db_student");//需要用到的数据库

    if (!db.open()) {//如果数据库连接失败，则弹出
        //critical(QWidget *parent, const QString &title,
        //const QString &text,
        //QMessageBox::StandardButtons buttons = Ok,
        //QMessageBox::StandardButton defaultButton = NoButton)
        QMessageBox::critical(0, "Cannot open database",
                              "Unable to establish a database connection", QMessageBox::Cancel);
        return false;
    }
    */
    return true;
}




#endif // QT_MYSQL_H
