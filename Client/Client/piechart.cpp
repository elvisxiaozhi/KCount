#include "piechart.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QVBoxLayout>
#include "database.h"
#include <QDebug>
#include "signalemitter.h"

PieChart::PieChart(QWidget *parent, int mode) : QWidget(parent)
{
    mostPressedVec = Database::returnKeyVec(mode);

    series = new QPieSeries();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(tr("Key Pressed Pie Chart"));
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    setLayout(mainVLayout);

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &PieChart::keyPressed);
}

void PieChart::reloadChart()
{
    series->clear();

    reloadChartData();
}

void PieChart::reloadChartData()
{
    std::sort(mostPressedVec.begin(), mostPressedVec.end(),
              [](const std::pair<QString, unsigned long int> &a, const std::pair<QString, unsigned long int> &b){ return a.second > b.second; });

    if(mostPressedVec.size() >= 5) {
        for(int i = 0; i < 5; ++i) {
            series->append(mostPressedVec[i].first, mostPressedVec[i].second);
            QPieSlice *slice = series->slices().at(i);
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        }
    }
    else {
        for(int i = 0; i < mostPressedVec.size(); ++i) {
            series->append(mostPressedVec[i].first, mostPressedVec[i].second);
            QPieSlice *slice = series->slices().at(i);
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        }
    }
}

void PieChart::keyPressed(QString pressedKey)
{
    auto dailyIt = std::find_if(mostPressedVec.begin(), mostPressedVec.end(),
        [pressedKey](const std::pair<QString, unsigned long int> &element){ return element.first == pressedKey; });
    if(dailyIt != mostPressedVec.end()) {
        ++(*dailyIt).second;
    }
    else {
        mostPressedVec.push_back(std::make_pair(pressedKey, 1));
    }
}
