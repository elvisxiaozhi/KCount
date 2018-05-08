#include "connection.h"
#include <QDebug>
#include <QHostAddress>
#include "messagebox.h"

Connection::Connection()
{
    tcpSocket = new QTcpSocket(this);
}

void Connection::connectToServer()
{
    MessageBox updatesMsBox;
    tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 6666);
    if(tcpSocket->waitForConnected()) {
        qDebug() << "Connected to server";
        connect(tcpSocket, &QTcpSocket::disconnected, this, &Connection::disconnected); //connect the signal and slot first and then disconnect from host
        tcpSocket->disconnectFromHost();
    }
    else {
        qDebug() << "Failed to connect to server";
        updatesMsBox.setWindowTitle(tr("Update Failed"));
        updatesMsBox.setText(tr("Oops, there is something wrong.<br>Maybe try again later?"));
    }
    updatesMsBox.exec();
}

void Connection::disconnected()
{
    qDebug() << "Disconned from server";
}

