#ifndef BROADCASTUDP_H
#define BROADCASTUDP_H

#include <QtNetwork>

class broadcastudp
{
public:
    broadcastudp();
    ~broadcastudp();
    void create_broadudp(const QString &addr,int port1);
    void send(const QByteArray &message,int port,const QString &addr);
    QByteArray receive();
    void exit_broadudp();
    QUdpSocket* getBroadUdpSocket();
private:
    QUdpSocket *sender=NULL;
    QUdpSocket *receiver=NULL;
};

#endif // BROADCASTUDP_H
