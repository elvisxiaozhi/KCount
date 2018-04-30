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

    this->legend()->setVisible(false); //hide the barset
    this->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    barChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(barChartWidget);
    barChartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);
}

void CustomBarChart::removeOldBarSet()
{
    this->removeSeries(barSeries);
    this->removeAxis(axis);
    barSeries->clear();
    axis->clear();
    barSet = new QBarSet("Bar Set");
}

void CustomBarChart::updateBarChartData(int choice, QVector<int> barChartVec)
{
    switch (choice) {
    case 0: //daily
        for(int i = 0; i < 24; i++) {
            barCategories.push_back(QString::number(i));
            barSet->append(barChartVec[i]);
        }
        break;
    case 1: //weekly
        for(int i = 6; i >= 0; i--) {
            barCategories.push_back(QDate::currentDate().addDays(-i).toString("dd"));
            barSet->append(barChartVec[i]);
        }
        break;
    case 2: //monthly
        for(int i = 0; i < QDate::currentDate().daysInMonth(); i++) {
            barCategories.push_front(QString::number(QDate::currentDate().addDays(-i).toString("dd").toInt()));
            barSet->append(barChartVec[i]);
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

    qDebug() << "Updated";
}
