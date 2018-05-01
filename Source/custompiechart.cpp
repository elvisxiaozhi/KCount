#include "custompiechart.h"
#include "database.h"
#include <QDate>
#include <QVBoxLayout>
#include <QDebug>
#include <QMovie>

CustomPieChart::CustomPieChart()
{
    series = new QPieSeries();

    QChart *chart = new QChart();
    chart->setTitle("Frequently Pressed Keys");
    chart->addSeries(series);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    pieChartWidget = new QWidget;
    QVBoxLayout *pieVLayout = new QVBoxLayout;
    pieChartWidget->setLayout(pieVLayout);
    pieVLayout->addWidget(chartView);
    chartView->hide();

    loadingLbl = new QLabel;
    pieVLayout->addWidget(loadingLbl);
    loadingMovie = new QMovie(":/Icons/Icons/loading.gif");

    showLoadingPage();
}

void CustomPieChart::showLoadingPage()
{
    loadingLbl->setMovie(loadingMovie);
    loadingMovie->start();
}

void CustomPieChart::updateBarChartData(QMap<QString, int> frequentlyPressedKeyMap)
{
    series->clear();

    QMap<QString, int>::iterator it;
    for(it = frequentlyPressedKeyMap.begin(); it != frequentlyPressedKeyMap.end(); it++) {
        series->append(it.key(), it.value());
    }

    QVector<QPieSlice *> sliceVec;
    for(int i = 0; i < frequentlyPressedKeyMap.size(); i++) { //note the i < frequentlyPressedKeyMap.size(); is used to prevent program out of index crashes //because the slice can not be more than frequentlyPressedKeyMap.size()
        QPieSlice *slice = series->slices().at(i);
        slice->setLabelVisible();
        slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        sliceVec.push_back(slice);
    }

    chartView->show();
    loadingLbl->hide();
}
