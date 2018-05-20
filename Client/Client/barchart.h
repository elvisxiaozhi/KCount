#ifndef BARCHART_H
#define BARCHART_H

#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

QT_CHARTS_USE_NAMESPACE

class BarChart : public QChart
{
public:
    BarChart();
};

#endif // BARCHART_H
