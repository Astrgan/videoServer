#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QDataStream>

#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QString>
#include <qDebug>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(qintptr ID);

    void run();

signals:
    void answerFromClient(QByteArray Data);
    void error(QTcpSocket::SocketError socketerror);

public slots:
    void readyRead();
    void disconnected();
    void send(QByteArray Data);

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
};

#endif // MYTHREAD_H
