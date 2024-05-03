#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QCoreApplication>
#include <QSerialPort>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QTimer>
#include "command.h"

class SerialPort {
public:
    static void Start(std::string filePath, int second, int bps, bool sendWay) {
        QSerialPort serialPort;
        serialPort.setPortName(Command::SerialPortName.c_str()); // 根据需求调整串口名称
        serialPort.setBaudRate(bps);
        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setStopBits(QSerialPort::OneStop);

        if (!serialPort.open(QIODevice::ReadWrite)) {
            qCritical() << "Failed to open port" << serialPort.portName() << ", error:" << serialPort.errorString();
            return;
        }

        QFile file(QString::fromStdString(filePath));
        if (!file.open(QIODevice::ReadOnly)) {
            qCritical() << "Failed to open file" << file.fileName() << ", error:" << file.errorString();
            return;
        }

        QTextStream in(&file);
        QString data = in.readAll(); // 读取整个文件
        file.close(); // 读取完成后即可关闭文件

        QTimer *timer = new QTimer;
        QObject::connect(timer, &QTimer::timeout, [&serialPort, data, sendWay]() mutable {
            QByteArray last_text = data.toUtf8();
            // 发送十六进制
            if (!sendWay) {
                last_text = last_text.toHex().toUpper();
            }
            std::cout << "current send data:" << last_text.toStdString() << std::endl;
            serialPort.write(last_text);
            if (!serialPort.waitForBytesWritten(-1)) {
                qCritical() << "Failed to write data";
            }
            if (!serialPort.flush()) { // 尝试flush数据
                qCritical() << "Failed to flush data";
            }
        });

        timer->start(second * 1000); // 每隔second秒触发一次

        QObject::connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, [&]() {
            timer->stop(); // 程序退出时停止定时器
            serialPort.close(); // 关闭串口
            delete timer; // 释放定时器资源
        });

        // 注意：以下行仅为了防止QCoreApplication::exec()立即返回；它不会影响定时器的执行。
        QCoreApplication::exec();
    }
};



#endif // SERIAL_PORT_H
