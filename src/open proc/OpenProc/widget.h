#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QObject>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    void readInitXml();
    void writeBatFile(QString);
    void runBatFile(QString);

    QTimer *timer;
    int passedTime;

private slots:
    void whenTimeout();
};

#endif // WIDGET_H
