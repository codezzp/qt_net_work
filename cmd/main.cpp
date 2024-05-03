#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <iostream>
#include "command.h"
#include "conf.h"
#include "serial_port.h"

void configFuncCB(std::string filePath, std::string content) {
    std::cout << "filePath " << filePath << "\ncontent " << content << std::endl;
}
void serialportFuncCB(std::string filePath, int second, int bps, bool sendWay) {
    std::cout << "filePath: " << filePath << "\n"
              << "second: " << second << "\n"
              << "bps: " << bps << "\n"
              << "sendWay (Hex=0, Text=1): " << sendWay << std::endl;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // 创建应用程序对象

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "net_work_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    if (argc > 1) {
        QCoreApplication::setApplicationName("cmd");
        QCoreApplication::setApplicationVersion("1.0");

        Command cmd(argc, argv, ConfCore::Start, SerialPort::Start);
        int result = cmd.Start();
        return result;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
