#include "piechart.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSlice>
#include <QVBoxLayout>
#include "database.h"
#include <QDebug>
#include "signalemitter.h"

PieChart::PieChart(QWidget *parent, int mode, QString title) : QWidget(parent)
{
    mostPressedVec = Database::returnKeyVec(mode);
    colorVec = { QColor(255,0,0), QColor(255,215,0), QColor(0,255,0), QColor(0,128,128), QColor(255,192,203) };

    series = new QPieSeries();

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setDropShadowEnabled(true);

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
            slice->setBrush(colorVec[i]);

            connect(slice, &QPieSlice::hovered, [slice, this](bool status){
                if(status) {
                    slice->setExploded(true);
                    slice->setLabelPosition(QPieSlice::LabelOutside);
                }
                else {
                    slice->setExploded(false);
                    slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
                }
            });
        }
    }
    else {
        for(int i = 0; i < mostPressedVec.size(); ++i) {
            series->append(mostPressedVec[i].first, mostPressedVec[i].second);
            QPieSlice *slice = series->slices().at(i);
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            slice->setBrush(colorVec[i]);

            connect(slice, &QPieSlice::hovered, [slice, this](bool status){
                if(status) {
                    slice->setExploded(true);
                    slice->setLabelPosition(QPieSlice::LabelOutside);
                }
                else {
                    slice->setExploded(false);
                    slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
                }
            });
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
