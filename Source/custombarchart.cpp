#include "custombarchart.h"
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QVBoxLayout>
#include <QDate>
#include "database.h"

CustomBarChart::CustomBarChart(QString title, int choice)
{
    barSet = new QBarSet("Bar Set");

    switch (choice) {
    case 0: //daily
        for(int i = 0; i < 24; i++) {
            barCategories.push_back(QString::number(i));
            QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# AND CreatedHour = %2").arg(QDate::currentDate().toString("MM/dd/yy")).arg(i);
            barSet->append(DataBase::returnTotalPressedTimes(queryStr)); //must appened data first
        }
        break;
    case 1: //weekly
        for(int i = 6; i >= 0; i--) {
            barCategories.push_back(QDate::currentDate().addDays(-i).toString("dd"));
            QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1#").arg(QDate::currentDate().addDays(-i).toString("MM/dd/yy"));
            barSet->append(DataBase::returnTotalPressedTimes(queryStr)); //must appened data first
        }
        break;
    case 2: //monthly
        for(int i = 0; i < QDate::currentDate().daysInMonth(); i++) {
            barCategories.push_front(QString::number(QDate::currentDate().addDays(-i).toString("dd").toInt()));
            QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1#").arg(QDate::currentDate().addDays(i - QDate::currentDate().daysInMonth() + 1).toString("MM/dd/yy"));
            barSet->append(DataBase::returnTotalPressedTimes(queryStr)); //must appened data first
        }
        break;
    case 3: //yearly
        for(int i = 0; i < 12; i++) {
            barCategories.push_front(QString::number(QDate::currentDate().addMonths(-i).toString("MM").toInt()));
            QString queryStr = QString("SELECT SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2#").arg(QDate::currentDate().addMonths(i - 11).toString("MM/dd/yy")).arg(QDate::currentDate().addMonths(i - 11 - 1).toString("MM/dd/yy"));
            barSet->append(DataBase::returnTotalPressedTimes(queryStr)); //must appened data first
        }
        break;
    default:
        break;
    }

    QBarSeries *barSeries = new QBarSeries();
    barSeries->append(barSet); //then barset and its data to bar seriers

    this->addSeries(barSeries);
    this->setTitle(title);
    this->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(barCategories);

    this->createDefaultAxes();
    this->setAxisX(axis, barSeries);
    this->legend()->setVisible(false); //hide the barset
    this->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    barChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(barChartWidget);
    barChartWidget->setLayout(chartVLayout);
    chartVLayout->addWidget(chartView);
}
