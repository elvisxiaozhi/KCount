#include "database.h"
#include <QSqlError>
#include <QDir>
#include <QDebug>

//#define ACCESS "Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ=C:\\Users\\Theodore\\Desktop\\UserData.mdb"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    keyPressedTimes = 0;

    QString filePlace = QDir::currentPath().replace("/", "\\") + "\\UserData.mdb";
    accessString = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(filePlace);
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
        dataBase.close();
        qDebug() << "Database closed";
    }
    else {
        qDebug() << dataBase.lastError().text();
    }
}
