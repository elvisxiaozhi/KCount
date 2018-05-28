#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include "singleton.h"

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    static unsigned long int returnTotalPressedTimes(int);
    static QVector<std::pair<QString, unsigned long int>> returnKeyVec(int);
    static void updatePressedKeyToDB(const QMap<QString, unsigned long int> &);
    static unsigned long int returnClickedTimes(QString, int);
    static void updateLeftClickToDB(const int &, const int &);
    static QMap<int, unsigned long int> returnBarChartData(int readMode = 1);
    static QMap<int, std::pair<int, int> > returnStackedBarChartData();

private:
    static QSqlDatabase database;
    static QString currentDate;
    static int currentHour;
    QString dataPath;
    QString databaseLoc;

    static bool isQueryFound(QSqlQuery); 
    void createDataFile();
    void showErrorMsBox();

public slots:
    static void timeout();
};

#endif // DATABASE_H
