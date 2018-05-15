#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

class Database : public QObject
{
    Q_OBJECT
public:
    Database();
    static unsigned long int returnTotalPressedTimes(int);
    static void updateTotalPressedTimes(QString);
    static QVector<std::pair<QString, unsigned long int>> returnKeyVec(int);
    static void updatePressedKeyToDB(const QMap<QString, unsigned long int> &);

private:
    static QSqlDatabase database;
    static QString currentDate;
    static int currentHour;

    static bool isQueryFound(QSqlQuery); 
};

#endif // DATABASE_H
