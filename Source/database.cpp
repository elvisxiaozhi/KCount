#include "database.h"
#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>
#include <QDate>
#include <QTime>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    keyPressedTimes = 0;

    QString filePlace = QDir::currentPath().replace("/", "\\") + "\\UserData.mdb";
    accessString = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(filePlace);
}

bool DataBase::isQueryFound(QSqlQuery searchQuery)
{
    while(searchQuery.next()) {
        return true;
    }
    return false;
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

void DataBase::connectToDataBase()
{
    dataBase = QSqlDatabase::addDatabase("QODBC");
    dataBase.setDatabaseName(accessString);

    if(dataBase.open()) {
        qDebug() << "Database opened";

//        QSqlQuery deleteQuery;
//        deleteQuery.exec("DELETE FROM Data");

        qDebug() << QDate::currentDate().toString("MM/dd/yy") << QTime::currentTime().toString("h");

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
        qDebug() << "Database closed";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }
}
