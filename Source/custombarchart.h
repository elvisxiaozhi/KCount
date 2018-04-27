#ifndef CUSTOMBARCHART_H
#define CUSTOMBARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>

QT_CHARTS_USE_NAMESPACE

class CustomBarChart : public QChart
{
public:
    CustomBarChart(QString, int);
    QBarSet *barSet;
    QStringList barCategories;
    QWidget *barChartWidget;
};

#endif // CUSTOMBARCHART_H
