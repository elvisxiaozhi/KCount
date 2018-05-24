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
#include <QtCharts/QBarCategoryAxis>
#include <QLabel>

QT_CHARTS_USE_NAMESPACE

class BarChart : public QWidget
{
    Q_OBJECT
public:
    BarChart(QWidget *parent = nullptr, int mode = 1);

    void reloadChart(int);

private:
    QBarSet *set;
    QBarSeries *series;
    QDateTimeAxis *dateAxisX;
    QLineSeries *lineSeries;
    QValueAxis *valueAxisX, *axisY;
    QStringList categories;
    QBarCategoryAxis *barAxisX;
    QStringList barCategories;
    QChart *chart;
    QChartView *chartView;
    QLabel *label;

    void loadChartData(int);

private slots:
    void testing(bool, int);
};

#endif // BARCHART_H
