#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>

class Statistics : public QMainWindow
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);

private:
    QWidget *mainWidget;
    QWidget *dailyBarChartWidget, *weeklyBarChartWidget, *monthlyBarChartWidget, *yearlyBarChartWidget;
    QWidget *dailyPieChartWidget;
    QVBoxLayout *mainVLayout;
    QTabWidget *barTabWidget, *pieTabWidget;
    QPushButton *barChartBtn, *pieChartBtn;
    void setLayout();
    void closeEvent(QCloseEvent *);
    void setDailyBarChart();
    void setWeeklyBarChart();
    void setMonthlyBarChart();
    void setYearlyBarChart();
    void setDailyPieChart();

signals:

public slots:

private slots:
    void resizeWindow(int);
    void showBarChart();
    void showPieChart();
};

#endif // STATISTICS_H
