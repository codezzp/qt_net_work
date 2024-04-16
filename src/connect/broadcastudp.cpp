#include "broadcastudp.h"

broadcastudp::broadcastudp()
{

}

broadcastudp::~broadcastudp()
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

QUdpSocket* broadcastudp::getBroadUdpSocket()
{
    return this->sender;
}

void broadcastudp::exit_broadudp()
{
    receiver->close();
    sender->close();
//    free(receiver);
//    free(sender);
}

void broadcastudp::create_broadudp(const QString &addr,int port)
{
    receiver=new QUdpSocket();
    receiver->bind(QHostAddress(addr),port);
    sender=new QUdpSocket();
    sender->bind(QHostAddress(addr));
    receiver->setSocketOption(QAbstractSocket::MulticastLoopbackOption,1);
}

void broadcastudp::send(const QByteArray &message,int port,const QString &addr)
{
    sender->writeDatagram(message.data(),message.size(),QHostAddress(addr),port);
}

QByteArray broadcastudp::receive()
{
    QByteArray datagram;
    while (receiver->hasPendingDatagrams())
    {
        datagram.resize(receiver->pendingDatagramSize());
        QHostAddress senderAddress;
        quint16 senderPort;
        receiver->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);
    }
    return datagram;
}
