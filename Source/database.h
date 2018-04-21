#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QTimer>

extern void clearDatabase();
extern QSqlDatabase dataBase;

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    unsigned long long int keyPressedTimes;
    QMap<QString, unsigned long long int> pressedKeyMap;
    QVector<std::pair<QString, unsigned long long int>> mapVector; //store map, in order to sort map value

//    static QSqlDatabase testDatabase;

private:
    QString filePath;
//    QSqlDatabase dataBase;
    bool isQueryFound(QSqlQuery);
    QTimer *timer;
    QStringList currentTimeStringList;
    void makeDataFile();
    void setTimer();
    void sortMap();
    void insertNewData(int);

signals:
    void keyPressedDone();
    void databaseOpened();

public slots:
    void updateDatabase();
    void readDatabase();

private slots:
    void keyPressed(QString);
    void updateTimer();
};

#endif // DATABASE_H
