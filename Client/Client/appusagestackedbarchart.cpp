#include "appusagestackedbarchart.h"
#include "database.h"
#include <QDebug>

QVector<std::pair<QString, int> > AppUsageStackedBarChart::usageVec = {};

AppUsageStackedBarChart::AppUsageStackedBarChart(QWidget *parent, int mode) : QWidget(parent)
{
    usageVec = Database::returnAppVec(mode);

    loadChartData();
}

void AppUsageStackedBarChart::loadChartData()
{
    chart = new QChart();
    chart->setTitle("App Usage");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);
    mainVLayout->addWidget(chartView);

    series = new QHorizontalStackedBarSeries(chart);

    valueAxisX = new QValueAxis(chart);
    valueAxisX->setGridLineVisible(false);
    valueAxisX->setTickCount(3);
//    valueAxisX->setRange(0, 23.2);
    valueAxisX->setLabelFormat("%d");

    for(int i = 0; i < usageVec.size(); ++i) {
        QBarSet *barSet = new QBarSet(usageVec[i].first, series);
        barSet->append(usageVec[i].second);
        setVec.push_back(barSet);
    }

    for(int i = 0; i < setVec.size(); ++i) {
        series->append(setVec[i]);
    }

    chart->addSeries(series);
    chart->setAxisX(valueAxisX, series);
}

void AppUsageStackedBarChart::reloadChart()
{
    delete mainVLayout;
    delete chart;
    usageVec.clear();

    loadChartData();
}
