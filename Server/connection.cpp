#include "connection.h"
#include <QDebug>

Connection::Connection()
{
    setServer();
}

void Connection::setServer()
{
    tcpServer = new QTcpServer;
    tcpServer->listen(QHostAddress::Any, 6666);

    connect(tcpServer, &QTcpServer::newConnection, this, &Connection::newConnection);
}

void Connection::newConnection()
{
    qDebug() << "connected";
}
