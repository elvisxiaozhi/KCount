#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>
#include <QTimer>
#include <QSettings>

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
    QMap<QString, unsigned long long int> currentHourPressedKeyMap;
    static QString dataPath;
    static void deleteDataFile(QString);
    static QSettings appPathSetting;

private:
    QString filePath;
    bool isQueryFound(QSqlQuery) const;
    QTimer *timer;
    QStringList currentTimeStringList;
    void makeDataFile();
    void setTimer();
    void sortMap();
    void insertNewData(QString, unsigned long long int);

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
