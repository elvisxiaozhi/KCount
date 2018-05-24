#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QGridLayout>
#include "barchart.h"
#include "stackedbarchart.h"

class Dashboard : public QWidget
{
    Q_OBJECT
public:
    explicit Dashboard(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainVLayout;
    QHBoxLayout *timeSpanHLayout;
    QGridLayout *gLayout;
    QComboBox *timeSpanBox;
    BarChart *barChartArr[4];
    BarChart *mBarChart;
    StackedBarChart *stackedBarChart;

    void setWindowStyleSheet();
    void setWindowLayout();
    void createTimeSpanBox();
    void createCharts();

protected:
    void paintEvent(QPaintEvent *);

signals:
    void loadingData();

private slots:
    void loadData();
    void comboBoxChanged(int);
};

#endif // DASHBOARD_H
