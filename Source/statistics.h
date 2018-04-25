#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "database.h"
#include <QTabWidget>

class Statistics : public QMainWindow
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);
    void setBarChart();

private:
    QWidget *mainWidget;
    QVBoxLayout *mainVLayout;
    QTabWidget *tabWidget;
    DataBase setDatabase;
    void setLayout();
    void closeEvent(QCloseEvent *);

signals:

public slots:


};

#endif // STATISTICS_H
