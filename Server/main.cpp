#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("TestingDB");
    db.setUserName("root");
    db.setPassword("password");
    if(db.open()) {
        qDebug() << "Opened";
        db.close();
    }
    else {
        qDebug() << db.lastError().text();
    }

    return a.exec();
}
