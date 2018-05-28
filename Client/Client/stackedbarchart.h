#ifndef STACKEDBARCHART_H
#define STACKEDBARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

class StackedBarChart : public QWidget
{
    Q_OBJECT
public:
    explicit StackedBarChart(QWidget *parent = nullptr);
    static QMap<int, std::pair<int, int> > dailyMap;
    static QMap<int, std::pair<int, int> > weeklyMap;

private:
    QChart *chart;
    QBarSet *leftSet, *rightSet;
    QStackedBarSeries *series;
    QStringList barCategories;
    QBarCategoryAxis *barAxisX;
    QValueAxis *valueAxisX, *axisY;

    void loadChartData(int);

signals:

public slots:
};

#endif // STACKEDBARCHART_H
