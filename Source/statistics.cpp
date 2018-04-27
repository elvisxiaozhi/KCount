#include "statistics.h"
#include <QDebug>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

Statistics::Statistics(QWidget *parent) : QMainWindow(parent)
{
    setLayout();

    setBarChart();
//    setDailyPieChart();
//    pieTabWidget->addTab(dailyPieChartWidget, "Day");
}

void Statistics::setLayout()
{
    setWindowTitle("Statistics");

    mainWidget = new QWidget(this);
    this->setCentralWidget(mainWidget);

    mainVLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainVLayout);

    barTabWidget = new QTabWidget(mainWidget);
    mainVLayout->addWidget(barTabWidget);

    pieTabWidget = new QTabWidget(mainWidget);
    pieTabWidget->hide();
    mainVLayout->addWidget(pieTabWidget);

    barChartBtn = new QPushButton("Bar Chart");
//    barChartBtn->setFlat(true);
    pieChartBtn = new QPushButton("Pie Chart");

    QHBoxLayout *btnHLayout = new QHBoxLayout;
    btnHLayout->setSpacing(0);

    btnHLayout->addStretch();
    btnHLayout->addWidget(barChartBtn);
    btnHLayout->addWidget(pieChartBtn);
    btnHLayout->addStretch();

    mainVLayout->addLayout(btnHLayout);

    connect(barTabWidget, &QTabWidget::currentChanged, this, &Statistics::resizeWindow);
    connect(barChartBtn, &QPushButton::clicked, this, &Statistics::showBarChart);
    connect(pieChartBtn, &QPushButton::clicked, this, &Statistics::showPieChart);
}

void Statistics::closeEvent(QCloseEvent *event)
{
    event->ignore();
    this->hide();
}

void Statistics::setBarChart() const
{
    CustomBarChart *dailyBarChart = new CustomBarChart(QString("Daily Totaly Pressed Times"), 0);
    CustomBarChart *weeklyBarChart = new CustomBarChart(QString("Weekly Totaly Pressed Times"), 1);
    CustomBarChart *monthlyBarChart = new CustomBarChart(QString("Monthly Totaly Pressed Times"), 2);
    CustomBarChart *yearlyBarChart = new CustomBarChart(QString("Yearly Totaly Pressed Times"), 3);

    barTabWidget->addTab(dailyBarChart->barChartWidget, "Day");
    barTabWidget->addTab(weeklyBarChart->barChartWidget, "Week");
    barTabWidget->addTab(monthlyBarChart->barChartWidget, "Month");
    barTabWidget->addTab(yearlyBarChart->barChartWidget, "Year");
}

void Statistics::setDailyPieChart()
{
    QPieSeries *series = new QPieSeries();
    series->append("Jane", 1);
    series->append("Joe", 2);
    series->append("Andy", 3);
    series->append("Barbara", 4);
    series->append("Axel", 5);

    QPieSlice *slice = series->slices().at(1);
    slice->setExploded();
    slice->setLabelVisible();
    slice->setPen(QPen(Qt::darkGreen, 2));
    slice->setBrush(Qt::green);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Simple piechart example");
    chart->legend()->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    dailyPieChartWidget = new QWidget;
    dailyPieChartWidget->hide();
    QVBoxLayout *pieVLayout = new QVBoxLayout(dailyPieChartWidget);
    dailyPieChartWidget->setLayout(pieVLayout);
    pieVLayout->addWidget(chartView);

    this->resize(400, 300);
}

void Statistics::resizeWindow(int index)
{
    if(index == 0) {
        this->resize(800, 400);
    }
    if(index == 1) {
        this->resize(500, 300);
    }
    if(index == 2) {
        this->resize(1000, 500);
    }
    if(index == 3) {
        this->resize(800, 400);
    }
}

void Statistics::showBarChart()
{
    dailyBarChart->barChartWidget->show();
    dailyPieChartWidget->hide();
    barTabWidget->show();
    pieTabWidget->hide();
}

void Statistics::showPieChart()
{
    dailyBarChart->barChartWidget->hide();
    dailyPieChartWidget->show();
    barTabWidget->hide();
    pieTabWidget->show();
}
