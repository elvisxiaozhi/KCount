#include "custombarchart.h"
#include <QtCharts/QLegend>
#include <QVBoxLayout>
#include <QDate>
#include <QDebug>

CustomBarChart::CustomBarChart()
{
    barSet = new QBarSet("Bar Set");

    barSeries = new QBarSeries();

    this->setTitle("Total Pressed Times");
    this->setAnimationOptions(QChart::SeriesAnimations);

    axis = new QBarCategoryAxis();
    axis->setGridLinePen(Qt::NoPen);

    axisY = new QValueAxis();
    axisY->setLabelFormat("%d"); //format to no decimal
    axisY->setGridLinePen(Qt::NoPen);

    this->legend()->setVisible(false); //hide the barset
    this->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    barChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(barChartWidget);
    barChartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);
    chartView->hide();

    loadingLbl = new QLabel;
    loadingLbl->setAlignment(Qt::AlignCenter);
    chartVLayout->addWidget(loadingLbl);
    loadingMovie = new QMovie(":/Icons/Icons/loading.gif");
}

void CustomBarChart::showLoadingPage()
{
    chartView->hide(); //hide chartview to show loading page fulling

    loadingLbl->setMovie(loadingMovie);
    loadingMovie->start();
    loadingLbl->show();
}

void CustomBarChart::updateBarChartData(int choice, QVector<int> barChartVec)
{
    this->removeSeries(barSeries);
    this->removeAxis(axis);
    barSeries->clear();
    axis->clear();
    barSet = new QBarSet("Bar Set");

    switch (choice) {
    case 0: //daily
        for(int i = 0; i < 24; i++) {
            barCategories.push_back(QString::number(i));
            barSet->append(barChartVec[i]);
        }
        break;
    case 1: //weekly
        for(int i = 6; i >= 0; i--) {
            barCategories.push_back(QDate::currentDate().addDays(-i).toString("d"));
            barSet->append(barChartVec[i]);
        }
        break;
    case 2: { //monthly
        int daysInMonth = QDate::currentDate().daysInMonth();
        for(int i = 0; i < daysInMonth; i++) {
            barCategories.push_back(QDate::currentDate().addDays(i - daysInMonth + 1).toString("MMM d"));
            barSet->append(barChartVec[i]);
        }
    }
        break;
    case 3: //yearly
        for(int i = 0; i < 12; i++) {
            barCategories.push_front(QString::number(QDate::currentDate().addMonths(-i).toString("MM").toInt()));
            barSet->append(barChartVec[i]);
        }
        break;
    default:
        break;
    }

    barSeries->append(barSet); //then barset and its data to bar seriers
    axis->append(barCategories);

    this->addSeries(barSeries);
    this->createDefaultAxes();
    this->setAxisX(axis, barSeries);
    this->setAxisY(axisY, barSeries);

    axisY->applyNiceNumbers(); //it must be after setAcisY()

    chartView->show();
    loadingLbl->hide();
}
