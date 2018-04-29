#include "statistics.h"
#include <QDebug>
#include <QCloseEvent>
#include <QHBoxLayout>
#include "custombarchart.h"
#include "custompiechart.h"

Statistics::Statistics(QWidget *parent) : QMainWindow(parent)
{
    setLayout();

    setBarChart();
    setPieChart();
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
    pieChartBtn = new QPushButton("Pie Chart");

    QHBoxLayout *btnHLayout = new QHBoxLayout;
    btnHLayout->setSpacing(0);

    btnHLayout->addStretch();
    btnHLayout->addWidget(barChartBtn);
    btnHLayout->addWidget(pieChartBtn);
    btnHLayout->addStretch();

    mainVLayout->addLayout(btnHLayout);

    connect(barTabWidget, &QTabWidget::currentChanged, this, &Statistics::resizeBarChartWindow);
    connect(barChartBtn, &QPushButton::clicked, [this](){ barTabWidget->show(); pieTabWidget->hide(); this->resize(800, 400); });
    connect(pieChartBtn, &QPushButton::clicked, [this](){ barTabWidget->hide(); pieTabWidget->show(); this->resize(500, 500); });
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

void Statistics::setPieChart() const
{
    CustomPieChart *dailyPieChart = new CustomPieChart(0);
    CustomPieChart *weeklyPieChart = new CustomPieChart(1);
    CustomPieChart *monthlyPieChart = new CustomPieChart(2);
    CustomPieChart *yearlyPieChart = new CustomPieChart(3);

    pieTabWidget->addTab(dailyPieChart->pieChartWidget, "Day");
    pieTabWidget->addTab(weeklyPieChart->pieChartWidget, "Week");
    pieTabWidget->addTab(monthlyPieChart->pieChartWidget, "Month");
    pieTabWidget->addTab(yearlyPieChart->pieChartWidget, "Year");
}

void Statistics::resizeBarChartWindow(int index)
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
