#include "myserver.h"

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{
    QFile file("D:\\videPath.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream readStream(&file);
        while (!readStream.atEnd()) {
            listVideoPath.append(readStream.readLine());
        }

        file.close();
    }

//    qDebug()<<listVideoPath;
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

void MyServer::saveString()
{
    QFile file("D:\\videPath.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        QTextStream writeStream(&file);
        writeStream << "\r\n" << listVideoPath.last();

        file.close();
    }
}


void MyServer::processing(QByteArray request){

    QVector<QChar> requestVec;
    QDataStream streamOUT(request);
    streamOUT >> requestVec;
    qDebug()<<"requestVec.begin().digitValue(): "<<requestVec.begin()->digitValue();

    QByteArray answer;
    QDataStream streamAnswer(&answer, QIODevice::WriteOnly);
    qDebug() << requestVec;

    switch(requestVec.begin()->digitValue()){
    case 1:
        qDebug()<<"case1";
        foreach(QString videoPath, listVideoPath){
            streamAnswer << videoPath << "\r\n";
        }
       break;
    case 2:
        listVideoPath.append(QString(requestVec.mid(1,-1).begin(), requestVec.size()-1));
        saveString();
        qDebug() << "listVideoPath.last()" << listVideoPath.last();
        streamAnswer << listVideoPath.last() << "\r\n";
        break;
    }
    emit sendAll(answer);
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
