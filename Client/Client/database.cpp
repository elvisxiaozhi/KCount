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

unsigned long int Database::returnTotalPressedTimes(int readMode)
{
    unsigned long int totalPressedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        switch (readMode) {
        case 0:
            query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedHour = %1").arg(currentHour);
            break;
        case 1:
            query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate = #%1#").arg(currentDate);
            break;
        case 2:
            query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate BETWEEN #%1# AND #%2#").arg(currentDate).arg(QDate::currentDate().addDays(-7).toString("MM/dd/yy"));
            break;
        case 3:
            query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate BETWEEN #%1# AND #%2#").arg(currentDate).arg(QDate::currentDate().addMonths(-1).toString("MM/dd/yy"));
            break;
        case 4:
            query = QString("SELECT PressedKey, SUM(PressedTimes) FROM KeyPress WHERE CreatedDate BETWEEN #%1# AND #%2#").arg(currentDate).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
            break;
        default:
            break;
        }

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

QVector<std::pair<QString, unsigned long int>> Database::returnKeyVec(int readMode)
{
    QVector<std::pair<QString, unsigned long int>> vec;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        switch (readMode) {
        case 0: //hour
            query = QString("SELECT PressedKey, PressedTimes FROM KeyPress WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);
            break;
        case 1: //day
            query = QString("SELECT PressedKey, SUM(PressedTimes) FROM KeyPress WHERE CreatedDate = #%1# GROUP BY PressedKey").arg(currentDate);
            break;
        case 2: //week
            query = QString("SELECT PressedKey, SUM(PressedTimes) FROM KeyPress WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addDays(-7).toString("MM/dd/yy"));
            break;
        case 3: //month
            query = QString("SELECT PressedKey, SUM(PressedTimes) FROM KeyPress WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addMonths(-1).toString("MM/dd/yy"));
            break;
        case 4: //year
            query = QString("SELECT PressedKey, SUM(PressedTimes) FROM KeyPress WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
            break;
        default:
            break;
        }

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

void Database::updatePressedKeyToDB(const QMap<QString, unsigned long int> &map)
{
    if(database.open()) {
        for(auto it : map.toStdMap()) { //iterate the an hour temp map
            //try to find if database has already stored pressed key at current hour
            QSqlQuery sqlQuery;
            QString query = QString("SELECT PressedTimes FROM KeyPress WHERE CreatedDate = #%1# AND CreatedHour = %2 AND PressedKey = '%3'").arg(currentDate).arg(currentHour).arg(it.first);
            sqlQuery.exec(query);
            //if database hasn't stored current hour's pressed keys
            if(!isQueryFound(sqlQuery)) {
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO KeyPress (CreatedDate, CreatedHour, PressedKey, PressedTimes)"
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
                QString updateQuery = QString("UPDATE KeyPress SET PressedTimes = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3 AND PressedKey = '%4'").arg(QString::number(it.second + sqlQuery.value(0).toInt())).arg(currentDate).arg(currentHour).arg(it.first);
                updateSqlQuery.exec(updateQuery); //then update
            }
        }

        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }
}

unsigned long int Database::returnLeftClickTimes(int readMode)
{
    unsigned long int clickedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        switch (readMode) {
        case 0: //hour
            query = QString("SELECT SUM(LeftClickTimes) FROM MouseClick WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);
            break;
        case 1: //day
            query = QString("SELECT SUM(LeftClickTimes) FROM MouseClick WHERE CreatedDate = #%1# GROUP BY PressedKey").arg(currentDate);
            break;
        case 2: //week
            query = QString("SELECT SUM(LeftClickTimes) FROM MouseClick WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addDays(-7).toString("MM/dd/yy"));
            break;
        case 3: //month
            query = QString("SELECT SUM(LeftClickTimes) FROM MouseClick WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addMonths(-1).toString("MM/dd/yy"));
            break;
        case 4: //year
            query = QString("SELECT SUM(LeftClickTimes) FROM MouseClick WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
            break;
        default:
            break;
        }

        sqlQuery.exec(query);
        while(sqlQuery.next()) {
            clickedTimes = sqlQuery.value(0).toInt();
        }

        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }

    return clickedTimes;
}

unsigned long int Database::returnRightClickTimes(int readMode)
{
    unsigned long int clickedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        switch (readMode) {
        case 0: //hour
            query = QString("SELECT SUM(RightClickTimes) FROM MouseClick WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);
            break;
        case 1: //day
            query = QString("SELECT SUM(RightClickTimes) FROM MouseClick WHERE CreatedDate = #%1# GROUP BY PressedKey").arg(currentDate);
            break;
        case 2: //week
            query = QString("SELECT SUM(RightClickTimes) FROM MouseClick WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addDays(-7).toString("MM/dd/yy"));
            break;
        case 3: //month
            query = QString("SELECT SUM(RightClickTimes) FROM MouseClick WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addMonths(-1).toString("MM/dd/yy"));
            break;
        case 4: //year
            query = QString("SELECT SUM(RightClickTimes) FROM MouseClick WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
            break;
        default:
            break;
        }

        sqlQuery.exec(query);
        while(sqlQuery.next()) {
            clickedTimes = sqlQuery.value(0).toInt();
        }

        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }

    return clickedTimes;
}

void Database::updateLeftClickToDB(const unsigned long int &clickedTimes)
{
    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query = QString("SELECT LeftClickTimes FROM MouseClick WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);

        sqlQuery.exec(query);
        if(!isQueryFound(sqlQuery)) {
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO MouseClick (CreatedDate, CreatedHour, LeftClick, RightClick)"
                                "VALUES(:CreatedDate, :CreatedHour, :LeftClick, :RightClick);");
            insertQuery.bindValue(":CreatedDate", currentDate);
            insertQuery.bindValue(":CreatedHour", currentHour);
            int times = clickedTimes;
            insertQuery.bindValue(":LeftClick", times);
            insertQuery.bindValue(":RightClick", 0);
            insertQuery.exec();
        }
        else {
            QSqlQuery updateSqlQuery;
            QString updateQuery = QString("UPDATE MouseClick SET LeftClickTimes = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3").arg(QString::number(clickedTimes + sqlQuery.value(0).toInt())).arg(currentDate).arg(currentHour);
            updateSqlQuery.exec(updateQuery); //then update
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
