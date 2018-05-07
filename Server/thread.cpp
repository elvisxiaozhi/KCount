#include "thread.h"
#include <QByteArray>

Thread::Thread(int ID, QObject *parent) :
    QThread(parent)
{
    socketDescriptor = ID;
}

void Thread::run()
{
    qDebug() << "Starting thread";
    tcpSocket = new QTcpSocket();
    if(!tcpSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket->error());
    }

    connect(tcpSocket, &QTcpSocket::readyRead, this, &Thread::readyRead, Qt::DirectConnection);
    connect(tcpSocket, &QTcpSocket::disconnected, this, &Thread::disconnected, Qt::DirectConnection);

    qDebug() << socketDescriptor << "Client Connected";

    exec();
}

void Thread::readyRead()
{
    QByteArray Data = tcpSocket->readAll();
    qDebug() << socketDescriptor << " Data in: " << Data;
    tcpSocket->write(Data);
}

void Thread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";
    tcpSocket->deleteLater();
    exit(0);
}
