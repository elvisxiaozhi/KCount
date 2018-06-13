#ifndef APPUSAGESTACKEDBARCHART_H
#define APPUSAGESTACKEDBARCHART_H

#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>

QT_CHARTS_USE_NAMESPACE

class AppUsageStackedBarChart : public QWidget
{
    Q_OBJECT
public:
    explicit AppUsageStackedBarChart(QWidget *parent = nullptr, int mode = 1);

    static QVector<std::pair<QString, int> > usageVec;

    void reloadChart();

private:
    QVBoxLayout *mainVLayout;
    QChart *chart;
    QChartView *chartView;
    QVector<QBarSet *> setVec;
    QHorizontalStackedBarSeries *series;
    QValueAxis *valueAxisX;

    void loadChartData();
};

#endif // APPUSAGESTACKEDBARCHART_H
