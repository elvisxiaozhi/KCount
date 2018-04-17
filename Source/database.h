#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QVector>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    unsigned long long int keyPressedTimes;
    QMap<QString, unsigned long long int> pressedKeyMap;
    QVector<std::pair<QString, unsigned long long int>> mapVector; //store map, in order to sort map value

private:
    QSqlDatabase dataBase;
    QString accessString;
    bool isQueryFound(QSqlQuery);

signals:
    void keyPressedDone();

public slots:
    void keyPressed(QString);
    void connectToDataBase();
};

#endif // DATABASE_H
