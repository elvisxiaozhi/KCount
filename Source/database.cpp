#include "database.h"
#include <QSqlError>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>
#include <QTime>
#include "signalemitter.h"

QSqlDatabase sqlDatabase;
QString Database::dataPath;
QSettings Database::appPathSetting("My Computer", "KCount");

Database::Database(QObject *parent) : QObject(parent)
{
    makeDataFile(); //make database file

    totalPressedTimes = 0; //initiate the total key pressed times

    //set database
    QString accessString = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(filePath);
    sqlDatabase = QSqlDatabase::addDatabase("QODBC");
    sqlDatabase.setDatabaseName(accessString);

    readDatabase(2); //read data from database, the read mode is read the data within a day

    setTimer(); //set the time, so the database will save automatically in each hour

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &Database::keyPressed); //when a key is pressed, then it will call the kepPressed slot
}

void Database::deleteDataFile(QString deleteDataPath)
{
    static QDir deleteFilePath(deleteDataPath);
    deleteFilePath.removeRecursively();
}

int Database::returnTotalPressedTimes(QString queryStr)
{
    int pressedTimes = 0;
    if(sqlDatabase.open()) {
        QSqlQuery query;
        query.exec(queryStr);
        while(query.next()) {
            pressedTimes = query.value(0).toInt();
        }
        sqlDatabase.close();
    }
    else {
        qDebug() << sqlDatabase.lastError().text();
    }
    return pressedTimes;
}

QMap<QString, int> Database::returnFrequentlyPressedKeyMap(QString queryStr)
{
    QMap<QString, int> frequentlyPressedKeyMap;
    if(sqlDatabase.open()) {
        QSqlQuery query;
        query.exec(queryStr);

        int iterateTimes = 0;
        while(query.next()) {
            frequentlyPressedKeyMap.insert(query.value(0).toString(), query.value(1).toInt());
            iterateTimes++;
            if(iterateTimes == 5) {
                break;
            }
        }

        sqlDatabase.close();
    }
    else {
        qDebug() << sqlDatabase.lastError().text();
    }
    return frequentlyPressedKeyMap;
}

bool Database::isQueryFound(QSqlQuery hasPressedKeyAtCurrentHourQuery) const
{
    while(hasPressedKeyAtCurrentHourQuery.next()) {
        return true;
    }
    return false;
}

void Database::makeDataFile()
{
    const static QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    dataPath = homePath.first() + "/AppData/Local/KCount";
    if(!QDir().exists(dataPath)) {
        QDir().mkdir(dataPath);
    }

    filePath = dataPath + "/UserData.mdb";
    if(!QFile().exists(filePath)) {
        if(appPathSetting.value("DataPath").isValid()) {
            QFile::copy(appPathSetting.value("AppPath").toString() + "/UserData.mdb", filePath);
        }
        else {
            QFile::copy(QDir::currentPath() + "/UserData.mdb", filePath);
        }
        appPathSetting.setValue("AppPath", QDir::currentPath());
    }
}

void Database::setTimer()
{
    currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    timer = new QTimer(this);
    timer->start(1000 * 60 * 60 - 1000 * 60 * QString(currentTimeStringList[1]).toInt() - 1000 * QString(currentTimeStringList[2]).toInt()); //1 sec * 60 (= 1 minute) * 60 (= 1 hour)
    connect(timer, &QTimer::timeout, this, &Database::updateTimer);
}

void Database::sortMap()
{
    mapVector.clear(); //when sort map, the vector needs to clear the old data and reload new data from map to sort
    QMap<QString, unsigned long int>::iterator it;
    for(it = pressedKeyMap.begin(); it != pressedKeyMap.end(); it++) {
        mapVector.push_back(std::make_pair(it.key(), it.value()));
    }
    std::sort(mapVector.begin(), mapVector.end(), [=](std::pair<QString, unsigned long int>& a, std::pair<QString, unsigned long int>& b){
        return a.second > b.second;
    });
}

void Database::insertNewData(QString pressedKey, unsigned long int pressedTimes)
{
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO Data (CreatedDate, CreatedHour, PressedKey, PressedTimes)"
                        "VALUES(:CreatedDate, :CreatedHour, :PressedKey, :PressedTimes);");
    insertQuery.bindValue(":CreatedDate", QDate::currentDate().toString("MM/dd/yy"));
    QString CreatedHour = QTime::currentTime().toString("h");
    insertQuery.bindValue(":CreatedHour", CreatedHour.toInt());
    insertQuery.bindValue(":PressedKey", pressedKey);
    int currentHourPressedTimes = pressedTimes; //can not bind this value directly to the next line, do not know why
    insertQuery.bindValue(":PressedTimes", currentHourPressedTimes);
    insertQuery.exec();
}

