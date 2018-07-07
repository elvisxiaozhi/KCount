#ifndef STACKEDBARCHART_H
#define STACKEDBARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QLabel>
#include <QtCharts/QLineSeries>
#include <QtCharts/QDateTimeAxis>

class Callout;

QT_CHARTS_USE_NAMESPACE

class StackedBarChart : public QGraphicsView
{
    Q_OBJECT
public:
    StackedBarChart(QWidget *parent = nullptr, int mode = 1);

    static QMap<int, std::pair<int, int> > dailyMap;
    static QMap<int, std::pair<int, int> > weeklyMap;
    static QMap<int, std::pair<int, int> > monthlyMap;
    static QMap<int, std::pair<int, int> > yearlyMap;

    void reloadChart(int);

private:
    QChart *chart;
    QBarSet *leftSet, *rightSet;
    QStackedBarSeries *series;
    QStringList barCategories;
    QBarCategoryAxis *barAxisX;
    QValueAxis *valueAxisX, *axisY;
    QDateTimeAxis *dateAxisX;
    QLineSeries *lineSeries;
    QLabel *label;
    QChartView *chartView;
    QGraphicsRectItem hoverItem;
    Callout *m_tooltip;

    void loadChartData(int);
    void reloadChart(QMap<int, std::pair<int, int> > &, int);

private slots:
    void changeBarColor(bool, int);
};

#endif // STACKEDBARCHART_H
