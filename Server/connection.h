#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection();

private:
    QTcpServer *tcpServer;
    QVector<QTcpSocket *> connectedClients;
    void setServer();

private slots:
    void newConnection();
    void clientDisconnected();
};

#endif // CONNECTION_H
