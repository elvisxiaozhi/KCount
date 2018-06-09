#ifndef APPUSAGESTACKEDBARCHART_H
#define APPUSAGESTACKEDBARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalPercentBarSeries>
#include <QtCharts/QHorizontalStackedBarSeries>

QT_CHARTS_USE_NAMESPACE

class AppUsageStackedBarChart : public QWidget
{
    Q_OBJECT
public:
    explicit AppUsageStackedBarChart(QWidget *parent = nullptr, int mode = 1);

    static QVector<std::pair<QString, int> > dailyVec;

private:
    QChart *chart;
    QVector<QBarSet *> setVec;
    QHorizontalStackedBarSeries *series;
};

#endif // APPUSAGESTACKEDBARCHART_H
