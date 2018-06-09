#include "appusagestackedbarchart.h"
#include <QVBoxLayout>
#include "database.h"
#include <QDebug>

QVector<std::pair<QString, int> > AppUsageStackedBarChart::dailyVec = {};

AppUsageStackedBarChart::AppUsageStackedBarChart(QWidget *parent, int mode) : QWidget(parent)
{
    switch (mode) {
    case 1:
        dailyVec = Database::returnAppVec(mode);
        break;
    default:
        break;
    }

    chart = new QChart();
    chart->setTitle("App Usage");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    setLayout(mainVLayout);

    series = new QHorizontalStackedBarSeries(chart);

    for(int i = 0; i < dailyVec.size(); ++i) {
        QBarSet *barSet = new QBarSet(dailyVec[i].first, series);
        barSet->append(dailyVec[i].second);
        setVec.push_back(barSet);
    }

    for(int i = 0; i < setVec.size(); ++i) {
        series->append(setVec[i]);
    }

    chart->addSeries(series);

    QBarCategoryAxis *axis = new QBarCategoryAxis(chart);
    chart->createDefaultAxes();
    chart->setAxisY(axis, series);
}
