#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QList>
#include "mythread.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);
    void startServer();
    QList<MyThread*> listClients;
signals:
    void sendAll(QByteArray Data);

public slots:
    void processing(QByteArray data);

protected:
    void incomingConnection(qintptr socketDescriptor);

};

#endif // MYSERVER_H