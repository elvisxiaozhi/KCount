#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(int ID, QObject *parent = nullptr);
    void run();

private:
    QTcpSocket *tcpSocket;
    int socketDescriptor;

signals:
    void error(QTcpSocket::SocketError);

public slots:
    void readyRead();
    void disconnected();
};

#endif // THREAD_H