void Database::keyPressed(QString pressedKey)
{
    //if the map has already stored the pressed key
    if(pressedKeyMap.contains(pressedKey)) {
        unsigned long int newValue = pressedKeyMap.value(pressedKey) + 1; //then the map key value + 1 pressed time
        pressedKeyMap.insert(pressedKey, newValue); //and insert new data to the map

        unsigned long int newValueAtCurrentHour = currentHourPressedKeyMap.value(pressedKey) + 1;
        currentHourPressedKeyMap.insert(pressedKey, newValueAtCurrentHour);
    }
    //if not
    else {
        pressedKeyMap.insert(pressedKey, 1); //then the insert the pressed key with a 1 time pressed value to the map

        currentHourPressedKeyMap.insert(pressedKey, 1);
    }

    totalPressedTimes++; //and also the total pressed times + 1 pressed time

    sortMap(); //then store the data to the mapVector and sort it in the descending order

    emit keyPressedDone(); //finally emit keyPressedDone() signal, and it will call a slot in MainWindow to update the lbl text
}

void Database::updateDatabase()
{
    QString currentDate = QDate::currentDate().toString("MM/dd/yy");
    int currentHour = QTime::currentTime().toString("h").toInt();

    if(sqlDatabase.open()) {
        for(auto it : currentHourPressedKeyMap.toStdMap()) { //iterate the an hour temp map
            //try to find if database has already stored pressed key at current hour
            QSqlQuery hasPressedKeyAtCurrentHourQuery;
            QString hasPressedKeyAtCurrentHourQueryStr = QString("SELECT PressedTimes FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2 AND PressedKey = '%3'").arg(currentDate).arg(currentHour).arg(it.first);
            hasPressedKeyAtCurrentHourQuery.exec(hasPressedKeyAtCurrentHourQueryStr);
            //if database hasn't stored current hour's pressed keys
            if(!isQueryFound(hasPressedKeyAtCurrentHourQuery)) {
                insertNewData(it.first, it.second); //then insert new data
            }
            else { //if database has already stored current hour's pressed keys
                QSqlQuery updatePessedKeyAtCurrentHourQuery; //then update stored value
                //note the it.second + hasPressedKeyAtCurrentHourQuery.value(0).toInt()
                //means new pressed times + the database has already stored pressed times = this hour totally pressed times
                QString updatePessedKeyAtCurrentHourStr = QString("UPDATE Data SET PressedTimes = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3 AND PressedKey = '%4'").arg(QString::number(it.second + hasPressedKeyAtCurrentHourQuery.value(0).toInt())).arg(currentDate).arg(currentHour).arg(it.first);
                updatePessedKeyAtCurrentHourQuery.exec(updatePessedKeyAtCurrentHourStr); //then update
            }
        }

        sqlDatabase.close();
    }
    else {
        qDebug() << sqlDatabase.lastError().text();
    }
}

void Database::readDatabase(int readMode)
{
    if(sqlDatabase.open()) {
        pressedKeyMap.clear(); //clear the old data and re-read new data

        QSqlQuery readQuery;
        QString readQueryStr;

        QString currentDate = QDate::currentDate().toString("MM/dd/yy");
        int currentHour = QTime::currentTime().toString("h").toInt();

        switch (readMode) {
        case 1: //read database within an hour
            readQueryStr = QString("SELECT PressedKey, PressedTimes FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);
            break;
        case 2: //read database within a day
            readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# GROUP BY PressedKey").arg(currentDate);
            break;
        case 3: //read database within a week
            readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addDays(-7).toString("MM/dd/yy"));
            break;
        case 4: //read database within a month
            readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addMonths(-1).toString("MM/dd/yy"));
            break;
        case 5: //read database within a year
            readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
            break;
        default:
            break;
        }

        readQuery.exec(readQueryStr);
        while(readQuery.next()) {
            pressedKeyMap.insert(readQuery.value(0).toString(), readQuery.value(1).toInt());
        }

        sqlDatabase.close();
    }
    else {
        qDebug() << sqlDatabase.lastError().text();
    }

    sortMap();

    totalPressedTimes = 0; //clear the old pressed times and re-count
    for(int i = 0; i < mapVector.size(); i++) {
        totalPressedTimes += mapVector[i].second;
    }
}

void Database::updateTimer()
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    updateDatabase();

    readDatabase(2); //because in a peroid of time, the data will store to database, and map and vector will be cleared, so when updating database, it needs to re-read data to map and vector
}

void clearDatabase()
{
    if(sqlDatabase.open()) {
        QSqlQuery deleteQuery;
        deleteQuery.exec("DELETE FROM Data");
        sqlDatabase.close();
    }
    else {
        qDebug() << sqlDatabase.lastError().text();
    }
}
