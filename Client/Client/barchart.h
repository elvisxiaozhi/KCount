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

class Callout;

QT_CHARTS_USE_NAMESPACE

class BarChart : public QGraphicsView
{
    Q_OBJECT
public:
    BarChart(QWidget *parent = nullptr, int mode = 1);

    static QMap<int, unsigned long int> dailyMap;
    static QMap<int, unsigned long int> weeklyMap;
    static QMap<int, unsigned long int> monthlyMap;
    static QMap<int, unsigned long int> yearlyMap;

    void reloadChart(int);

private:
    QBarSet *set;
    QBarSeries *series;
    QDateTimeAxis *dateAxisX;
    QLineSeries *lineSeries;
    QValueAxis *valueAxisX, *axisY;
    QBarCategoryAxis *barAxisX;
    QStringList barCategories;
    QChart *chart;
    QLabel *label;
    QChartView *chartView;
    QGraphicsRectItem hoverItem;
    QBrush labelBrush;
    Callout *m_tooltip;

    void loadChartData(int);
    void reloadChart(QMap<int, unsigned long int> &, int);

private slots:
    void changeBarColor(bool, int);
};

#endif // BARCHART_H
