#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QList>
#include <QDebug>
#include <QDataStream>
#include <QByteArray>

#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>

#include "mythread.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void startServer();
    void StartMedia();
    QList<MyThread*> listClients;
    QList<QString> listString;

    VlcInstance *instance;
    VlcMediaPlayer *player;
    VlcMedia *media;
signals:
    void sendAll(QByteArray Data);

public slots:
    void processing(QByteArray data);

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // MYSERVER_H
