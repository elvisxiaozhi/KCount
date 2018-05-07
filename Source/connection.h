#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>

class Connection : QObject
{
    Q_OBJECT
public:
    Connection();
    void connectToServer();

private:
    QTcpSocket *tcpSocket;
};

#endif // CONNECTION_H
