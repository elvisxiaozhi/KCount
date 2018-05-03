#ifndef CUSTOMBARCHART_H
#define CUSTOMBARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QLabel>
#include <QMovie>

QT_CHARTS_USE_NAMESPACE

class CustomBarChart : public QChart
{
public:
    CustomBarChart();
    QBarSet *barSet;
    QBarSeries *barSeries;
    QBarCategoryAxis *axis;
    QStringList barCategories;
    QWidget *barChartWidget;
    QChartView *chartView;
    QValueAxis *axisY;
    QLabel *totalLbl, *averageLbl, *loadingLbl;
    QMovie *loadingMovie;
    void updateBarChartData(int, QVector<int>);
    void showLoadingPage();
    void setWindowStyleSheet();

private:
    bool IsFirstRun;
};

#endif // CUSTOMBARCHART_H
