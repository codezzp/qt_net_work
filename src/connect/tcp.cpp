#include "tcp.h"

Tcp::Tcp()
{

}

Tcp::~Tcp(){
    if(sender!=NULL){
        delete sender;
        sender=NULL;
    }
    if(receive!=NULL){
        delete receive;
        receive=NULL;
    }
    if(server!=NULL){
        delete server;
        server=NULL;
    }
}

void Tcp::create_tcp(const QString &addr,int port)
{
    server=new QTcpServer();
    server->listen(QHostAddress(addr), port);
    receive=new QTcpSocket();
    sender=new QTcpSocket();
    con=true;
}

void Tcp::send(const QByteArray &message,int port,const QString &addr)
{
   sender->abort();
   sender->connectToHost(addr, port);
//    if(con==true)
//    {
//        sender->connectToHost(addr, port);
//        con=false;
//    }
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    sender->write(message);
}

QByteArray Tcp::recv()
{
    QByteArray buff;
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030"));
    while (server->hasPendingConnections())
    {
        receive = server->nextPendingConnection();
        buff = receive->readAll();
    }
    return buff;
}

void Tcp::exit_tcp()
{
    server->close();
    sender->close();
    receive->close();
//    free(sender);
//    free(receive);
//    free(server);
}

QTcpSocket* Tcp::getTcpSOcket()
{
    return sender;
}
