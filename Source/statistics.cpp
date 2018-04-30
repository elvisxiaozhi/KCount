#include "statistics.h"
#include <QDebug>
#include <QCloseEvent>
#include <QHBoxLayout>

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

void Statistics::setBarChart()
{   
    for(int i = 0; i < 4; i++) {
        barChartArr[i] = new CustomBarChart;
    }

    barTabWidget->addTab(barChartArr[0]->barChartWidget, "Day");
    barTabWidget->addTab(barChartArr[1]->barChartWidget, "Week");
    barTabWidget->addTab(barChartArr[2]->barChartWidget, "Month");
    barTabWidget->addTab(barChartArr[3]->barChartWidget, "Year");
}

void Statistics::setPieChart()
{
    for(int i = 0; i < 4; i++) {
        pieChartArr[i] = new CustomPieChart;
    }

    pieTabWidget->addTab(pieChartArr[0]->pieChartWidget, "Day");
    pieTabWidget->addTab(pieChartArr[1]->pieChartWidget, "Week");
    pieTabWidget->addTab(pieChartArr[2]->pieChartWidget, "Month");
    pieTabWidget->addTab(pieChartArr[3]->pieChartWidget, "Year");
}

void Statistics::updateBarChart(int index, QVector<int> barChartVec)
{
    barChartArr[index]->updateBarChartData(index, barChartVec);
}

void Statistics::updatePieChart(int index, QMap<QString, int> pieChartMap)
{
    pieChartArr[index]->updateBarChartData(pieChartMap);
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
