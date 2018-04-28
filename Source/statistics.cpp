#include "statistics.h"
#include <QDebug>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "database.h"
#include <QDate>

Statistics::Statistics(QWidget *parent) : QMainWindow(parent)
{
    setLayout();

    setBarChart();
    setDailyPieChart();
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
    QString currentDate = QDate::currentDate().toString("MM/dd/yy");
    QString readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# GROUP BY PressedKey ORDER BY SUM(PressedTimes) DESC").arg(currentDate);
    QMap<QString, int> frequentlyPressedKeyMap = DataBase::returnFrequentlyPressedKeyMap(readQueryStr);
    qDebug() << frequentlyPressedKeyMap;

    QPieSeries *series = new QPieSeries();

    QMap<QString, int>::iterator it;
    for(it = frequentlyPressedKeyMap.begin(); it != frequentlyPressedKeyMap.end(); it++) {
        series->append(it.key(), it.value());
    }

    QVector<QPieSlice *> slices;
    slices.resize(5);
    for(int i = 0; i < slices.size(); i++) {
        slices[i] = series->slices().at(i);
        slices[i]->setLabelVisible();
        slices[i]->setLabelPosition(QPieSlice::LabelInsideHorizontal);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Pie Chart");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    dailyPieChartWidget = new QWidget;
    QVBoxLayout *pieVLayout = new QVBoxLayout(dailyPieChartWidget);
    dailyPieChartWidget->setLayout(pieVLayout);
    pieVLayout->addWidget(chartView);
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
    barTabWidget->show();
    pieTabWidget->hide();
}

void Statistics::showPieChart()
{
    barTabWidget->hide();
    pieTabWidget->show();
}
