#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>

class Statistics : public QMainWindow
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);

private:
    QWidget *mainWidget, *dailyBarChartWidget, *weeklyBarChartWidget, *monthlyBarChartWidget, *yearlyBarChartWidget;
    QVBoxLayout *mainVLayout;
    QTabWidget *tabWidget;
    void setLayout();
    void closeEvent(QCloseEvent *);
    void setDailyBarChart();
    void setWeeklyBarChart();
    void setMonthlyBarChart();
    void setYearlyBarChart();

signals:

public slots:

private slots:
    void resizeWindow(int);
};

#endif // STATISTICS_H
