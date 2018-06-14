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
#include <QElapsedTimer>

QT_CHARTS_USE_NAMESPACE

class AppUsageStackedBarChart : public QWidget
{
    Q_OBJECT
public:
    explicit AppUsageStackedBarChart(QWidget *parent = nullptr, int mode = 1);

    void reloadChart();

private:
    QVector<std::pair<QString, int> > usageVec;
    QString lastAppName;
    QElapsedTimer timer;
    QVBoxLayout *mainVLayout;
    QChart *chart;
    QChartView *chartView;
    QVector<QBarSet *> setVec;
    QHorizontalStackedBarSeries *series;
    QValueAxis *valueAxisX;

    void loadChartData();

private slots:
    void appChanged(QString);
    void hovered(bool, int);
};

#endif // APPUSAGESTACKEDBARCHART_H
