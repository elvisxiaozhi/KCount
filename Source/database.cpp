#include "database.h"
#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QSqlQuery>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    keyPressedTimes = 0;

//    QString filePlace = QDir::currentPath().replace("/", "\\") + "\\UserData.mdb";
    QString filePlace = QDir::currentPath().replace("/", "\\") + "\\Database1.mdb";
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

        for(int i = 0; i < mapVector.size(); i++) {
            QSqlQuery searchQuery;
            QString searchString = QString("SELECT PressedKey FROM Data WHERE PressedKey = '%1'").arg(mapVector[i].first);
            searchQuery.exec(searchString);

            if(isQueryFound(searchQuery) == true) {
                qDebug() << "Found"; //update query here
            }
            else {
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO Data (CreatedHour, PressedKey)"
                                    "VALUES(:CreatedHour, :PressedKey);");
                int pressedTimes = mapVector[i].second; //can not bind this value directly to the next line, do not know why
                insertQuery.bindValue(":CreatedHour", pressedTimes);
                insertQuery.bindValue(":PressedKey", mapVector[i].first);
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
