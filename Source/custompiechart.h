#ifndef CUSTOMPIECHART_H
#define CUSTOMPIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

class CustomPieChart
{
public:
    CustomPieChart();
    QWidget *pieChartWidget;
};

#endif // CUSTOMPIECHART_H
