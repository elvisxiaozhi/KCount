#include "server.h"
#include "thread.h"
#include <QTcpSocket>

Server::Server(QObject *parent) :
    QTcpServer(parent)
{

}

void Server::startServer()
{
    if(!this->listen(QHostAddress::LocalHost, 6666)) {
        qDebug() << "Could not start server";
    }
    else {
        qDebug() << "Listening...";
    }

    connect(this, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::newConnection()
{
    qDebug() << "New client connected to server";
    while(this->hasPendingConnections()) {
        QTcpSocket *connectedSocket = this->nextPendingConnection();
        connectedClients.push_back(connectedSocket);
        connect(connectedSocket, &QTcpSocket::disconnected, this, &Server::disconnected);
    }
}

void Server::disconnected()
{
    QTcpSocket *socket = static_cast<QTcpSocket *>(sender());
    socket->deleteLater();
    connectedClients.pop_back();
    qDebug() << "Client disconnected from server";
}

void Server::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    Thread *thread = new Thread(socketDescriptor, this);
    connect(thread, &Thread::finished, thread, &QObject::deleteLater);
    thread->start();
}
