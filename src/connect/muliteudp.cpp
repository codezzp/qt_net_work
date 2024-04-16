#include "muliteudp.h"

MuliteUdp::MuliteUdp()
{

}

MuliteUdp::~MuliteUdp()
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
void MuliteUdp::create_Mulitudp(int port,const QString &addr)
{
    groupAddress=QHostAddress(addr);
    sender=new QUdpSocket();
    receiver = new QUdpSocket();
    receiver->setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    receiver->bind(QHostAddress::AnyIPv4,port);
    sender->bind(groupAddress);
    receiver->joinMulticastGroup(groupAddress);
}

void MuliteUdp::send(const QByteArray &message,int port)
{
    sender->writeDatagram(message,groupAddress,port);
}

QByteArray MuliteUdp::recv()
{
    QByteArray datagram;
    while(receiver->hasPendingDatagrams())
    {
        datagram.resize(receiver->pendingDatagramSize());
        QHostAddress peerAddr;
        quint16 peerPort;
        receiver->readDatagram(datagram.data(),datagram.size(),&peerAddr,&peerPort);
    }
    return datagram;
}

void MuliteUdp::exit_Mulitudp()
{
    receiver->leaveMulticastGroup(groupAddress);
    receiver->abort();
    receiver->close();
    sender->close();
}

QUdpSocket* MuliteUdp::getMultiSocket()
{
    return sender;
}
