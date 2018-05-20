#include "barchart.h"
#include <QVBoxLayout>
#include <QLabel>

BarChart::BarChart(QWidget *parent) : QWidget(parent)
{
    set = new QBarSet("Set");

    series = new QBarSeries();

    chart = new QChart();
    chart->setTitle("Bar Chart");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueIcy);


    axis = new QBarCategoryAxis();
    axis->setGridLineVisible(false);

    axisY = new QValueAxis();
    axisY->setGridLineVisible(false);
    axisY->setLabelFormat("%d");

    series->append(set);
    axis->append(categories);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    setLayout(mainVLayout);

    setChartData();
}

void BarChart::setChartData()
{
    *set << 1 << 2;
    categories << "Jan" << "Feb";

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setAxisX(axis, series);
    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //it must be after setAcisY()
}
