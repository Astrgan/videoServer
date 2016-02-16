#include "mythread.h"

MyThread::MyThread(qintptr ID)
{
    this->socketDescriptor = ID;
}

void MyThread::run()
{
    // thread starts here


    socket = new QTcpSocket();

    // set the ID
    if(!socket->setSocketDescriptor(this->socketDescriptor)){

        //something's wrong, we just emit a signal
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    exec();
}

//void MyThread::readyRead()
//{
//    // get the information
//    QByteArray Data = socket->readAll();

//    // will write on server side window
//    qDebug() << socketDescriptor << " Data in: " << Data;

//    socket->write(Data);
//}

//void MyThread::readyRead()
//{
//    emit answerFromClient(socket->readAll());
//}

void MyThread::readyRead()
{
    QByteArray Data = socket->readAll();
    QVector<QChar> qVec;
    QDataStream streamOUT(Data);
    streamOUT >> qVec;

    for(int i = 0; i<10; i++){
        qDebug()<< "vector: " << qVec[i];
    }

    QFile fileOut("D:\\fileout.txt");
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<< "save";
        QTextStream writeStream(&fileOut);
        writeStream << QString(qVec.data());
        fileOut.close();
    }else{
        qDebug()<< "Don't save";
    }
    emit answerFromClient(Data);

}


void MyThread::disconnected()
{

    socket->deleteLater();
    exit(0);
}

void MyThread::send(QByteArray Data)
{
    socket->write(Data);
    socket->flush();
}
