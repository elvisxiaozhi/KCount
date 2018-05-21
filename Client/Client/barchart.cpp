#include "barchart.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDateTime>

BarChart::BarChart(QWidget *parent) : QWidget(parent)
{
    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueIcy);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    series = new QBarSeries(chart);
    set = new QBarSet("BarSet", series);
    series->append(set);

    axisX = new QDateTimeAxis(chart);
    axisX->setFormat("MMM dd");
    axisX->setGridLineVisible(false);

    axisY = new QValueAxis(chart);
    axisY->setGridLineVisible(false);
    axisY->setLabelFormat("%d");

    lineSeries = new QLineSeries(chart);
    lineSeries->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    setLayout(mainVLayout);

    setChartData();
}

void BarChart::setChartData()
{    
    int daysInMonth = QDate::currentDate().daysInMonth();
    for(int i = 0; i < daysInMonth; ++i) {
        lineSeries->append(QDateTime::currentDateTime().addDays(i - daysInMonth + 1).toMSecsSinceEpoch(), 0);
        set->append(i);
    }

    chart->addSeries(series);
    chart->addSeries(lineSeries);
    chart->setAxisX(axisX, lineSeries);
    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //it must be after setAcisY()
}
