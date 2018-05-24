#include "barchart.h"
#include <QVBoxLayout>
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

    chartView = new QChartView(chart);
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

    loadChartData(mode);
}

void BarChart::reloadChart(int mode)
{
    switch (mode) {
    case 1:
        if(chart->series().size() == 0) {
            loadChartData(1);
        }
        else {
            chart->removeSeries(series);
            series->clear();
            set = new QBarSet("BarSet", series);
            series->append(set);
            loadChartData(1);
        }

//        chart->removeSeries(series);
//        series->clear();
//        set = new QBarSet("BarSet", series);
//        series->append(set);
//        loadChartData(mode);
        break;
    case 2:
        chart->removeSeries(series);
        series->clear();
        barCategories.clear();
        set = new QBarSet("BarSet", series);
        series->append(set);
        loadChartData(mode);
        break;
    case 3:
        chart->removeSeries(series);
        series->clear();
        lineSeries->clear();
        set = new QBarSet("BarSet", series);
        lineSeries = new QLineSeries(chart);
        series->append(set);
        loadChartData(mode);
        break;
    case 4:
        chart->removeSeries(series);
        series->clear();
        lineSeries->clear();
        set = new QBarSet("BarSet", series);
        lineSeries = new QLineSeries(chart);
        series->append(set);
        loadChartData(mode);
        break;
    default:
        break;
    }
}

void BarChart::loadChartData(int mode)
{
    qDebug() << MostPressed::dailyMap;
    switch (mode) {
    case 1: { //daily
        for(auto it = MostPressed::dailyMap.cbegin(); it != MostPressed::dailyMap.cend(); ++it) {
            if(it.value() > 0) {
                for(auto it = MostPressed::dailyMap.cbegin(); it != MostPressed::dailyMap.cend(); ++it) {
                    set->append(it.value());
                }

                chart->addSeries(series);
                chart->setAxisX(valueAxisX, series);

                label->hide();
                chartView->show();
                chart->show();

                chart->setAxisY(axisY, series);
                axisY->applyNiceNumbers(); //it must be after setAcisY()

                break;
            }
            if(it == MostPressed::dailyMap.cend() - 1) {
                chart->hide();
                label->show();
                chartView->hide();
                qDebug() << "End";
            }
        }
//        for(auto it = MostPressed::dailyMap.cbegin(); it != MostPressed::dailyMap.cend(); ++it) {
//            set->append(it.value());
//        }
//        chart->addSeries(series);
//        chart->setAxisX(valueAxisX, series);
    }
        break;
    case 2: { //weekly
        for(auto it = MostPressed::weeklyMap.cbegin(); it != MostPressed::weeklyMap.cend(); ++it) {
            barCategories.append(QDate::currentDate().addDays(std::distance(MostPressed::weeklyMap.cbegin(), it) - 6).toString("d"));
            set->append(it.value());
        }
        chart->addSeries(series);
        barAxisX->append(barCategories);
        chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
    }
        break;
    case 3: { //monthly
        for(auto it = MostPressed::monthlyMap.cbegin(); it != MostPressed::monthlyMap.cend(); ++it) {
            lineSeries->append(QDateTime::currentDateTime().addDays(std::distance(MostPressed::monthlyMap.cbegin(), it) - MostPressed::monthlyMap.size() + 1).toMSecsSinceEpoch(), 0);
            set->append(it.value());
        }
        chart->addSeries(series);
        chart->addSeries(lineSeries);
        dateAxisX->setFormat("MMM dd");
        chart->setAxisX(dateAxisX, lineSeries);
    }
        break;
    case 4: //yearly
        for(auto it = MostPressed::yearlyMap.cbegin(); it != MostPressed::yearlyMap.cend(); ++it) {
            lineSeries->append(QDateTime::currentDateTime().addMonths(-12 + std::distance(MostPressed::yearlyMap.cbegin(), it) + 1).toMSecsSinceEpoch(), 0);
//            set->append(it.value());
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

//    chart->setAxisY(axisY, series);
//    axisY->applyNiceNumbers(); //it must be after setAcisY()
}

void BarChart::testing(bool, int index)
{
    qDebug() << index;
}
