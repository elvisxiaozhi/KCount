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
#include <QHBoxLayout>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_CHARTS_USE_NAMESPACE

Statistics::Statistics(QWidget *parent) : QMainWindow(parent)
{
    setLayout();

    setDailyBarChart();
    setWeeklyBarChart();
    setMonthlyBarChart();
    setYearlyBarChart();

    setDailyPieChart();

    barTabWidget->addTab(dailyBarChartWidget, "Day");
    barTabWidget->addTab(weeklyBarChartWidget, "Week");
    barTabWidget->addTab(monthlyBarChartWidget, "Month");
    barTabWidget->addTab(yearlyBarChartWidget, "Year");

    pieTabWidget->addTab(dailyPieChartWidget, "Day");
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

void Statistics::setDailyBarChart()
{
    QBarSet *barSet = new QBarSet("Total Pressed Times");

    QStringList barCategories;
    for(int i = 0; i < 24; i++) {
        barCategories.push_back(QString::number(i));
        QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(QDate::currentDate().toString("MM/dd/yy")).arg(i);
        barSet->append(DataBase::readTotalPressedTimesInADay(queryStr)); //must appened data first
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet); //then barset and its data to bar seriers

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries); //then add bar seriers to bar chart
    barChart->setTitle("Daily Totaly Pressed Times");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(barCategories);

    barChart->createDefaultAxes();
    barChart->setAxisX(axis, barSeries);

    barChart->legend()->setVisible(false); //hide the barset
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(barChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    dailyBarChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(dailyBarChartWidget);
    dailyBarChartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);
}

void Statistics::setWeeklyBarChart()
{
    QBarSet *barSet = new QBarSet("Total Pressed Times");

    QStringList barCategories;
    for(int i = 6; i >= 0; i--) {
        barCategories.push_back(QDate::currentDate().addDays(-i).toString("dd"));
        QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1#").arg(QDate::currentDate().addDays(-i).toString("MM/dd/yy"));
        barSet->append(DataBase::readTotalPressedTimesInADay(queryStr)); //must appened data first
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet); //then barset and its data to bar seriers

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries); //then add bar seriers to bar chart
    barChart->setTitle("Weekly Pressed Times");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(barCategories);

    barChart->createDefaultAxes();
    barChart->setAxisX(axis, barSeries);

    barChart->legend()->setVisible(false); //hide the barset
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(barChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    weeklyBarChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(weeklyBarChartWidget);
    weeklyBarChartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);
}

void Statistics::setMonthlyBarChart()
{
    QBarSet *barSet = new QBarSet("Total Pressed Times");

    int daysInMonth = QDate::currentDate().daysInMonth();

    QStringList barCategories;
    for(int i = 0; i < daysInMonth; i++) {
        barCategories.push_front(QString::number(QDate::currentDate().addDays(-i).toString("dd").toInt()));
        QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1#").arg(QDate::currentDate().addDays(i - daysInMonth + 1).toString("MM/dd/yy"));
        barSet->append(DataBase::readTotalPressedTimesInADay(queryStr)); //must appened data first
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet); //then barset and its data to bar seriers

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries); //then add bar seriers to bar chart
    barChart->setTitle("Monthly Pressed Times");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(barCategories);

    barChart->createDefaultAxes();
    barChart->setAxisX(axis, barSeries);

    barChart->legend()->setVisible(false); //hide the barset
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(barChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    monthlyBarChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(monthlyBarChartWidget);
    monthlyBarChartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);
}

void Statistics::setYearlyBarChart()
{
    QBarSet *barSet = new QBarSet("Total Pressed Times");

    QStringList barCategories;
    for(int i = 0; i < 12; i++) {
        barCategories.push_front(QString::number(QDate::currentDate().addMonths(-i).toString("MM").toInt()));
        QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2#").arg(QDate::currentDate().addMonths(i - 11).toString("MM/dd/yy")).arg(QDate::currentDate().addMonths(i - 11 - 1).toString("MM/dd/yy"));
        barSet->append(DataBase::readTotalPressedTimesInADay(queryStr)); //must appened data first
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet); //then barset and its data to bar seriers

    QChart *barChart = new QChart();
    barChart->addSeries(barSeries); //then add bar seriers to bar chart
    barChart->setTitle("Yearly Pressed Times");
    barChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(barCategories);

    barChart->createDefaultAxes();
    barChart->setAxisX(axis, barSeries);

    barChart->legend()->setVisible(false); //hide the barset
    barChart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(barChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    yearlyBarChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(yearlyBarChartWidget);
    yearlyBarChartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);
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
    dailyBarChartWidget->show();
    dailyPieChartWidget->hide();
    barTabWidget->show();
    pieTabWidget->hide();
}

void Statistics::showPieChart()
{
    dailyBarChartWidget->hide();
    dailyPieChartWidget->show();
    barTabWidget->hide();
    pieTabWidget->show();
}
