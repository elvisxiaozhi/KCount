#ifndef CUSTOMPIECHART_H
#define CUSTOMPIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

class CustomPieChart
{
public:
    explicit CustomPieChart();
    QWidget *pieChartWidget;
    QPieSeries *series;
    void updateBarChartData(QMap<QString, int>);
};

#endif // CUSTOMPIECHART_H
