#include "custompiechart.h"
#include <QtCharts/QChartView>
#include <QVBoxLayout>
#include "database.h"
#include <QDate>
#include <QDebug>

CustomPieChart::CustomPieChart(int choice)
{
    QString readQueryStr;

    switch (choice) {
    case 0:
        readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate = #%1# GROUP BY PressedKey ORDER BY SUM(PressedTimes) DESC").arg(QDate::currentDate().toString("MM/dd/yy"));
        break;
    case 1:
        readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey ORDER BY SUM(PressedTimes) DESC").arg(QDate::currentDate().toString("MM/dd/yy")).arg(QDate::currentDate().addDays(-7).toString("MM/dd/yy"));
        break;
    case 2:
        readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey ORDER BY SUM(PressedTimes) DESC").arg(QDate::currentDate().toString("MM/dd/yy")).arg(QDate::currentDate().addMonths(-1).toString("MM/dd/yy"));
        break;
    case 3:
        readQueryStr = QString("SELECT PressedKey, SUM(PressedTimes) FROM Data WHERE CreatedDate BETWEEN #%1# AND #%2# GROUP BY PressedKey ORDER BY SUM(PressedTimes) DESC").arg(QDate::currentDate().toString("MM/dd/yy")).arg(QDate::currentDate().addYears(-1).toString("MM/dd/yy"));
        break;
    default:
        break;
    }

    QMap<QString, int> frequentlyPressedKeyMap = DataBase::returnFrequentlyPressedKeyMap(readQueryStr);

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

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    pieChartWidget = new QWidget;
    QVBoxLayout *pieVLayout = new QVBoxLayout;
    pieChartWidget->setLayout(pieVLayout);
    pieVLayout->addWidget(chartView);
}
