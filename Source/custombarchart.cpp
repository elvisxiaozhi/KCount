#include "custombarchart.h"
#include <QtCharts/QLegend>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QDebug>

CustomBarChart::CustomBarChart()
{
    barSet = new QBarSet("Bar Set");

    barSeries = new QBarSeries();

    axis = new QBarCategoryAxis();
    axis->setGridLinePen(Qt::NoPen);

    this->setTitle(tr("Total Pressed Times"));
    this->setAnimationOptions(QChart::AllAnimations);
    this->legend()->setVisible(false); //hide the barset
    this->legend()->setAlignment(Qt::AlignBottom);
    this->setTheme(QChart::ChartThemeBlueIcy);

    chartView = new QChartView(this);
    chartView->setRenderHint(QPainter::Antialiasing);

    barChartWidget = new QWidget;
    QVBoxLayout *chartVLayout = new QVBoxLayout(barChartWidget);
    barChartWidget->setLayout(chartVLayout);

    totalLbl = new QLabel;
    averageLbl = new QLabel;
    totalLbl->setStyleSheet("QLabel { font-size: 15px; color: #784212; font-family: Comic Sans MS; }");
    averageLbl->setStyleSheet("QLabel { font-size: 15px; color: #784212; font-family: Comic Sans MS; }");
    QHBoxLayout *lblHLayout = new QHBoxLayout;
    lblHLayout->addWidget(totalLbl);
    lblHLayout->addStretch();
    lblHLayout->addWidget(averageLbl);

    chartVLayout->addLayout(lblHLayout);
    chartVLayout->addWidget(chartView);
    chartView->hide();

    loadingLbl = new QLabel;
    loadingLbl->setAlignment(Qt::AlignCenter);
    chartVLayout->addWidget(loadingLbl);
    loadingMovie = new QMovie(":/Icons/Icons/loading.gif");

    IsFirstRun = true;
}

void CustomBarChart::showLoadingPage()
{
    chartView->hide(); //hide chartview to show loading page fulling

    loadingLbl->setMovie(loadingMovie);
    loadingMovie->start();
    loadingLbl->show();
}

void CustomBarChart::setWindowStyleSheet()
{
    chartView->setStyleSheet("QChartView {background-color: #FAD7A0;}");
}

void CustomBarChart::updateBarChartData(int choice, QVector<int> barChartVec)
{
    if(!IsFirstRun) {
        this->removeSeries(barSeries);
        this->removeAxis(axis);
        barSeries->clear();
        axis->clear();
        barSet = new QBarSet("Bar Set");
        delete axisY;
    }

    switch (choice) {
    case 0: //daily
        for(int i = 0; i < 24; i++) {
            barCategories.push_back(QString::number(i));
            barSet->append(barChartVec[i]);
        }
        break;
    case 1: //weekly
        for(int i = 6; i >= 0; i--) {
            barCategories.push_back(QDate::currentDate().addDays(-i).toString("d"));
            barSet->append(barChartVec[6 - i]);
        }
        break;
    case 2: { //monthly
        int daysInMonth = QDate::currentDate().daysInMonth();
        for(int i = 0; i < daysInMonth; i++) {
            barCategories.push_back(QDate::currentDate().addDays(i - daysInMonth + 1).toString("MMM d"));
            barSet->append(barChartVec[i]);
        }
    }
        break;
    case 3: //yearly
        for(int i = 0; i < 12; i++) {
            barCategories.push_front(QString::number(QDate::currentDate().addMonths(-i).toString("MM").toInt()));
            barSet->append(barChartVec[i]);
        }
        break;
    default:
        break;
    }

    int sumUp = std::accumulate(barChartVec.begin(), barChartVec.end(), 0);
    totalLbl->setText("Total: " + QString::number(sumUp));
    averageLbl->setText("Average: " + QString::number(sumUp / barChartVec.size()));

    barSeries->append(barSet); //then barset and its data to bar seriers
    axis->append(barCategories);

    this->addSeries(barSeries);
    this->createDefaultAxes();
    this->setAxisX(axis, barSeries);

    axisY = new QValueAxis(); //set a new y axis each time when refreashing, or the y axis will not show
    axisY->setGridLinePen(Qt::NoPen);
    axisY->setLabelFormat("%d"); //format to no decimal
    this->setAxisY(axisY, barSeries);
    axisY->applyNiceNumbers(); //it must be after setAcisY()

    chartView->show();
    loadingLbl->hide();

    IsFirstRun = false;
}
