#ifndef PIECHART_H
#define PIECHART_H

#include <QWidget>
#include <QtCharts/QPieSeries>

QT_CHARTS_USE_NAMESPACE

class PieChart : public QWidget
{
    Q_OBJECT
public:
    explicit PieChart(QWidget *parent = nullptr, int mode = 1);

    void reloadChart(int);

private:
    QPieSeries *series;
    QVector<std::pair<QString, unsigned long int>> dailyVec;
    QVector<std::pair<QString, unsigned long int>> weeklyVec;
    QVector<std::pair<QString, unsigned long int>> monthlyVec;
    QVector<std::pair<QString, unsigned long int>> yearlyVec;

    void reloadChartData(QVector<std::pair<QString, unsigned long int>> &);

private slots:
    void keyPressed(QString);
};

#endif // PIECHART_H
