#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QTimer>

class Database : public QObject
{
    Q_OBJECT
public:
    Database();
    static unsigned long int returnTotalPressedTimes();
    static void updateTotalPressedTimes(QString);

private:
    static QSqlDatabase database;
    static QString currentDate;
    static int currentHour;
    QTimer *timer;

    static bool isQueryFound(QSqlQuery);
    void setTimer();

signals:
    void updateDatabase();

private slots:
    void updateTimer();
};

#endif // DATABASE_H
