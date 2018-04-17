#include "database.h"
#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>
#include <QTime>
#include "signalemitter.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    keyPressedTimes = 0;

    QString filePlace = QDir::currentPath().replace("/", "\\") + "\\UserData.mdb";
    accessString = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(filePlace);
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

void DataBase::setTimer()
{
    currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    timer = new QTimer(this);
    timer->start(1000 * 60 * 60 - 1000 * 60 * QString(currentTimeStringList[1]).toInt() - 1000 * QString(currentTimeStringList[2]).toInt()); //1 sec * 60 (= 1 minute) * 60 (= 1 hour)
    connect(timer, &QTimer::timeout, this, &DataBase::updateTimer);
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

    mapVector.clear();
    QMap<QString, unsigned long long int>::iterator it;
    for(it = pressedKeyMap.begin(); it != pressedKeyMap.end(); it++) {
        mapVector.push_back(std::make_pair(it.key(), it.value()));
    }
    std::sort(mapVector.begin(), mapVector.end(), [=](std::pair<QString, unsigned long long int>& a, std::pair<QString, unsigned long long int>& b){
        return a.second > b.second;
    });

    emit keyPressedDone();
}

void DataBase::updateDatabase()
{
    if(dataBase.open()) {
        qDebug() << "Database opened";
        for(int i = 0; i < mapVector.size(); i++) {
            QSqlQuery searchQuery;
            QString searchString = QString("SELECT * FROM Data WHERE CreatedDate = #%1# AND PressedKey = '%2'").arg(QDate::currentDate().toString("MM/dd/yy")).arg(mapVector[i].first);
            searchQuery.exec(searchString);

            if(isQueryFound(searchQuery) == true) {
                QSqlQuery updateQuery;
                QString updateString = QString("UPDATE Data SET PressedTimes = %1 WHERE PressedKey = '%2'").arg(QString::number(mapVector[i].second)).arg(mapVector[i].first);
                updateQuery.exec(updateString);
            }
            else {
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO Data (CreatedDate, CreatedHour, PressedKey, PressedTimes)"
                                    "VALUES(:CreatedDate, :CreatedHour, :PressedKey, :PressedTimes);");
                insertQuery.bindValue(":CreatedDate", QDate::currentDate().toString("MM/dd/yy"));
                QString CreatedHour = QTime::currentTime().toString("h");
                insertQuery.bindValue(":CreatedHour", CreatedHour.toInt());
                insertQuery.bindValue(":PressedKey", mapVector[i].first);
                int pressedTimes = mapVector[i].second; //can not bind this value directly to the next line, do not know why
                insertQuery.bindValue(":PressedTimes", pressedTimes);
                insertQuery.exec();
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

        while(readQuery.next()) {
            pressedKeyMap.insert(readQuery.value(0).toString(), readQuery.value(1).toInt());
        }

        dataBase.close();
        qDebug() << "Database read and closed";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }

    mapVector.clear();
    QMap<QString, unsigned long long int>::iterator it;
    for(it = pressedKeyMap.begin(); it != pressedKeyMap.end(); it++) {
        mapVector.push_back(std::make_pair(it.key(), it.value()));
    }
    std::sort(mapVector.begin(), mapVector.end(), [=](std::pair<QString, unsigned long long int>& a, std::pair<QString, unsigned long long int>& b){
        return a.second > b.second;
    });
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
