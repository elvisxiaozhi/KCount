#ifndef CONNECTION_H
#define CONNECTION_H

#include <QTcpServer>
#include <QObject>

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection();

private:
    QTcpServer *tcpServer;
    void setServer();

private slots:
    void newConnection();
};

#endif // CONNECTION_H
