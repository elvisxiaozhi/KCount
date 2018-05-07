#include "connection.h"
#include <QDebug>
#include <QHostAddress>

Connection::Connection()
{
    tcpSocket = new QTcpSocket(this);
}

void Connection::connectToServer()
{
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 6666);
    if(tcpSocket->waitForConnected()) {
        qDebug() << "Connected to server";
        connect(tcpSocket, &QTcpSocket::disconnected, this, &Connection::disconnected); //connect the signal and slot first and then disconnect from host
        tcpSocket->disconnectFromHost();
    }
    else {
        qDebug() << "Failed to connect to server";
    }
}

void Connection::disconnected()
{
//    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
//    socket->deleteLater();
    qDebug() << "Disconned from server";
}

