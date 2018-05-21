#ifndef BARCHART_H
#define BARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>

QT_CHARTS_USE_NAMESPACE

class BarChart : public QWidget
{
    Q_OBJECT
public:
    BarChart(QWidget *parent = nullptr, int mode = 1);

private:
    QBarSet *set;
    QBarSeries *series;
    QDateTimeAxis *axisX;
    QLineSeries *lineSeries;
    QValueAxis *axisY;
    QStringList categories;
    QChart *chart;
    QChartView *view;

    void setChartData(int);
};

#endif // BARCHART_H
