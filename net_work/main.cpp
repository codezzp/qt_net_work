#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "net_work_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();

    // QStringList drivers = QSqlDatabase::drivers(); //获取到qt中所支持的数据库类型

    // foreach(QString driver,drivers)
    // {
    //     qDebug()<<driver;
    // }
    // // 显示可用的sql驱动

    return a.exec();
}
