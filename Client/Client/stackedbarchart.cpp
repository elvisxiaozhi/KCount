#include "stackedbarchart.h"
#include <QVBoxLayout>
#include <QDebug>
#include "database.h"
#include "callout.h"
#include <QDate>

QMap<int, std::pair<int, int> > StackedBarChart::dailyMap = {};
QMap<int, std::pair<int, int> > StackedBarChart::weeklyMap = {};
QMap<int, std::pair<int, int> > StackedBarChart::monthlyMap = {};
QMap<int, std::pair<int, int> > StackedBarChart::yearlyMap = {};

StackedBarChart::StackedBarChart(QWidget *parent, int mode)
    : QGraphicsView(new QGraphicsScene, parent),
      chart(0),
      m_tooltip(0)
{
    switch (mode) {
    case 1:
        dailyMap = Database::returnStackedBarChartData(mode);
        break;
    case 2:
        weeklyMap = Database::returnStackedBarChartData(mode);
        break;
    case 3:
        monthlyMap = Database::returnStackedBarChartData(mode);
        break;
    case 4:
        yearlyMap = Database::returnStackedBarChartData(mode);
        break;
    default:
        break;
    }

    chart = new QChart();
    chart->setTitle("Mouse Clicked Bar Chart");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueIcy);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    label = new QLabel(this);
    label->setText("No Data Recoreded");
    label->setAlignment(Qt::AlignCenter);
    label->hide();

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    mainVLayout->addWidget(label);

    series = new QStackedBarSeries(chart);

    leftSet = new QBarSet("LeftSet", series);
    rightSet = new QBarSet("RightSet", series);

    series->append(leftSet);
    series->append(rightSet);

    valueAxisX = new QValueAxis(chart);
    valueAxisX->setGridLineVisible(false);
    valueAxisX->setTickCount(3);
    valueAxisX->setRange(0, 23.2);
    valueAxisX->setLabelFormat("%d");

    axisY = new QValueAxis(chart);
    axisY->setGridLineVisible(false);
    axisY->setLabelFormat("%d");

    barAxisX = new QBarCategoryAxis(chart);
    barAxisX->setGridLineVisible(false);

    dateAxisX = new QDateTimeAxis(chart);
    dateAxisX->setGridLineVisible(false);

    lineSeries = new QLineSeries(chart);
    lineSeries->hide();

    hoverItem.setBrush(QBrush(QColor(255, 192, 203)));
    hoverItem.setPen(Qt::NoPen);

    setFixedSize(600, 300);
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setContentsMargins(0, 0, 0, 0);
    setFrameStyle(QFrame::NoFrame);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);
    setLayout(mainVLayout);
}

void StackedBarChart::reloadChart(int mode)
{
    switch (mode) {
    case 1:
        reloadChart(dailyMap, mode);
        break;
    case 2:
        reloadChart(weeklyMap, mode);
        break;
    case 3:
        reloadChart(monthlyMap, mode);
        break;
    case 4:
        reloadChart(yearlyMap, mode);
        break;
    default:
        break;
    }
}

void StackedBarChart::loadChartData(int mode)
{
    switch (mode) {
    case 1:
        for(auto it = dailyMap.cbegin(); it != dailyMap.cend(); ++it) {
            leftSet->append(it.value().first);
            rightSet->append(it.value().second);
        }

        chart->addSeries(series);
        chart->setAxisX(valueAxisX, series);
        break;
    case 2: { //weekly
       for(auto it = weeklyMap.cbegin(); it != weeklyMap.cend(); ++it) {
           barCategories.append(QDate::currentDate().addDays(std::distance(weeklyMap.cbegin(), it) - 6).toString("d"));
           leftSet->append(it.value().first);
           rightSet->append(it.value().second);
       }

       chart->addSeries(series);
       barAxisX->append(barCategories);
       chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
    }
        break;
    case 3: { //monthly
        for(auto it = monthlyMap.cbegin(); it != monthlyMap.cend(); ++it) {
            lineSeries->append(QDateTime::currentDateTime().addDays(std::distance(monthlyMap.cbegin(), it) - monthlyMap.size() + 1).toMSecsSinceEpoch(), 0);
            leftSet->append(it.value().first);
            rightSet->append(it.value().second);
        }
        chart->addSeries(series);
        chart->addSeries(lineSeries);
        dateAxisX->setFormat("MMM dd");
        chart->setAxisX(dateAxisX, lineSeries);
    }
        break;
    case 4: //yearly
        for(auto it = yearlyMap.cbegin(); it != yearlyMap.cend(); ++it) {
            barCategories.append(QDate::currentDate().addMonths(-11 + std::distance(yearlyMap.cbegin(), it)).toString("MMM"));
            leftSet->append(it.value().first);
            rightSet->append(it.value().second);
        }
        chart->addSeries(series);
        barAxisX->append(barCategories);
        chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
        break;
    default:
        break;
    }

    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //it must be after setAcisY()

    connect(leftSet, &QBarSet::hovered, this, &StackedBarChart::changeBarColor);
    connect(rightSet, &QBarSet::hovered, this, &StackedBarChart::changeBarColor);
}

void StackedBarChart::reloadChart(QMap<int, std::pair<int, int> > &map, int mode)
{
    for(auto it = map.cbegin(); it != map.cend(); ++it) {
        if(it.value().first > 0 || it.value().second > 0) {
            chart->show();
            label->hide();

            if(chart->series().size() == 0) {
                loadChartData(mode);
            }
            else {
                //delete axisY is for daily and yearly charts
                delete axisY;
                axisY = new QValueAxis(chart);
                axisY->setGridLineVisible(false);
                axisY->setLabelFormat("%d");

                chart->removeSeries(series);
                series->clear();

                leftSet = new QBarSet("LeftSet", series);
                rightSet = new QBarSet("RightSet", series);
                series->append(leftSet);
                series->append(rightSet);

                barCategories.clear(); //only for weekly chart

                //lineSeries for monthly and yearly charts
                delete lineSeries;
                lineSeries = new QLineSeries(chart);

                loadChartData(mode);
            }

            break;
        }
        if(it == map.cend() - 1) {
            chart->hide();
            label->show();
        }
    }
}

void StackedBarChart::changeBarColor(bool status, int index)
{
    if (m_tooltip == 0) {
        m_tooltip = new Callout(chart);
        m_tooltip->setPosition(QPoint(10, -200));
    }

    if(status) {
        QPoint p = chartView->mapFromGlobal(QCursor::pos());
        QGraphicsItem *it = chartView->itemAt(p);
        hoverItem.setParentItem(it);
        hoverItem.setRect(it->boundingRect());
        hoverItem.show();

        QBarSet *barSender = qobject_cast<QBarSet *>(sender());
        m_tooltip->setText(QString::number(barSender->at(index)));
        QPointF point(-1, 5);
        m_tooltip->setAnchor(point);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    }
    else {
        hoverItem.setParentItem(nullptr);
        hoverItem.hide();

        m_tooltip->hide();
    }
}
