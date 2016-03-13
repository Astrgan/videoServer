#include "myserver.h"
#include <QTextCodec>

MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{
    QFile file("D:\\videPaths.txt");
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
    QFile file("D:\\videPaths.txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        QTextStream writeStream(&file);
        writeStream << listVideoPath.last() <<"\n";

        file.close();
    }
}


void MyServer::processing(QByteArray request){


    /*************ВЕРСИЯ-2*************/

    /*************ВЕРСИЯ-1*************/

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
            streamAnswer << videoPath << QString("\n");
        }
       break;
    case 2:
        listVideoPath.append(QString (requestVec.mid(1,-1).begin(), requestVec.size()-1));
        saveString();
        qDebug() << "listVideoPath.last()" << listVideoPath.last();
        streamAnswer << listVideoPath.last() << QString("\n");
        break;

    }

    /*ТЕСТИРОВАНИЕ HTTP ЗАПРОСА*/
/*
    QByteArray answer;
    QDataStream streamAnswer(&answer, QIODevice::WriteOnly);
    streamAnswer << "HTTP/1.0 200 Ok\n"
          "Content-Type: text/html; charset=\"utf-8\"\n"
          "\n"
          "<h1>Nothing to see here</h1>\n"
          << QDateTime::currentDateTime().toString() << "\n";
    qDebug()<<request;

*/
    /*КОНЕЦ ТЕСТИРОВАНИЕ HTTP ЗАПРОСА*/

    /*ТЕСТИРОВАНИЕ HTTP ЗАПРОСА222*/
/*
    QByteArray header;
    QDataStream streamAnswer(&header, QIODevice::WriteOnly);
    streamAnswer << "HTTP/1.0 200 Ok\n"
          "Content-Type: application/JNLPtest2.jar\n"
          "\n";

    emit sendAll(header);


    QByteArray answer;
    QFile file("D:\\Users\\Astrgan\\Documents\\Development\\WorkspaceForMyJob\\JNLPtest2\\dist\\JNLPtest2.jar");
    if(file.open(QIODevice::ReadOnly)){
        answer=file.readAll();
    }
    file.close();
        qDebug()<<request;
*/

        /*КОНЕЦ ТЕСТИРОВАНИЕ HTTP ЗАПРОСА222*/
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
