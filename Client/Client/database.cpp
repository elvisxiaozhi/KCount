#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>

QSqlDatabase Database::database;
QString Database::currentDate = QDate::currentDate().toString("MM/dd/yy");
int Database::currentHour = QTime::currentTime().toString("h").toInt();

Database::Database()
{
    //set database
    QString dbName = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= C:/Users/Theodore/Desktop/UserData.mdb");
    database = QSqlDatabase::addDatabase("QODBC");
    database.setDatabaseName(dbName);

    setTimer(); //set the time, so the database will save automatically in each hour
}

unsigned long int Database::returnTotalPressedTimes()
{
    unsigned long int totalPressedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

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

void Database::updateTotalPressedTimes(QString pressedKey)
{
    if(database.open()) {
        //try to find if database has already stored pressed key at current hour
        QSqlQuery sqlQuery;
        QString query = QString("SELECT PressedTimes FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2 AND PressedKey = '%3'").arg(currentDate).arg(currentHour).arg(pressedKey);
        sqlQuery.exec(query);

        if(isQueryFound(sqlQuery)) {
            QSqlQuery updateSqlQuery; //then update stored value
            QString updateQuery = QString("UPDATE Data SET PressedTimes = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3 AND PressedKey = '%4'").arg(QString::number(sqlQuery.value(0).toInt() + 1)).arg(currentDate).arg(currentHour).arg(pressedKey);
            updateSqlQuery.exec(updateQuery);
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

void Database::setTimer()
{
    QStringList currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    timer = new QTimer(this);
    timer->start(1000 * 60 * 60 - 1000 * 60 * QString(currentTimeStringList[1]).toInt() - 1000 * QString(currentTimeStringList[2]).toInt()); //1 sec * 60 (= 1 minute) * 60 (= 1 hour)
    connect(timer, &QTimer::timeout, this, &Database::updateTimer);
}

void Database::updateTimer()
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    emit updateDatabase();
}
