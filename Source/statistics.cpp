#include "statistics.h"
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QDate>
#include <QDebug>
#include <QCloseEvent>
#include "database.h"

QT_CHARTS_USE_NAMESPACE

Statistics::Statistics(QWidget *parent) : QMainWindow(parent)
{
    setLayout();
    setBarChart();
}

void Statistics::setBarChart()
{
    QBarSet *totalPressedTimesSet = new QBarSet("Total Pressed Times");

    QStringList barCategories;
    for(int i = 6; i >= 0; i--) {
        barCategories.push_back(QDate::currentDate().addDays(-i).toString("dd"));
        totalPressedTimesSet->append(DataBase::readTotalPressedTimesInADay(QDate::currentDate().addDays(-i).toString("MM/dd/yy"))); //must appened data first
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(totalPressedTimesSet); //then barset and its data to bar seriers

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries); //then add bar seriers to bar chart
    barChart->setTitle("Weekly Totaly Pressed Times");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(barCategories);

    barChart->createDefaultAxes();
    barChart->setAxisX(axis, barSeries);

    barChart->legend()->setVisible(false); //hide the barset
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(barChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QWidget *chartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(chartWidget);
    chartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);

    tabWidget->addTab(chartWidget, "Week");

    this->resize(420, 300);
}

void Statistics::setLayout()
{
    setWindowTitle("Statistics");

    mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainVLayout);

    tabWidget = new QTabWidget(mainWidget);
    mainVLayout->addWidget(tabWidget);
}

void Statistics::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}
