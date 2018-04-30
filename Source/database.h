#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QTimer>
#include <QSettings>

//static void clearDatabase();

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr);
    unsigned long int totalPressedTimes;
    QMap<QString, unsigned long int> pressedKeyMap;
    QVector<std::pair<QString, unsigned long int>> mapVector; //store map, in order to sort map value
    QMap<QString, unsigned long int> currentHourPressedKeyMap;
    static QSqlDatabase sqlDatabase;
    static QString dataPath;
    static void deleteDataFile(QString);
    static void clearDatabase();
    static QSettings appPathSetting;
    static int returnTotalPressedTimes(QString);
    static QMap<QString, int> returnFrequentlyPressedKeyMap(QString);

private:
    QString filePath;
    bool isQueryFound(QSqlQuery) const;
    QTimer *timer;
    QStringList currentTimeStringList;
    void makeDataFile();
    void setTimer();
    void sortMap();
    void insertNewData(QString, unsigned long int);

signals:
    void keyPressedDone();
    void databaseOpened();

public slots:
    void updateDatabase();
    void readDatabase(int);

private slots:
    void keyPressed(QString);
    void updateTimer();
};

#endif // DATABASE_H
