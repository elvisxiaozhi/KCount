#include "database.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>
#include <QDir>
#include <QStandardPaths>
#include "initialisation.h"
#include "messagebox.h"

QSqlDatabase Database::database;
QString Database::currentDate = QDate::currentDate().toString("MM/dd/yy");
int Database::currentHour = QTime::currentTime().toString("h").toInt();

Database::Database(QObject *parent) : QObject(parent)
{
    createDataFile();

    //set database
    QString dbName = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(databaseLoc);
    database = QSqlDatabase::addDatabase("QODBC");
    database.setDatabaseName(dbName);

    if(!database.open()) {
        showErrorMsBox();
    }
}

unsigned long int Database::returnTotalPressedTimes(int readMode)
{
    unsigned long int totalPressedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        switch (readMode) {
        case 0:
            query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);
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
            query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate BETWEEN #%1# AND #%2#").arg(currentDate).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
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
            query = QString("SELECT PressedKey, SUM(PressedTimes) FROM KeyPress WHERE CreatedDate = #%1# AND CreatedHour = %2 GROUP BY PressedKey").arg(currentDate).arg(currentHour);
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

unsigned long int Database::returnClickedTimes(QString clickType, int readMode)
{
    unsigned long int clickedTimes = 0;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        switch (readMode) {
        case 0: //hour
            query = QString("SELECT SUM(%1) FROM MouseClick WHERE CreatedDate = #%2# AND CreatedHour = %3").arg(clickType).arg(currentDate).arg(currentHour);
            break;
        case 1: //day
            query = QString("SELECT SUM(%1) FROM MouseClick WHERE CreatedDate = #%2#").arg(clickType).arg(currentDate);
            break;
        case 2: //week
            query = QString("SELECT SUM(%1) FROM MouseClick WHERE CreatedDate BETWEEN #%2# AND #%3#").arg(clickType).arg(currentDate).arg(QDate::currentDate().addDays(-7).toString("MM/dd/yy"));
            break;
        case 3: //month
            query = QString("SELECT SUM(%1) FROM MouseClick WHERE CreatedDate BETWEEN #%2# AND #%3#").arg(clickType).arg(currentDate).arg(QDate::currentDate().addMonths(-1).toString("MM/dd/yy"));
            break;
        case 4: //year
            query = QString("SELECT SUM(%1) FROM MouseClick WHERE CreatedDate BETWEEN #%2# AND #%3#").arg(clickType).arg(currentDate).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
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

void Database::updateLeftClickToDB(const int &leftClickedTimes, const int &rightClickedTimes)
{
    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query = QString("SELECT LeftClick, RightClick FROM MouseClick WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);

        sqlQuery.exec(query);
        if(!isQueryFound(sqlQuery)) {
            QSqlQuery insertQuery;
            insertQuery.prepare("INSERT INTO MouseClick (CreatedDate, CreatedHour, LeftClick, RightClick)"
                                "VALUES(:CreatedDate, :CreatedHour, :LeftClick, :RightClick);");
            insertQuery.bindValue(":CreatedDate", currentDate);
            insertQuery.bindValue(":CreatedHour", currentHour);
            insertQuery.bindValue(":LeftClick", leftClickedTimes);
            insertQuery.bindValue(":RightClick", rightClickedTimes);
            insertQuery.exec();
        }
        else {
            QSqlQuery updateSqlQuery;
            QString updateQuery;
            updateQuery = QString("UPDATE MouseClick SET LeftClick = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3").arg(QString::number(leftClickedTimes + sqlQuery.value(0).toInt())).arg(currentDate).arg(currentHour);
            updateSqlQuery.exec(updateQuery);
            updateQuery = QString("UPDATE MouseClick SET RightClick = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3").arg(QString::number(rightClickedTimes + sqlQuery.value(1).toInt())).arg(currentDate).arg(currentHour);
            updateSqlQuery.exec(updateQuery);
        }

        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }
}

QMap<int, unsigned long> Database::returnBarChartData(int readMode)
{
    QMap<int, unsigned long int> map;

    if(database.open()) {
        QSqlQuery sqlQuery;
        QString query;

        switch (readMode) {
        case 1: //daily
            for(int i = 0; i < 24; ++i) {
                query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(i);
                sqlQuery.exec(query);
                while(sqlQuery.next()) {
                    map.insert(i, sqlQuery.value(0).toInt());
                }
            }
            break;
        case 2: //weekly
            for(int i = 6; i >= 0; --i) {
                query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate = #%1#").arg(QDate::currentDate().addDays(-i).toString("MM/dd/yy"));
                sqlQuery.exec(query);
                while(sqlQuery.next()) {
                    map.insert(6 - i, sqlQuery.value(0).toInt());
                }
            }
            break;
        case 3: { //monthly
            int daysInMonth = QDate::currentDate().daysInMonth();
            for(int i = 0; i < daysInMonth; ++i) {
                query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE CreatedDate = #%1#").arg(QDate::currentDate().addDays(i - daysInMonth + 1).toString("MM/dd/yy"));
                sqlQuery.exec(query);
                while(sqlQuery.next()) {
                    map.insert(i, sqlQuery.value(0).toInt());
                }
            }
        }
            break;
        case 4:
            for(int i = 0; i < 12; ++i) {
                query = QString("SELECT SUM(PressedTimes) FROM KeyPress WHERE WHERE CreatedDate BETWEEN #%1# AND #%2#").arg(QDate::currentDate().addMonths(-i).toString("MM/dd/yy")).arg(QDate::currentDate().addMonths(-i - 1).toString("MM/dd/yy"));
                sqlQuery.exec(query);
                while(sqlQuery.next()) {
                    map.insert(i, sqlQuery.value(0).toInt());
                }
            }
            break;
        default:
            break;
        }

        database.close();
    }
    else {
        qDebug() << database.lastError().text();
    }

    return map;
}

bool Database::isQueryFound(QSqlQuery query)
{
    while(query.next()) {
        return true;
    }
    return false;
}

void Database::createDataFile()
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    dataPath = homePath.first() + "/AppData/Local/Nana";
    if(!QDir().exists(dataPath)) {
        QDir().mkdir(dataPath);
    }

    databaseLoc = dataPath + "/UserData.mdb";
    if(!QFile().exists(databaseLoc)) {
        if(Initialisation::settings.value("InitSettings/AppPath").isValid()) {
            QFile::copy(Initialisation::settings.value("InitSettings/AppPath").toString() + "/UserData.mdb", databaseLoc);
        }
        else {
            QFile::copy(QDir::currentPath() + "/UserData.mdb", databaseLoc);
        }
        Initialisation::settings.setValue("InitSettings/AppPath", QDir::currentPath());
    }
}

void Database::showErrorMsBox()
{
    MessageBox msBox;
    connect(&msBox, &MessageBox::fixDatabaseError, [=](){ createDataFile(); });
    msBox.setDetailedText(database.lastError().text());
    msBox.showDatabaseErrorMsBox();
}

void Database::timeout()
{
    currentDate = QDate::currentDate().toString("MM/dd/yy");
    currentHour = QTime::currentTime().toString("h").toInt();
}
