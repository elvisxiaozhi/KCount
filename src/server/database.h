#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class Database
{
public:
    Database();

private:
    QSqlDatabase database;
    QString password;
    void DBConnection();
};

#endif // DATABASE_H
