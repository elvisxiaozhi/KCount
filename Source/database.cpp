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

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    makeDataFile();

    keyPressedTimes = 0;

    QString accessString = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(filePath);
    dataBase = QSqlDatabase::addDatabase("QODBC");
    dataBase.setDatabaseName(accessString);

    readDatabase();

    setTimer();

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &DataBase::keyPressed);
}

bool DataBase::isQueryFound(QSqlQuery searchQuery)
{
    while(searchQuery.next()) {
        return true;
    }
    return false;
}

void DataBase::makeDataFile()
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString dataPath = homePath.first() + "/AppData/Local/Keylogger";
    if(!QDir().exists(dataPath)) {
        QDir().mkdir(dataPath);
    }

    filePath = dataPath + "/UserData.mdb";
    if(!QFile().exists(filePath)) {
        QDir().rename(QDir::currentPath() + "/UserData.mdb", filePath);
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

void DataBase::insertNewData(int vectorIndex)
{
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO Data (CreatedDate, CreatedHour, PressedKey, PressedTimes)"
                        "VALUES(:CreatedDate, :CreatedHour, :PressedKey, :PressedTimes);");
    insertQuery.bindValue(":CreatedDate", QDate::currentDate().toString("MM/dd/yy"));
    QString CreatedHour = QTime::currentTime().toString("h");
    insertQuery.bindValue(":CreatedHour", CreatedHour.toInt());
    insertQuery.bindValue(":PressedKey", mapVector[vectorIndex].first);
    int pressedTimes = mapVector[vectorIndex].second; //can not bind this value directly to the next line, do not know why
    insertQuery.bindValue(":PressedTimes", pressedTimes);
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
    if(dataBase.open()) {
        qDebug() << "Database opened";
        for(int i = 0; i < mapVector.size(); i++) {
            QSqlQuery searchQuery;
            QString searchString = QString("SELECT * FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2 AND PressedKey = '%3'").arg(QDate::currentDate().toString("MM/dd/yy")).arg(QTime::currentTime().toString("h").toInt()).arg(mapVector[i].first);
            searchQuery.exec(searchString);

            if(isQueryFound(searchQuery) == true) {
                QSqlQuery searchPressedKeyQuery;
                QString searchPressedKeyString = QString("SELECT * FROM Data WHERE CreatedDate = #%1# AND PressedKey = '%2'").arg(QDate::currentDate().toString("MM/dd/yy")).arg(mapVector[i].first);
                searchPressedKeyQuery.exec(searchPressedKeyString);

                if(isQueryFound((searchPressedKeyQuery)) == true) {
                    QSqlQuery updateQuery;
                    QString updateString = QString("UPDATE Data SET PressedTimes = %1 WHERE PressedKey = '%2'").arg(QString::number(mapVector[i].second)).arg(mapVector[i].first);
                    updateQuery.exec(updateString);
                }
                else {
                    insertNewData(i);
                }
            }
            else {
                insertNewData(i);
            }
        }

        dataBase.close();
        qDebug() << "Database updated and closed";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }
}

void DataBase::readDatabase()
{
    if(dataBase.open()) {
        qDebug() << "Database opened and ready to read";

        QSqlQuery readQuery;
        QString readString = QString("SELECT PressedKey, PressedTimes FROM Data WHERE CreatedDate = #%1#").arg(QDate::currentDate().toString("MM/dd/yy"));
        readQuery.exec(readString);

        pressedKeyMap.clear();
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
    for(int i = 0; i < mapVector.size(); i++) {
        keyPressedTimes += mapVector[i].second;
    }
}

void DataBase::updateTimer()
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    updateDatabase();
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
