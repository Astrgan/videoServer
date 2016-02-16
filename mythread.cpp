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

void MyThread::readyRead()
{
    QByteArray data = socket->readAll();
    emit answerFromClient(data);

}


void MyThread::disconnected()
{

    socket->deleteLater();
    exit(0);
}

void MyThread::send(QByteArray data)
{
    socket->write(data);
    socket->flush();
}
