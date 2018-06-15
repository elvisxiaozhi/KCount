#include "appusagestackedbarchart.h"
#include "database.h"
#include <QDebug>
#include "signalemitter.h"

AppUsageStackedBarChart::AppUsageStackedBarChart(QWidget *parent, int mode)
    : QGraphicsView(new QGraphicsScene, parent),
      chart(0)
//      m_tooltip(0)
{
    timer.start();
    usageVec = Database::returnAppVec(mode);

    setFixedSize(600, 300);
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContentsMargins(0, 0, 0, 0);
    setFrameStyle(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);

    loadChartData(); 

    scene()->addItem(chart);

    connect(Emitter::Instance(), &SignalEmitter::appChanged, this, &AppUsageStackedBarChart::appChanged);
}

void AppUsageStackedBarChart::loadChartData()
{
    chart = new QChart();
    chart->setTitle("App Usage");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setDropShadowEnabled(true);
    chart->setContentsMargins(0, 0, 0, 0);
    chart->setMinimumSize(600, 300);

//    chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);

//    mainVLayout = new QVBoxLayout(this);
//    setLayout(mainVLayout);
//    mainVLayout->addWidget(chartView);

    series = new QHorizontalStackedBarSeries(chart);
    series->setLabelsVisible(true);

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

        connect(barSet, &QBarSet::hovered, this, &AppUsageStackedBarChart::hovered);
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

void AppUsageStackedBarChart::hovered(bool status, int)
{
    if(status) {
        QBarSet *barSender = qobject_cast<QBarSet *>(sender());
        qDebug() << barSender->label();
    }
}

void AppUsageStackedBarChart::reloadChart()
{
//    delete mainVLayout;
//    delete chart;
//    setVec.clear();

//    loadChartData();
}
