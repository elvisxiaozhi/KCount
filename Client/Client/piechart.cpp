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
    switch (mode) {
    case 1:
        dailyVec = Database::returnKeyVec(1);
        break;
    case 2:
        weeklyVec = Database::returnKeyVec(2);
        break;
    case 3:
        monthlyVec = Database::returnKeyVec(3);
        break;
    case 4:
        yearlyVec = Database::returnKeyVec(4);
        break;
    default:
        break;
    }

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

void PieChart::reloadChart(int mode)
{
    series->clear();

    switch (mode) {
    case 1:
        qDebug() << "D";
        reloadChartData(dailyVec);
        break;
    case 2:
        qDebug() << "W";
        reloadChartData(weeklyVec);
        break;
    case 3:
        qDebug() << "M";
        reloadChartData(monthlyVec);
        break;
    case 4:
        qDebug() << "Y";
        reloadChartData(yearlyVec);
        break;
    default:
        break;
    }
}

void PieChart::reloadChartData(QVector<std::pair<QString, unsigned long> > &vec)
{
    std::sort(vec.begin(), vec.end(),
              [](const std::pair<QString, unsigned long int> &a, const std::pair<QString, unsigned long int> &b){ return a.second > b.second; });

    if(vec.size() >= 5) {
        for(int i = 0; i < 5; ++i) {
            series->append(vec[i].first, vec[i].second);
            QPieSlice *slice = series->slices().at(i);
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        }
    }
    else {
        for(int i = 0; i < vec.size(); ++i) {
            series->append(vec[i].first, vec[i].second);
            QPieSlice *slice = series->slices().at(i);
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
        }
    }
}

void PieChart::keyPressed(QString pressedKey)
{
    auto dailyIt = std::find_if(dailyVec.begin(), dailyVec.end(),
        [pressedKey](const std::pair<QString, unsigned long int> &element){ return element.first == pressedKey; });
    if(dailyIt != dailyVec.end()) {
        ++(*dailyIt).second;
    }
    else {
        dailyVec.push_back(std::make_pair(pressedKey, 1));
    }

    auto weeklyIt = std::find_if(weeklyVec.begin(), weeklyVec.end(),
        [pressedKey](const std::pair<QString, unsigned long int> &element){ return element.first == pressedKey; });
    if(weeklyIt != weeklyVec.end()) {
        ++(*weeklyIt).second;
    }
    else {
        weeklyVec.push_back(std::make_pair(pressedKey, 1));
    }

    auto monthlyIt = std::find_if(monthlyVec.begin(), monthlyVec.end(),
        [pressedKey](const std::pair<QString, unsigned long int> &element){ return element.first == pressedKey; });
    if(monthlyIt != monthlyVec.end()) {
        ++(*monthlyIt).second;
    }
    else {
        monthlyVec.push_back(std::make_pair(pressedKey, 1));
    }

    auto yearlyIt = std::find_if(yearlyVec.begin(), yearlyVec.end(),
        [pressedKey](const std::pair<QString, unsigned long int> &element){ return element.first == pressedKey; });
    if(yearlyIt != yearlyVec.end()) {
        ++(*yearlyIt).second;
    }
    else {
        yearlyVec.push_back(std::make_pair(pressedKey, 1));
    }
}
