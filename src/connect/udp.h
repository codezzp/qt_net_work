#ifndef UDP_H
#define UDP_H

#include <QtNetwork>
#include <qtextcodec.h>

class Udp
{
public:
    Udp();
    ~Udp();
    void create_udp(const QString &addr,int port);
    void send(const QByteArray &message,int port,const QString &addr);
    QByteArray receive();
    void exit_udp();
    QUdpSocket* getUdpSocket();
private:
    QUdpSocket *sender=NULL;
    QUdpSocket *receiver=NULL;
};

#endif // UDP_H
