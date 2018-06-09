#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include "barchart.h"
#include "stackedbarchart.h"
#include "piechart.h"
#include <QScrollArea>
#include "appusagestackedbarchart.h"

class Dashboard : public QWidget
{
    Q_OBJECT
public:
    explicit Dashboard(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QHBoxLayout *timeSpanHLayout;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QGridLayout *gLayout;
    QComboBox *timeSpanBox;
    BarChart *barChartArr[4];
    StackedBarChart *stackedBarChartArr[4];
    PieChart *pieChartArr[5];
    AppUsageStackedBarChart *appUsageChart;

    void setWindowStyleSheet();
    void setWindowLayout();
    void createTimeSpanBox();
    void createCharts();
    bool eventFilter(QObject *watched, QEvent *event);

protected:
    void paintEvent(QPaintEvent *);

signals:
    void loadingData();

private slots:
    void loadData();
    void comboBoxChanged(int);
};

#endif // DASHBOARD_H
