#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);

private:
    QSqlDatabase dataBase;
    QString accessString;

signals:

public slots:
    void connectToDataBase();
};

#endif // DATABASE_H
