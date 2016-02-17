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
    StartMedia();
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
    qDebug()<<"qVec.last().digitValue(): "<<qVec.last().digitValue();

//    for(int i=0; i<qVec.size(); i++ ){
//        qDebug()<< qVec[i].digitValue();
//    }

    switch(qVec.last().digitValue()) {
    case 1:

       break;
    case 2:
        emit sendAll(data);
       break;
    }
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

void MyServer::StartMedia()
{
    QStringList args;
    args << ":sout=#rtp{dst=127.0.0.1,port=1234,sdp=rtsp://127.0.0.1/ch1}";


//    QString file = "D:\\RaM.mkv";                     //192.168.1.207
//    instance = new VlcInstance(args);
//    media = new VlcMedia(file, true, instance);
//    media->setOption(":sout=#transcode{vcodec=h264,vb=0,scale=0,acodec=mpga,ab=128,channels=2,samplerate=44100}:rtp{dst=192.168.1.207,port=554,sdp=rtsp://192.168.1.207/ch1}");
//    player = new VlcMediaPlayer(instance);
//    player->open(media);

    instance = new VlcInstance(args);
    player = new VlcMediaPlayer(instance);
    media = new VlcMedia("D:\\RaM.mkv", true, instance);
    media->setOption(":sout=#rtp{dst=127.0.0.1,port=1234,sdp=rtsp://127.0.0.1/ch1}");

    player->open(media);
    player->play();
}
