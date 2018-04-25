#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include "database.h"

class Statistics : public QMainWindow
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);
    void setBarChart();

private:
    void closeEvent(QCloseEvent *);
    DataBase setDatabase;

signals:

public slots:


};

#endif // STATISTICS_H
