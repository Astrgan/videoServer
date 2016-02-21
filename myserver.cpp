#include "myserver.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{
    QFile file("D:\\videPath.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream readStream(&file);
        while (!readStream.atEnd()) {
            listString.append(readStream.readLine());
        }

        file.close();
    }

//    qDebug()<<listString;
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
    QVector<QChar> qVec;
    QDataStream streamOUT(data);
    streamOUT >> qVec;
    qDebug()<<"qVec.begin().digitValue(): "<<qVec.begin()->digitValue();

//    for(int i=0; i<qVec.size(); i++ ){
//        qDebug()<< qVec[i].digitValue();
//    }

    QByteArray answer;
    QDataStream streamAnswer(&answer, QIODevice::WriteOnly);
    streamAnswer << qVec.mid(1,-1) << "\r\n";
    qDebug()<< qVec;

    switch(qVec.begin()->digitValue()) {
    case 1:

       break;
    case 2:
        qDebug()<<"case2";
//        listString.append(QString(qVec.mid(1,-1)));
        emit sendAll(answer);
       break;
    }


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
