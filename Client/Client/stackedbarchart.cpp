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

    *leftSet << 1 << 2 << 3 << 4 << 5 << 6;
    *rightSet << 5 << 0 << 0 << 4 << 0 << 7;

    barAxisX = new QBarCategoryAxis(chart);

    QStringList categories;
    categories << "Jan" << "Feb" << "Mar" << "Apr" << "May" << "Jun";
    barAxisX->append(categories);

    series->append(leftSet);
    series->append(rightSet);

    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setAxisX(barAxisX, series);
}
