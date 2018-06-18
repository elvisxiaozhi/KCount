#ifndef PIECHART_H
#define PIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>

QT_CHARTS_USE_NAMESPACE

class PieChart : public QWidget
{
    Q_OBJECT
public:
    explicit PieChart(QWidget *parent = nullptr, int mode = 1, QString title = "Key Pressed");

    void reloadChart();

private:
    QPieSeries *series;
    QVector<std::pair<QString, unsigned long int>> mostPressedVec;
    QVector<QColor> colorVec;

    void reloadChartData();

private slots:
    void keyPressed(QString);
};

#endif // PIECHART_H
