#include "database.h"
#include <QSqlError>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>
#include <QTime>
#include "signalemitter.h"

QSqlDatabase dataBase;
QString DataBase::dataPath;
QSettings DataBase::appPathSetting("My Computer", "Keylogger");

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    makeDataFile();

    keyPressedTimes = 0;

    QString accessString = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(filePath);
    dataBase = QSqlDatabase::addDatabase("QODBC");
    dataBase.setDatabaseName(accessString);

    readDatabase(2);

    setTimer();

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &DataBase::keyPressed);
}

void DataBase::deleteDataFile(QString deleteDataPath)
{
    static QDir deleteFilePath(deleteDataPath);
    deleteFilePath.removeRecursively();
}

bool DataBase::isQueryFound(QSqlQuery hasPressedKeyAtCurrentHourQuery) const
{
    while(hasPressedKeyAtCurrentHourQuery.next()) {
        return true;
    }
    return false;
}

void DataBase::makeDataFile()
{
    const static QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    dataPath = homePath.first() + "/AppData/Local/Keylogger";
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

void DataBase::setTimer()
{
    currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    timer = new QTimer(this);
    timer->start(1000 * 60 * 60 - 1000 * 60 * QString(currentTimeStringList[1]).toInt() - 1000 * QString(currentTimeStringList[2]).toInt()); //1 sec * 60 (= 1 minute) * 60 (= 1 hour)
    connect(timer, &QTimer::timeout, this, &DataBase::updateTimer);
}

void DataBase::sortMap()
{
    mapVector.clear(); //when sort map, the vector needs to clear the old data and reload new data from map to sort
    QMap<QString, unsigned long long int>::iterator it;
    for(it = pressedKeyMap.begin(); it != pressedKeyMap.end(); it++) {
        mapVector.push_back(std::make_pair(it.key(), it.value()));
    }
    std::sort(mapVector.begin(), mapVector.end(), [=](std::pair<QString, unsigned long long int>& a, std::pair<QString, unsigned long long int>& b){
        return a.second > b.second;
    });
}

void DataBase::insertNewData(QString pressedKey, unsigned long long int pressedTimes)
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

void DataBase::keyPressed(QString pressedKey)
{
    if(pressedKeyMap.contains(pressedKey)) {
        unsigned long long int newValue = pressedKeyMap.value(pressedKey) + 1;
        pressedKeyMap.insert(pressedKey, newValue);

        unsigned long long int newValueAtCurrentHour = currentHourPressedKeyMap.value(pressedKey) + 1;
        currentHourPressedKeyMap.insert(pressedKey, newValueAtCurrentHour);
    }
    else {
        pressedKeyMap.insert(pressedKey, 1);

        currentHourPressedKeyMap.insert(pressedKey, 1);
    }

    keyPressedTimes++;

    sortMap();

    emit keyPressedDone();
}

void DataBase::updateDatabase()
{
    QString currentDate = QDate::currentDate().toString("MM/dd/yy");
    int currentHour = QTime::currentTime().toString("h").toInt();

    if(dataBase.open()) {
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

        dataBase.close();
    }
    else {
        qDebug() << dataBase.lastError().text();
    }
}

void DataBase::readDatabase(int readMode)
{
    if(dataBase.open()) {
        pressedKeyMap.clear(); //clear the old data and re-read new data

        QSqlQuery readQuery;
        QString readQueryStr;

        QString currentDate = QDate::currentDate().toString("MM/dd/yy");
        int currentHour = QTime::currentTime().toString("h").toInt();

        QString lastMonth = currentDate;
        QString lastYear = currentDate; //lastMonth and lastYear must be define here, or it will jump the case in switch

        switch (readMode) {
        case 1: //read database within an hour
            readQueryStr = QString("SELECT PressedKey, PressedTimes FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(currentDate).arg(currentHour);
            break;
        case 2: //read database within a day
            readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# GROUP BY PressedKey").arg(currentDate);
            break;
        case 3:
            if(QString(currentDate[1]).toInt() > 1) {
                lastMonth.replace(QString(lastMonth[1]), QString::number(QString(lastMonth[1]).toInt() - 1)); //current month minus one minus to get last month
            }
            else { //if it is January, then
                lastMonth.replace(QString(lastMonth[0]), QString::number(1));
                lastMonth.replace(QString(lastMonth[1]), QString::number(2)); //set the month to December
                lastMonth.replace(QString(lastMonth[7]), QString::number(QString(lastMonth[7]).toInt() - 1)); //and current year minus one year to get last year
            }

            readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(lastMonth);
            break;
        case 4:
            lastYear.replace(QString(lastYear[7]), QString::number(QString(lastYear[7]).toInt() - 1)); //current year minus one year to get last year

            readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey").arg(currentDate).arg(lastYear);
            break;
        default:
            break;
        }

        readQuery.exec(readQueryStr);
        while(readQuery.next()) {
            pressedKeyMap.insert(readQuery.value(0).toString(), readQuery.value(1).toInt());
        }

        dataBase.close();
    }
    else {
        qDebug() << dataBase.lastError().text();
    }

    sortMap();

    keyPressedTimes = 0; //clear the old pressed times and re-count
    for(int i = 0; i < mapVector.size(); i++) {
        keyPressedTimes += mapVector[i].second;
    }
}

void DataBase::updateTimer()
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    updateDatabase();

    readDatabase(2); //because in a peroid of time, the data will store to database, and map and vector will be cleared, so when updating database, it needs to re-read data to map and vector
}

void clearDatabase()
{
    if(dataBase.open()) {
        QSqlQuery deleteQuery;
        deleteQuery.exec("DELETE FROM Data");
        dataBase.close();
    }
    else {
        qDebug() << dataBase.lastError().text();
    }
}
