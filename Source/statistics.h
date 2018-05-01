#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include "custombarchart.h"
#include "custompiechart.h"

class Statistics : public QMainWindow
{
    Q_OBJECT
public:
    explicit Statistics(QWidget *parent = nullptr);

private:
    QWidget *mainWidget;
    QWidget *dailyBarChartWidget;
    QVBoxLayout *mainVLayout;
    QTabWidget *barTabWidget, *pieTabWidget;
    CustomBarChart *barChartArr[4];
    CustomPieChart *pieChartArr[5];
    QPushButton *barChartBtn, *pieChartBtn;
    void setLayout();
    void closeEvent(QCloseEvent *);
    void setBarChart();
    void setPieChart();

signals:
    void loadBarChartData(int);
    void loadPieChartData(int);

public slots:
    void updateBarChart(int, QVector<int>);
    void updatePieChart(int, QMap<QString, int>);
    void showBarChartLoadingPage(int);
    void showPieChartLoadingPage(int);

private slots:
    void resizeBarChartWindow(int);
};

#endif // STATISTICS_H
