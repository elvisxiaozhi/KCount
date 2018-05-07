#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QTextStream>
#include <windows.h>

Database::Database()
{
    DBConnection();
}

void Database::DBConnection()
{
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    qDebug() << "Type your database password: ";
    QTextStream textStream(stdin);
    password = textStream.readLine();

    database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("localhost");
    database.setPort(3306);
    database.setDatabaseName("KCountDB");
    database.setUserName("root");
    database.setPassword(password);
    if(database.open()) {
        qDebug() << "Database opened";
        database.close();
        qDebug() << "Database closed";
    }
    else {
        qDebug() << database.lastError().text();
    }
}
