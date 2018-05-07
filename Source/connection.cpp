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
    }
    else {
        qDebug() << "Failed to connect to server";
    }
}

