#include "dashboard.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QLabel>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent)
{
    setWindowStyleSheet();
    setWindowLayout();
    createTimeSpanBox();
    createCharts();

    connect(this, &Dashboard::loadingData, this, &Dashboard::loadData);
//    connect(timeSpanBox, QOverload<int>::of(&QComboBox::activated),
//            [=](int index){
//        comboBoxChanged(index);
//    });
}

void Dashboard::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #eeeeee; }"
                "QComboBox { border: 1px solid gray; border-radius: 8px; padding: 8px 8px 8px 8px; min-width: 3em; background: #faebd7; }"
                "QComboBox:editable { background: #ffe4e1; }" //set arrow background color
                "QComboBox:on { background: #fa8072; }" //change the combo box bgcolor when opened
                "QComboBox::drop-down {"
                "border-left-width: 1px; border-left-color: darkgray; border-left-style: solid;/* just a single line */"
                "border-top-right-radius: 3px; border-bottom-right-radius: 3px; /* same radius as the QComboBox */ "
                "}" //change the shape of drop down menu
                "QComboBox QAbstractItemView { border: 2px solid darkgray; selection-background-color: #FFC0CB; }" //change the selection bgcolor
                "QComboBox::down-arrow { image: url(:/Resources/Icons/down-arrow.png); }"
                "QComboBox::down-arrow:on { image: url(:/Resources/Icons/up-arrow.png); }"
                "QLabel#SpanTextLbl { background: #D3FFCE; padding: 8px 8px 8px 8px; border: 1px solid gray; border-radius: 8px; }"
                );
}

void Dashboard::setWindowLayout()
{
    mainVLayout = new QVBoxLayout(this);
    timeSpanHLayout = new QHBoxLayout; //do not set parent
    gLayout = new QGridLayout;

    mainVLayout->addLayout(timeSpanHLayout);
    mainVLayout->addLayout(gLayout);
    this->setLayout(mainVLayout);
}

void Dashboard::createTimeSpanBox()
{
    QLabel *spanTextLbl = new QLabel(this);
    spanTextLbl->setText("Time Span: ");
    spanTextLbl->setObjectName("SpanTextLbl");

    timeSpanBox = new QComboBox(this);
    timeSpanBox->installEventFilter(this);
    timeSpanBox->addItems({tr("Hour"), tr("Day"), tr("Week"), tr("Month"), tr("Year")});
    timeSpanBox->setCurrentText("Day");

    timeSpanHLayout->addStretch();
    timeSpanHLayout->addWidget(spanTextLbl);
    timeSpanHLayout->addWidget(timeSpanBox);
}

void Dashboard::createCharts()
{
//    for(int i = 1; i <= 4; ++i) {
//        barChartArr[i] = new BarChart(this, i);
//        barChartArr[i]->setFixedSize(600, 300);
//        gLayout->addWidget(barChartArr[i], 0, 0);
//        if(i != 1) {
//            barChartArr[i]->hide();
//        }
//    }

    mBarChart = new BarChart(this, 1);
    mBarChart->setFixedSize(600, 300);
    gLayout->addWidget(mBarChart, 0, 0);

//    stackedBarChart = new StackedBarChart(this);
//    stackedBarChart->setFixedSize(600, 300);
//    gLayout->addWidget(stackedBarChart, 1, 0);
}

void Dashboard::paintEvent(QPaintEvent *event)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    //paint the title
    painter.setPen(QColor(255,115,115));
    painter.setFont(QFont("Futura", 20));
    painter.drawText(QRect(50, 50, event->rect().width(), event->rect().height()), "Dashboard");
}

void Dashboard::loadData()
{
//    for(int i = 1; i <= 4; i++) {
//        if(!barChartArr[i]->isHidden()) {
//            barChartArr[i]->reloadChart(i);
//        }
//    }
    mBarChart->reloadChart(1);
}

void Dashboard::comboBoxChanged(int index)
{
//    for(int i = 1; i <= 4; ++i) {
//        if(i == index) {
//            barChartArr[i]->show();
//        }
//        else {
//            barChartArr[i]->hide();
//        }
//    }
    loadData();
}
