#ifndef MULITEUDP_H
#define MULITEUDP_H

#include <QtNetwork>

class MuliteUdp
{
public:
    MuliteUdp();
    ~MuliteUdp();
    void create_Mulitudp(int port,const QString &addr);
    void send(const QByteArray &message,int port);
    QByteArray recv();
    void exit_Mulitudp();
    QUdpSocket* getMultiSocket();
private:
    QUdpSocket *receiver=NULL;
    QUdpSocket *sender=NULL;
    QHostAddress groupAddress;
};

#endif // MULITEUDP_H
