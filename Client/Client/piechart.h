#ifndef PIECHART_H
#define PIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChartView>

class Callout;

QT_CHARTS_USE_NAMESPACE

class PieChart : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PieChart(QWidget *parent = nullptr, int mode = 1, QString title = "Key Pressed");

    void reloadChart();

private:
    QPieSeries *series;
    QVector<std::pair<QString, unsigned long int>> mostPressedVec;
    QVector<QColor> colorVec;
    Callout *m_tooltip;
    QChart *chart;

    void reloadChartData();

private slots:
    void keyPressed(QString);
    void hovered(bool);
};

#endif // PIECHART_H
