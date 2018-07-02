#include "piechart.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QPieSlice>
#include <QVBoxLayout>
#include "database.h"
#include <QDebug>
#include "signalemitter.h"
#include "callout.h"

PieChart::PieChart(QWidget *parent, int mode, QString title)
    : QGraphicsView(new QGraphicsScene, parent),
      m_tooltip(0)
{
    mostPressedVec = Database::returnKeyVec(mode);
    colorVec = { QColor(255,0,0), QColor(255,215,0), QColor(0,255,0), QColor(0,128,128), QColor(255,192,203) };

    series = new QPieSeries();

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setDropShadowEnabled(true);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);

    setFixedSize(600, 300);
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContentsMargins(0, 0, 0, 0);
    setFrameStyle(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);
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

            connect(slice, &QPieSlice::hovered, this, &PieChart::hovered);
        }
    }
    else {
        for(int i = 0; i < mostPressedVec.size(); ++i) {
            series->append(mostPressedVec[i].first, mostPressedVec[i].second);
            QPieSlice *slice = series->slices().at(i);
            slice->setLabelVisible();
            slice->setLabelPosition(QPieSlice::LabelInsideHorizontal);
            slice->setBrush(colorVec[i]);

            connect(slice, &QPieSlice::hovered, this, &PieChart::hovered);
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

void PieChart::hovered(bool status)
{
    if (m_tooltip == 0) {
        m_tooltip = new Callout(chart);
        m_tooltip->setPosition(QPoint(40, 230));
    }

    QPieSlice *sliceSender = qobject_cast<QPieSlice *>(sender());
    if(status) {
        sliceSender->setExploded(true);

        m_tooltip->setText(sliceSender->label() + QString(": %1; %2%").arg(QString::number(sliceSender->value())).arg(100 * sliceSender->percentage(), 0, 'f', 1));
        m_tooltip->setAnchor(QPointF(10, 7));
        m_tooltip->updateGeometry();
        m_tooltip->show();
    }
    else {
        sliceSender->setExploded(false);

        m_tooltip->hide();
    }
}
