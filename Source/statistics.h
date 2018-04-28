#ifndef STATISTICS_H
#define STATISTICS_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>

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
    QPushButton *barChartBtn, *pieChartBtn;
    void setLayout();
    void closeEvent(QCloseEvent *);
    void setBarChart() const;
    void setPieChart() const;

signals:

public slots:

private slots:
    void resizeWindow(int);
    void showBarChart();
    void showPieChart();
};

#endif // STATISTICS_H
