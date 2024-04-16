#ifndef TCP_H
#define TCP_H

#include <QtNetwork>
#include <QtCore>
#include <QTextCodec>
#include<QCoreApplication>
#include <QByteArray>

class Tcp:QObject
{
public:
    Tcp();
    ~Tcp();
    void create_tcp(const QString &addr,int port);
    void send(const QByteArray &message,int port,const QString &addr);
    QByteArray recv();
    void exit_tcp();
    bool con;
    QTcpSocket* getTcpSOcket();
private:
    QTcpSocket *receive=NULL;
    QTcpServer *server=NULL;
    QTcpSocket *sender=NULL;
};

#endif // TCP_H
