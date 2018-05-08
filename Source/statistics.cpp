#include "statistics.h"
#include <QDebug>
#include <QHBoxLayout>
#include <QApplication>
#include <QDesktopWidget>
#include <QStyle>

Statistics::Statistics(QWidget *parent) : QMainWindow(parent)
{
    setLayout();
    setWindowStyleSheet();
    setBarChart();
    setPieChart();
}

void Statistics::setLayout()
{
    setWindowTitle(tr("Statistics"));
    setWindowIcon(QIcon(":/Icons/Icons/bar_chart.png"));

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
    connect(barChartBtn, &QPushButton::clicked, [this](){ barTabWidget->show(); pieTabWidget->hide(); this->resize(800, 400); setWindowIcon(QIcon(":/Icons/Icons/bar_chart.png")); });
    connect(pieChartBtn, &QPushButton::clicked, [this](){ barTabWidget->hide(); pieTabWidget->show(); this->resize(600, 600); setWindowIcon(QIcon(":/Icons/Icons/pie_chart.png")); });
}

void Statistics::setWindowStyleSheet()
{
    setStyleSheet(
                "QMainWindow { background-color: #FAE5D3; font-family: Comic Sans MS; }"
                "QTabBar::tab { background-color: #E8F8F5; margin-right: 5px; min-width: 50px; padding: 10px 20px; font-family: Comic Sans MS; }"
                "QTabBar::tab:selected { background: #007ACC; color: #fff; border-top: 2px solid #F1C40F; border-left: 2px solid #F1C40F; border-right: 2px solid #F1C40F; }"
                "QTabBar::tab:hover { font: bold; background-color: #BB8FCE; }"
                "QTabBar::tab:pressed { background-color: #EC7063 }"
                "QTabWidget::pane { border: 2px solid #F1C40F 2px solid #F1C40F; }"
                ".QPushButton { background-color: #3498DB; font-size: 15px; font-family: Comic Sans MS; color: white; border-radius: 15px; border: 2px solid #FF5A5F; padding: 5px 10px; margin: 5px 2px; }"
                ".QPushButton:hover { background-color: #BB8FCE; font-size: 18px; padding: 3px 5px; }"
                ".QPushButton:pressed { background-color: #EC7063 }"
                );
}

void Statistics::setBarChart()
{   
    for(int i = 0; i < 4; ++i) {
        barChartArr[i] = new CustomBarChart;
    }

    barTabWidget->addTab(barChartArr[0]->barChartWidget, tr("Day"));
    barTabWidget->addTab(barChartArr[1]->barChartWidget, tr("Week"));
    barTabWidget->addTab(barChartArr[2]->barChartWidget, tr("Month"));
    barTabWidget->addTab(barChartArr[3]->barChartWidget, tr("Year"));
}

void Statistics::setPieChart()
{
    for(int i = 0; i < 5; ++i) {
        pieChartArr[i] = new CustomPieChart;
    }

    pieTabWidget->addTab(pieChartArr[0]->pieChartWidget, tr("Hour"));
    pieTabWidget->addTab(pieChartArr[1]->pieChartWidget, tr("Day"));
    pieTabWidget->addTab(pieChartArr[2]->pieChartWidget, tr("Week"));
    pieTabWidget->addTab(pieChartArr[3]->pieChartWidget, tr("Month"));
    pieTabWidget->addTab(pieChartArr[4]->pieChartWidget, tr("Year"));
}

void Statistics::updateBarChart(int index, QVector<int> barChartVec)
{
    barChartArr[index]->updateBarChartData(index, barChartVec);
}

void Statistics::updatePieChart(int index, QMap<QString, int> pieChartMap)
{
    pieChartArr[index]->updateBarChartData(pieChartMap);
}

void Statistics::showBarChartLoadingPage(int index)
{
    barChartArr[index]->showLoadingPage();
}

void Statistics::showPieChartLoadingPage(int index)
{
    pieChartArr[index]->showLoadingPage();
}

void Statistics::resizeBarChartWindow(int index)
{
//    this->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, this->size(), qApp->desktop()->availableGeometry())); //this line is to align window to center

    if(index == 0) {
        this->resize(800, 400);
    }
    if(index == 1) {
        this->resize(500, 300);
    }
    if(index == 2) {
        this->showMaximized();
    }
    if(index == 3) {
        this->resize(800, 400);
    }
}
