#include "stackedbarchart.h"
#include <QVBoxLayout>
#include <QDebug>
#include "database.h"
#include <QDate>

QMap<int, std::pair<int, int> > StackedBarChart::dailyMap = {};
QMap<int, std::pair<int, int> > StackedBarChart::weeklyMap = {};

StackedBarChart::StackedBarChart(QWidget *parent) : QWidget(parent)
{
    dailyMap = Database::returnStackedBarChartData();

    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueIcy);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    setLayout(mainVLayout);

    series = new QStackedBarSeries(chart);

    leftSet = new QBarSet("LeftSet", series);
    rightSet = new QBarSet("RightSet", series);

    series->append(leftSet);
    series->append(rightSet);

    valueAxisX = new QValueAxis(chart);
    valueAxisX->setGridLineVisible(false);
    valueAxisX->setTickCount(3);
    valueAxisX->setRange(0, 23.2);
    valueAxisX->setLabelFormat("%d");

    axisY = new QValueAxis(chart);
    axisY->setGridLineVisible(false);
    axisY->setLabelFormat("%d");

    barAxisX = new QBarCategoryAxis(chart);
    barAxisX->setGridLineVisible(false);
    barAxisX->append(barCategories);

    loadChartData(2);
}

void StackedBarChart::loadChartData(int mode)
{
    switch (mode) {
    case 1:
        for(auto it = dailyMap.cbegin(); it != dailyMap.cend(); ++it) {
            leftSet->append(it.value().first);
            rightSet->append(it.value().second);
        }

        chart->addSeries(series);
        chart->setAxisX(valueAxisX, series);
        break;
    case 2: { //weekly
//       for(auto it = weeklyMap.cbegin(); it != weeklyMap.cend(); ++it) {
//           barCategories.append(QDate::currentDate().addDays(std::distance(weeklyMap.cbegin(), it) - 6).toString("d"));
//           set->append(it.value());
//       }
        for(int i = 0; i < 7; ++i) {
            barCategories.append(QDate::currentDate().addDays(i - 6).toString("d"));
            leftSet->append(i);
            rightSet->append(7 - i);
        }

        chart->addSeries(series);

        chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
    }
        break;
    default:
        break;
    }

    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //it must be after setAcisY()
}
