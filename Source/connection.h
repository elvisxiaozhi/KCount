#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QTcpSocket>

class Connection : public QObject
{
    Q_OBJECT
public:
    Connection();
    void connectToServer();

private:
    QTcpSocket *tcpSocket;

private slots:
    void disconnected();
};

#endif // CONNECTION_H
