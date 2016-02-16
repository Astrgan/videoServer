#include "myserver.h"
#include <QDebug>
#include <QDataStream>

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{
}

void MyServer::startServer()
{
    int port = 1234;

    if(!this->listen(QHostAddress::Any, port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}

void MyServer::processing(QByteArray data)
{
    emit sendAll(data);
}

// This function is called by QTcpServer when a new connection is available.
void MyServer::incomingConnection(qintptr socketDescriptor)
{
    // We have a new connection
    qDebug() << socketDescriptor << " Connecting...";

    listClients.append(new MyThread(socketDescriptor));
    listClients.last()->moveToThread(listClients.last());

    // connect signal/slot
    // once a thread is not needed, it will be beleted later
    connect(listClients.last(), SIGNAL(finished()), listClients.last(), SLOT(deleteLater()));
    connect(listClients.last(), SIGNAL(answerFromClient(QByteArray)), this, SLOT(processing(QByteArray)));
    connect(this, SIGNAL(sendAll(QByteArray)), listClients.last(), SLOT(send(QByteArray)));

    listClients.last()->start();
}
