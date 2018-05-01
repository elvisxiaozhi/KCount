#ifndef CUSTOMPIECHART_H
#define CUSTOMPIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QLabel>
#include <QMovie>
#include <QtCharts/QChartView>

QT_CHARTS_USE_NAMESPACE

class CustomPieChart
{
public:
    explicit CustomPieChart();
    QWidget *pieChartWidget;
    QPieSeries *series;
    QChartView *chartView;
    QLabel *loadingLbl;
    QMovie *loadingMovie;
    void showLoadingPage();
    void updateBarChartData(QMap<QString, int>);
};

#endif // CUSTOMPIECHART_H
