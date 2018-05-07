#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);
    void startServer();

private:
    QVector<QTcpSocket *> connectedClients;

private slots:
    void newConnection();
    void disconnected();

protected:
    void incomingConnection(int);
};

#endif // SERVER_H
