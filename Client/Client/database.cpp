#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>

QSqlDatabase Database::database;

Database::Database()
{
    //set database
    QString dbName = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= C:/Users/Theodore/Desktop/UserData.mdb");
    database = QSqlDatabase::addDatabase("QODBC");
    database.setDatabaseName(dbName);
}

int Database::returnTotalPressedTimes()
{
    int totalPressedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        QString currentDate = QDate::currentDate().toString("MM/dd/yy");
        int currentHour = QTime::currentTime().toString("h").toInt();

        query = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);
        sqlQuery.exec(query);

        while(sqlQuery.next()) {
            totalPressedTimes = sqlQuery.value(0).toInt();
        }

        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }

    return totalPressedTimes;
}

void Database::updateDatabase(QString pressedKey)
{
    QString currentDate = QDate::currentDate().toString("MM/dd/yy");
    int currentHour = QTime::currentTime().toString("h").toInt();

    if(database.open()) {
        //try to find if database has already stored pressed key at current hour
        QSqlQuery sqlQuery;
        QString query = QString("SELECT PressedTimes FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2 AND PressedKey = '%3'").arg(currentDate).arg(currentHour).arg(pressedKey);
        sqlQuery.exec(query);

        if(isQueryFound(sqlQuery)) {
            QSqlQuery updateSqlQuery; //then update stored value
            QString updateQuery = QString("UPDATE Data SET PressedTimes = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3 AND PressedKey = '%4'").arg(QString::number(sqlQuery.value(0).toInt() + 1)).arg(currentDate).arg(currentHour).arg(pressedKey);
            updateSqlQuery.exec(updateQuery); //then update
        }
        else {
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO Data (CreatedDate, CreatedHour, PressedKey, PressedTimes)"
                                "VALUES(:CreatedDate, :CreatedHour, :PressedKey, :PressedTimes);");
            insertQuery.bindValue(":CreatedDate", currentDate);
            insertQuery.bindValue(":CreatedHour", currentHour);
            insertQuery.bindValue(":PressedKey", pressedKey);
            insertQuery.bindValue(":PressedTimes", 1);
            insertQuery.exec();
        }
        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }
}

bool Database::isQueryFound(QSqlQuery query)
{
    while(query.next()) {
        return true;
    }
    return false;
}
