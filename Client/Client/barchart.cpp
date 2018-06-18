#include "barchart.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QDateTime>
#include "mostpressed.h"
#include "database.h"
#include "callout.h"

QMap<int, unsigned long int> BarChart::dailyMap = {};
QMap<int, unsigned long int> BarChart::weeklyMap = {};
QMap<int, unsigned long int> BarChart::monthlyMap = {};
QMap<int, unsigned long int> BarChart::yearlyMap = {};

BarChart::BarChart(QWidget *parent, int mode)
    : QGraphicsView(new QGraphicsScene, parent),
      chart(0),
      m_tooltip(0)
{
    chart = new QChart;
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setTheme(QChart::ChartThemeBlueIcy);
    chart->legend()->setVisible(false);
    chart->legend()->setAlignment(Qt::AlignBottom);

    switch (mode) {
    case 1:
        dailyMap = Database::returnBarChartData(mode);
        chart->setTitle("Daily Key Pressed");
        break;
    case 2:
        weeklyMap = Database::returnBarChartData(mode);
        chart->setTitle("Weekly Key Pressed");
        break;
    case 3:
        monthlyMap = Database::returnBarChartData(mode);
        chart->setTitle("Monthly Key Pressed");
        break;
    case 4:
        yearlyMap = Database::returnBarChartData(mode);
        chart->setTitle("Yearly Key Pressed");
        break;
    default:
        break;
    }

    series = new QBarSeries(chart);
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    set = new QBarSet("BarSet", series);
    series->append(set);

    labelBrush.setColor(QColor(255, 192, 203));

    dateAxisX = new QDateTimeAxis(chart);
    dateAxisX->setGridLineVisible(false);

    barAxisX = new QBarCategoryAxis(chart);
    barAxisX->setGridLineVisible(false);

    valueAxisX = new QValueAxis(chart);
    valueAxisX->setGridLineVisible(false);
    valueAxisX->setTickCount(3);
    valueAxisX->setRange(0, 23.2);
    valueAxisX->setLabelFormat("%d");

    axisY = new QValueAxis(chart);
    axisY->setGridLineVisible(false);
    axisY->setLabelFormat("%d");

    lineSeries = new QLineSeries(chart);
    lineSeries->hide();

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    label = new QLabel(this);
    label->setText("No Data Recoreded");
    label->setAlignment(Qt::AlignCenter);
    label->hide();

    hoverItem.setBrush(QBrush(QColor(255, 192, 203)));
    hoverItem.setPen(Qt::NoPen);

    QVBoxLayout *mainVLayout = new QVBoxLayout(this);
    mainVLayout->addWidget(chartView);
    mainVLayout->addWidget(label);

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

void BarChart::reloadChart(int mode)
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

void BarChart::loadChartData(int mode)
{
    switch (mode) {
    case 1: //daily
        for(auto it = dailyMap.cbegin(); it != dailyMap.cend(); ++it) {
            set->append(it.value());
        }
        chart->addSeries(series);
        chart->setAxisX(valueAxisX, series);
        break;
    case 2: { //weekly
        for(auto it = weeklyMap.cbegin(); it != weeklyMap.cend(); ++it) {
            barCategories.append(QDate::currentDate().addDays(std::distance(weeklyMap.cbegin(), it) - 6).toString("d"));
            set->append(it.value());
        }
        chart->addSeries(series);
        barAxisX->append(barCategories);
        chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
    }
        break;
    case 3: { //monthly
        for(auto it = monthlyMap.cbegin(); it != monthlyMap.cend(); ++it) {
            lineSeries->append(QDateTime::currentDateTime().addDays(std::distance(monthlyMap.cbegin(), it) - monthlyMap.size() + 1).toMSecsSinceEpoch(), 0);
            set->append(it.value());
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
            set->append(it.value());
        }
        chart->addSeries(series);
        barAxisX->append(barCategories);
        chart->setAxisX(barAxisX, series); //previously attached to the series are deleted
        break;
    default:
        break;
    }

    set->setLabelBrush(labelBrush);
    chart->setAxisY(axisY, series);
    axisY->applyNiceNumbers(); //it must be after setAcisY()
    connect(set, &QBarSet::hovered, this, &BarChart::changeBarColor); //only connect once, or there will be QGraphicsItem::setParentItem issue
}

void BarChart::reloadChart(QMap<int, unsigned long int> &map, int mode)
{
    for(auto it = map.cbegin(); it != map.cend(); ++it) {
        if(it.value() > 0) {
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

                set = new QBarSet("BarSet", series);
                series->append(set);

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

void BarChart::changeBarColor(bool status, int index)
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
