#include <QCoreApplication>
#include "database.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    Database database;
    Connection connection;

    return a.exec();
}
