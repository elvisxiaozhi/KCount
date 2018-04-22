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

    readDatabaseByDay();

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
    mapVector.clear();
    QMap<QString, unsigned long long int>::iterator it;
    for(it = pressedKeyMap.begin(); it != pressedKeyMap.end(); it++) {
        mapVector.push_back(std::make_pair(it.key(), it.value()));
    }
    std::sort(mapVector.begin(), mapVector.end(), [=](std::pair<QString, unsigned long long int>& a, std::pair<QString, unsigned long long int>& b){
        return a.second > b.second;
    });
}

void DataBase::insertNewData(int vectorIndex, int keyPressedTimesEarlierToday)
{
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO Data (CreatedDate, CreatedHour, PressedKey, PressedTimes)"
                        "VALUES(:CreatedDate, :CreatedHour, :PressedKey, :PressedTimes);");
    insertQuery.bindValue(":CreatedDate", QDate::currentDate().toString("MM/dd/yy"));
    QString CreatedHour = QTime::currentTime().toString("h");
    insertQuery.bindValue(":CreatedHour", CreatedHour.toInt());
    insertQuery.bindValue(":PressedKey", mapVector[vectorIndex].first);
    int pressedTimes = mapVector[vectorIndex].second; //can not bind this value directly to the next line, do not know why
    insertQuery.bindValue(":PressedTimes", pressedTimes - keyPressedTimesEarlierToday);
    insertQuery.exec();
}

void DataBase::keyPressed(QString pressedKey)
{
    if(pressedKeyMap.contains(pressedKey)) {
        unsigned long long int newValue = pressedKeyMap.value(pressedKey) + 1;
        pressedKeyMap.insert(pressedKey, newValue);
    }
    else {
        pressedKeyMap.insert(pressedKey, 1);
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
        qDebug() << "Database opened";
        for(int i = 0; i < mapVector.size(); i++) {
            QSqlQuery hasPressedKeyAtCurrentHourQuery;
            QString hasPressedKeyAtCurrentHourStr = QString("SELECT * FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2 AND PressedKey = '%3'").arg(currentDate).arg(currentHour).arg(mapVector[i].first);
            hasPressedKeyAtCurrentHourQuery.exec(hasPressedKeyAtCurrentHourStr);

            if(isQueryFound(hasPressedKeyAtCurrentHourQuery) == true) {
                QSqlQuery hasPressedKeyEarlierTodayQuery;
                QString hasPressedKeyEarlierTodayStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# AND CreatedHour < %2 AND PressedKey = '%3'").arg(QDate::currentDate().toString("MM/dd/yy")).arg(QTime::currentTime().toString("h").toInt()).arg(hasPressedKeyAtCurrentHourQuery.value(3).toString());
                hasPressedKeyEarlierTodayQuery.exec(hasPressedKeyEarlierTodayStr);
                while(hasPressedKeyEarlierTodayQuery.next()) {
                    int keyPressedTimesEarlierToday = hasPressedKeyEarlierTodayQuery.value(0).toInt();
                    QSqlQuery updatePessedKeyAtCurrentHourQuery;
                    QString updatePessedKeyAtCurrentHourStr = QString("UPDATE Data SET PressedTimes = %1 WHERE CreatedDate = #%2# AND CreatedHour = %3 AND PressedKey = '%4'").arg(QString::number(mapVector[i].second - keyPressedTimesEarlierToday)).arg(currentDate).arg(currentHour).arg(mapVector[i].first);
                    updatePessedKeyAtCurrentHourQuery.exec(updatePessedKeyAtCurrentHourStr);
                }
            }
            else {
                QSqlQuery hasPressedKeyEarlierTodayQuery;
                QString hasPressedKeyEarlierTodayStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# AND CreatedHour < %2 AND PressedKey = '%3'").arg(currentDate).arg(currentHour).arg(mapVector[i].first);
                hasPressedKeyEarlierTodayQuery.exec(hasPressedKeyEarlierTodayStr);
                if(isQueryFound(hasPressedKeyEarlierTodayQuery) == true) {
                    int keyPressedTimesEarlierToday = hasPressedKeyEarlierTodayQuery.value(0).toInt();
                    insertNewData(i, keyPressedTimesEarlierToday);
                }
                else {
                    insertNewData(i, 0);
                }
            }
        }

        dataBase.close();
        qDebug() << "Database updated and closed";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }
}

void DataBase::readDatabaseByHour()
{
    if(dataBase.open()) {
        qDebug() << "Database opened and ready to read by hour";

        pressedKeyMap.clear();

        QSqlQuery readQuery;
        QString readString = QString("SELECT PressedKey, PressedTimes FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(QDate::currentDate().toString("MM/dd/yy")).arg(QTime::currentTime().toString("h").toInt());
        readQuery.exec(readString);

        while(readQuery.next()) {
            pressedKeyMap.insert(readQuery.value(0).toString(), readQuery.value(1).toInt());
        }

        dataBase.close();
        qDebug() << "Database read and closed";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }

    sortMap();

    keyPressedTimes = 0;
    for(int i = 0; i < mapVector.size(); i++) {
        keyPressedTimes += mapVector[i].second;
    }
}

void DataBase::readDatabaseByDay()
{
    if(dataBase.open()) {
        qDebug() << "Database opened and ready to read";

        pressedKeyMap.clear();

        QSqlQuery readQuery;
        QString readString = QString("SELECT PressedKey, PressedTimes FROM Data WHERE CreatedDate = #%1#").arg(QDate::currentDate().toString("MM/dd/yy"));
        readQuery.exec(readString);

        while(readQuery.next()) {
            QSqlQuery countQuery;
            QString countString = QString("SELECT SUM(PressedTimes) FROM Data WHERE PressedKey = '%1'").arg(readQuery.value(0).toString());
            countQuery.exec(countString);
            while(countQuery.next()) {
                pressedKeyMap.insert(readQuery.value(0).toString(), countQuery.value(0).toInt()); //insert pressed key and the sum of PressedTimes to map
            }
        }

        dataBase.close();
        qDebug() << "Database read and closed";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }

    sortMap();

    keyPressedTimes = 0;
    for(int i = 0; i < mapVector.size(); i++) {
        keyPressedTimes += mapVector[i].second;
    }
}

void DataBase::updateTimer()
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    updateDatabase();

    readDatabaseByDay(); //because each hour the data will store to database once, and map and vector will be cleared in each hour, so when updating database, it needs to reload data to map and vector
}

void clearDatabase()
{
    if(dataBase.open()) {
        qDebug() << "Database opened and ready to delete";
        QSqlQuery deleteQuery;
        deleteQuery.exec("DELETE FROM Data");
        dataBase.close();
        qDebug() << "Database has been cleared";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }
}
