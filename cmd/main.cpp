#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include "command.h"

void configFuncCB(std::string filePath, std::string content) {
    std::cout << "filePath " << filePath << "\ncontent " << content << std::endl;
}
void serialPortCB(std::string filePath, int second, int times) {
    std::cout << "filePath " << filePath << "\nsecond " << second << "\ntimes " << times << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        Command cmd(argc, argv, configFuncCB, serialPortCB);
        return cmd.Start();
    } else {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
