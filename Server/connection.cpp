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
    qDebug() << "New client connected to server";
    while(tcpServer->hasPendingConnections()) {
        QTcpSocket *connectedSocket = tcpServer->nextPendingConnection();
        connectedClients.push_back(connectedSocket);
        connect(connectedSocket, &QTcpSocket::disconnected, this, &Connection::clientDisconnected);
    }
}

void Connection::clientDisconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    socket->deleteLater();
    qDebug() << "Client disconnected from server";
}
