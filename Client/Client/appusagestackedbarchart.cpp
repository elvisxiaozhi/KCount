#include "appusagestackedbarchart.h"
#include "database.h"
#include <QDebug>
#include "signalemitter.h"

AppUsageStackedBarChart::AppUsageStackedBarChart(QWidget *parent, int mode) : QWidget(parent)
{
    timer.start();
    usageVec = Database::returnAppVec(mode);

    loadChartData();

    connect(Emitter::Instance(), &SignalEmitter::appChanged, this, &AppUsageStackedBarChart::appChanged);
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

    int totalUsage = 0;
    std::for_each(usageVec.begin(), usageVec.end(),
                  [this, &totalUsage](const std::pair<QString, int> &element) {
       return totalUsage += element.second;
    });

    valueAxisX = new QValueAxis(chart);
    valueAxisX->setGridLineVisible(false);
    valueAxisX->setTickCount(3);
    valueAxisX->setRange(0, totalUsage);
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

void AppUsageStackedBarChart::appChanged(QString processName)
{
    QRegExp regEx("\\\\");
    QStringList processNameList = processName.split(regEx);
    QString appName = QString(processNameList.at(processNameList.size() - 1)).split(".exe").first();
    int elapsedTime = timer.elapsed();
    int usedTime = 0;
    if(timer.elapsed() % 1000 >= 500) {
        usedTime = std::ceil(elapsedTime / 1000);
    }
    else {
        usedTime = std::floor(elapsedTime / 1000);
    }

    auto it = std::find_if(
                usageVec.begin(), usageVec.end(),
                [this](const std::pair<QString, int> &element){ return element.first == lastAppName; }
                );
    if(it != usageVec.end()) {
        (*it).second += usedTime;
    }
    else {
        usageVec.push_back(std::make_pair(lastAppName, usedTime));
    }

    timer.start(); //output first, then restart the timer
    lastAppName = appName;
}

void AppUsageStackedBarChart::reloadChart()
{
    delete mainVLayout;
    delete chart;
    setVec.clear();

    loadChartData();
}
