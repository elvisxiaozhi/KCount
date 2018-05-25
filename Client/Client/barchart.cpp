#include "barchart.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QDateTime>
#include "mostpressed.h"
#include "database.h"

QMap<int, unsigned long int> BarChart::dailyMap = {};
QMap<int, unsigned long int> BarChart::weeklyMap = {};
QMap<int, unsigned long int> BarChart::monthlyMap = {};
QMap<int, unsigned long int> BarChart::yearlyMap = {};

BarChart::BarChart(QWidget *parent, int mode) : QWidget(parent)
{
    switch (mode) {
    case 1:
        dailyMap = Database::returnBarChartData(mode);
        break;
    case 2:
        weeklyMap = Database::returnBarChartData(mode);
        break;
    case 3:
        monthlyMap = Database::returnBarChartData(mode);
        break;
    case 4:
        yearlyMap = Database::returnBarChartData(mode);
        break;
    default:
        break;
    }

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

    label = new QLabel(this);
    label->setText("No Data Recoreded");
    label->setAlignment(Qt::AlignCenter);
    label->hide();

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    mainVLayout->addWidget(label);
    setLayout(mainVLayout);

//    connect(set, &QBarSet::hovered, this, &BarChart::testing);
}

void BarChart::reloadChart(int mode)
{   
    switch (mode) {
    case 1:
        reloadChart(dailyMap, mode);
        break;
    case 2:
        reloadChart(weeklyMap, mode);
    default:
        break;
    }
}

void BarChart::loadChartData(int mode)
{
    switch (mode) {
    case 1: //daily
        for(auto it = dailyMap.cbegin(); it != dailyMap.cend(); ++it) {
            set->append(it.value());
        }
        chart->addSeries(series);
        chart->setAxisX(valueAxisX, series);
        break;
    case 2: { //weekly
        for(auto it = weeklyMap.cbegin(); it != weeklyMap.cend(); ++it) {
            barCategories.append(QDate::currentDate().addDays(std::distance(weeklyMap.cbegin(), it) - 6).toString("d"));
            set->append(it.value());
        }
        chart->addSeries(series);
        barAxisX->append(barCategories);
        chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
    }
        break;
    case 3: { //monthly
        for(auto it = monthlyMap.cbegin(); it != monthlyMap.cend(); ++it) {
            lineSeries->append(QDateTime::currentDateTime().addDays(std::distance(monthlyMap.cbegin(), it) - monthlyMap.size() + 1).toMSecsSinceEpoch(), 0);
            set->append(it.value());
        }
        chart->addSeries(series);
        chart->addSeries(lineSeries);
        dateAxisX->setFormat("MMM dd");
        chart->setAxisX(dateAxisX, lineSeries);
    }
        break;
    case 4: //yearly
            for(auto it = yearlyMap.cbegin(); it != yearlyMap.cend(); ++it) {
                lineSeries->append(QDateTime::currentDateTime().addMonths(-12 + std::distance(yearlyMap.cbegin(), it) + 1).toMSecsSinceEpoch(), 0);
                set->append(it.value());
            }
            dateAxisX->setTickCount(12);
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

void BarChart::reloadChart(QMap<int, unsigned long int> &map, int mode)
{
    for(auto it = map.cbegin(); it != map.cend(); ++it) {
        if(it.value() > 0) {
            chart->show();
            label->hide();

            if(chart->series().size() == 0) {
                loadChartData(mode);
            }
            else {
                //delete axisY is for daily and yearly charts
                delete axisY;
                axisY = new QValueAxis(chart);
                axisY->setGridLineVisible(false);
                axisY->setLabelFormat("%d");

                chart->removeSeries(series);
                series->clear();

                set = new QBarSet("BarSet", series);
                series->append(set);

                barCategories.clear(); //only for weekly chart

                //lineSeries for monthly and yearly charts
                lineSeries->clear();
                lineSeries = new QLineSeries(chart);

                loadChartData(mode);
            }

            break;
        }
        if(it == map.cend() - 1) {
            chart->hide();
            label->show();
        }
    }
}

void BarChart::testing(bool, int index)
{
    qDebug() << index;
}
