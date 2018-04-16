#include "database.h"
#include <QSqlError>
#include <QDir>
#include <QDebug>

//#define ACCESS "Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ=C:\\Users\\Theodore\\Desktop\\UserData.mdb"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    QString filePlace = QDir::currentPath().replace("/", "\\") + "\\UserData.mdb";
    accessString = QString("Driver={Microsoft Access Driver (*.mdb)}; FIL={MS Access}; DBQ= %1").arg(filePlace);
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
