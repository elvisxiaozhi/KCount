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
}

unsigned long int Database::returnTotalPressedTimes()
{
    unsigned long int totalPressedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        query = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1#").arg(currentDate);
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

QVector<std::pair<QString, unsigned long int>> Database::returnKeyVec()
{
    QVector<std::pair<QString, unsigned long int>> vec;

    if(database.open()) {

        QSqlQuery sqlQuery;
        QString query;

        query = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# GROUP BY PressedKey").arg(currentDate);

        sqlQuery.exec(query);
        while(sqlQuery.next()) {
            vec.push_back(std::make_pair(sqlQuery.value(0).toString(), sqlQuery.value(1).toInt()));
        }

        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }

    std::sort(vec.begin(), vec.end(),
              [](const std::pair<QString, unsigned long int> &a, const std::pair<QString, unsigned long int> &b){ return a.second > b.second; });

    return vec;
}

void Database::updatePressedKeyToDB(const QMap<QString, unsigned long> &map)
{
    if(database.open()) {
        for(auto it : map.toStdMap()) { //iterate the an hour temp map
            //try to find if database has already stored pressed key at current hour
            QSqlQuery sqlQuery;
            QString query = QString("SELECT PressedTimes FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2 AND PressedKey = '%3'").arg(currentDate).arg(currentHour).arg(it.first);
            sqlQuery.exec(query);
            //if database hasn't stored current hour's pressed keys
            if(!isQueryFound(sqlQuery)) {
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO Data (CreatedDate, CreatedHour, PressedKey, PressedTimes)"
                                    "VALUES(:CreatedDate, :CreatedHour, :PressedKey, :PressedTimes);");
                insertQuery.bindValue(":CreatedDate", currentDate);
                insertQuery.bindValue(":CreatedHour", currentHour);
                insertQuery.bindValue(":PressedKey", it.first);
                int pressedTimes = it.second; //can not bind this value directly to the next line, do not know why
                insertQuery.bindValue(":PressedTimes", pressedTimes);
                insertQuery.exec();
            }
            else { //if database has already stored current hour's pressed keys
                QSqlQuery updateSqlQuery; //then update stored value
                //note the it.second + sqlQuery.value(0).toInt()
                //means new pressed times + the database has already stored pressed times = this hour totally pressed times
                QString updateQuery = QString("UPDATE Data SET PressedTimes = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3 AND PressedKey = '%4'").arg(QString::number(it.second + sqlQuery.value(0).toInt())).arg(currentDate).arg(currentHour).arg(it.first);
                updateSqlQuery.exec(updateQuery); //then update
            }
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
