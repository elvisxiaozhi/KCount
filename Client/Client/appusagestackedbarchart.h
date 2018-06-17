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
#include <QElapsedTimer>

class Callout;

QT_CHARTS_USE_NAMESPACE

class AppUsageStackedBarChart : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AppUsageStackedBarChart(QWidget *parent = nullptr, int mode = 1, QString title = "App Usage");

    void reloadChart();

private:
    QVector<std::pair<QString, int> > usageVec;
    QString lastAppName;
    QElapsedTimer timer;
    QChart *chart;
    QVector<QBarSet *> setVec;
    QHorizontalStackedBarSeries *series;
    QValueAxis *valueAxisX;
    Callout *m_tooltip;

    void loadChartData();    

private slots:
    void appChanged(QString);
    void hovered(bool, int);
};

#endif // APPUSAGESTACKEDBARCHART_H
