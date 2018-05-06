#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QTextStream>
#include <windows.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    qDebug() << "Type your database password: ";
    QTextStream textStream(stdin);
    QString password = textStream.readLine();

    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("TestingDB");
    db.setUserName("root");
    db.setPassword(password);
    if(db.open()) {
        qDebug() << "Opened";
        db.close();
    }
    else {
        qDebug() << db.lastError().text();
    }

    return a.exec();
}
