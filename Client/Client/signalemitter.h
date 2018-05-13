#ifndef SIGNALEMITTER_H
#define SIGNALEMITTER_H

#include <QObject>
#include "singleton.h"

class SignalEmitter : public QObject
{
    Q_OBJECT
public:
    explicit SignalEmitter(QObject *parent = nullptr);

signals:
    void keyPressed(QString);

public slots:
};

typedef Singleton<SignalEmitter> Emitter;

#endif // SIGNALEMITTER_H
