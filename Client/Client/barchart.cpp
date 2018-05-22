#include "barchart.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QDateTime>
#include "mostpressed.h"

BarChart::BarChart(QWidget *parent, int mode) : QWidget(parent)
{
    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueIcy);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    series = new QBarSeries(chart);
    set = new QBarSet("BarSet", series);
    series->append(set);

    dateAxisX = new QDateTimeAxis(chart);
    dateAxisX->setGridLineVisible(false);

    barAxisX = new QBarCategoryAxis(chart);
    barAxisX->setGridLineVisible(false);

    valueAxisX = new QValueAxis(chart);
    valueAxisX->setGridLineVisible(false);
    valueAxisX->setTickCount(3);
    valueAxisX->setRange(0, 23.2);
    valueAxisX->setLabelFormat("%d");

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

    loadChartData(mode);
}

void BarChart::reloadChart(int mode)
{
    switch (mode) {
    case 1:
        chart->removeSeries(series);
        series->clear();
        set = new QBarSet("BarSet", series);
        series->append(set);
        loadChartData(mode);
        qDebug() << MostPressed::dailyMap;
        break;
    default:
        break;
    }
}

void BarChart::loadChartData(int mode)
{
    switch (mode) {
    case 1: { //daily
        QMap<int, unsigned long int>::const_iterator it;
        for(it = MostPressed::dailyMap.cbegin(); it != MostPressed::dailyMap.cend(); ++it) {
            set->append(it.value());
        }
        chart->addSeries(series);
        chart->setAxisX(valueAxisX, series);
    }
        break;
    case 2: //weekly
        for(int i = 6; i >= 0; --i) {
            barCategories.append(QDate::currentDate().addDays(-i).toString("d"));
            set->append(i + 1);
        }
        chart->addSeries(series);
        barAxisX->append(barCategories);
        chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
        break;
    case 3: { //monthly
        int daysInMonth = QDate::currentDate().daysInMonth();
        for(int i = 0; i < daysInMonth; ++i) {
            lineSeries->append(QDateTime::currentDateTime().addDays(i - daysInMonth + 1).toMSecsSinceEpoch(), 0);
            set->append(i);
        }
        chart->addSeries(series);
        chart->addSeries(lineSeries);
        dateAxisX->setFormat("MMM dd");
        chart->setAxisX(dateAxisX, lineSeries);
    }
        break;
    case 4: //yearly
        for(int i = 0; i < 12; ++i) {
            lineSeries->append(QDateTime::currentDateTime().addMonths(-12 + i + 1).toMSecsSinceEpoch(), 0);
            set->append(i);
            dateAxisX->setTickCount(12);
        }
        chart->addSeries(series);
        chart->addSeries(lineSeries);
        dateAxisX->setFormat("MMM");
        chart->setAxisX(dateAxisX, lineSeries);
        break;
    default:
        break;
    }

    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //it must be after setAcisY()
}
