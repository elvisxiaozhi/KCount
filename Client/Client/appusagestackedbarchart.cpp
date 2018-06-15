#include "appusagestackedbarchart.h"
#include "database.h"
#include <QDebug>
#include "signalemitter.h"
#include "callout.h"

AppUsageStackedBarChart::AppUsageStackedBarChart(QWidget *parent, int mode)
    : QGraphicsView(new QGraphicsScene, parent),
      chart(0),
      m_tooltip(0)
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

    chart = new QChart();
    chart->setTitle("App Usage");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->setDropShadowEnabled(true);
    chart->setContentsMargins(0, 0, 0, 0);
    chart->setMinimumSize(600, 300);

    scene()->addItem(chart);

    loadChartData(); 

    connect(Emitter::Instance(), &SignalEmitter::appChanged, this, &AppUsageStackedBarChart::appChanged);
}

void AppUsageStackedBarChart::loadChartData()
{
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
    if (m_tooltip == 0)
        m_tooltip = new Callout(chart);

    if (status) {
        QBarSet *barSender = qobject_cast<QBarSet *>(sender());
        m_tooltip->setText(QString(barSender->label() + ": ") + QString::number(barSender->sum()));

        QPointF p(6.6, 0.15);

        m_tooltip->setAnchor(p);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    }
    else {
        m_tooltip->hide();
    }
}

void AppUsageStackedBarChart::reloadChart()
{
    delete valueAxisX;
    chart->removeSeries(series);
    series->clear();
    delete series;
    setVec.clear();

    loadChartData();
}
