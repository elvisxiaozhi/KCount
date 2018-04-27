#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include "custombarchart.h"

class Statistics : public QMainWindow
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);

private:
    QWidget *mainWidget;
    QWidget *dailyBarChartWidget;
    CustomBarChart *dailyBarChart, *weeklyBarChart, *monthlyBarChart, *yearlyBarChart;
    QWidget *dailyPieChartWidget;
    QVBoxLayout *mainVLayout;
    QTabWidget *barTabWidget, *pieTabWidget;
    QPushButton *barChartBtn, *pieChartBtn;
    void setLayout();
    void closeEvent(QCloseEvent *);
    void setDailyPieChart();

signals:

public slots:

private slots:
    void resizeWindow(int);
    void showBarChart();
    void showPieChart();
};

#endif // STATISTICS_H
