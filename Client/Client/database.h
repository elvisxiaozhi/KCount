#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

class Database : public QObject
{
    Q_OBJECT
public:
    Database();
    static int returnTotalPressedTimes();
    static void updateDatabase(QString);

private:
    static QSqlDatabase database;

    static bool isQueryFound(QSqlQuery);
};

#endif // DATABASE_H
