#include "udp.h"

Udp::Udp()
{

}

Udp::~Udp()
{
    if(receiver!=NULL){
        delete receiver;
        receiver=NULL;
    }
    if(sender!=NULL){
        delete sender;
        sender=NULL;
    }
}

void Udp::exit_udp()
{
    receiver->close();
    sender->close();
//    free(receiver);
//    free(sender);
}

void Udp::create_udp(const QString &addr,int port)
{
    receiver=new QUdpSocket();
    receiver->bind(QHostAddress(addr),port);
    sender=new QUdpSocket();
}

void Udp::send(const QByteArray &message,int port,const QString &addr)
{
    sender->writeDatagram(message.data(),message.size(),QHostAddress(addr),port);
}

QByteArray Udp::receive()
{
    QByteArray datagram;
    datagram.resize(receiver->pendingDatagramSize());
    receiver->readDatagram(datagram.data(),datagram.size());
    return datagram;
}

QUdpSocket* Udp::getUdpSocket()
{
    return this->sender;
}
