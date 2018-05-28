#include "stackedbarchart.h"
#include <QVBoxLayout>

StackedBarChart::StackedBarChart(QWidget *parent) : QWidget(parent)
{
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

    loadChartData();

//    barAxisX = new QBarCategoryAxis(chart);

//    QStringList categories;
//    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
//    barAxisX->append(categories);
}

void StackedBarChart::loadChartData()
{
    for(int i = 0; i < 24; ++i) {
        leftSet->append(i);
        rightSet->append(24 - i);
    }

    chart->addSeries(series);
    chart->setAxisX(valueAxisX, series);

    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //it must be after setAcisY()
}
