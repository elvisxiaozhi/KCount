#ifndef BARCHART_H
#define BARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>

QT_CHARTS_USE_NAMESPACE

class BarChart : public QWidget
{
    Q_OBJECT
public:
    BarChart(QWidget *parent = nullptr);

private:
    QBarSet *set;
    QBarSeries *series;
    QBarCategoryAxis *axis;
    QValueAxis *axisY;
    QStringList categories;
    QChart *chart;
    QChartView *view;

    void setChartData();
};

#endif // BARCHART_H
