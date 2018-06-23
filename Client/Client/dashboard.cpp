#include "dashboard.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QLabel>
#include <QKeyEvent>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent)
{
    titleVec = {"Hourly", "Daily", "Weekly", "Monthly", "Yearly"};

    setWindowStyleSheet();
    setWindowLayout();
    createTimeSpanBox();
    createCharts();

    connect(this, &Dashboard::loadingData, this, &Dashboard::loadData);
    connect(timeSpanBox, QOverload<int>::of(&QComboBox::activated),
            [=](int index){
        comboBoxChanged(index);
    });
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
                "QScrollBar:vertical { background: #faebd7; width: 4px; }"
                "QScrollBar::handle:vertical { background: #ff7373; }"
                );
}

void Dashboard::setWindowLayout()
{
    mainVLayout = new QVBoxLayout(this);
    timeSpanHLayout = new QHBoxLayout; //do not set parent

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Window);
    scrollArea->setFrameShadow(QFrame::Plain);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(1000, 450);

    scrollWidget = new QWidget(this);

    gLayout = new QGridLayout;

    scrollWidget->setLayout(gLayout);

    mainVLayout->addLayout(timeSpanHLayout);
    mainVLayout->addWidget(scrollArea);
    this->setLayout(mainVLayout);

    scrollArea->setWidget(scrollWidget); //needs to be in the last
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
    for(int i = 0; i < 5; ++i) {
        appUsageChartArr[i] = new AppUsageStackedBarChart(this, i, QString("%1 App Usage").arg(titleVec[i]));
        appUsageChartArr[i]->setFixedSize(600, 300);

        pieChartArr[i] = new PieChart(this, i, QString("%1 Key Pressed").arg(titleVec[i]));
        pieChartArr[i]->setFixedSize(300, 300);

        if(i != 1) {
            appUsageChartArr[i]->hide();

            pieChartArr[i]->hide();
        }

        gLayout->addWidget(appUsageChartArr[i], 0, 0);
        gLayout->addWidget(pieChartArr[i], 1, 1);
    }

    for(int i = 0; i < 4; ++i) {
        barChartArr[i] = new BarChart(this, i + 1);
        barChartArr[i]->setFixedSize(600, 300);

        stackedBarChartArr[i] = new StackedBarChart(this, i + 1);
        stackedBarChartArr[i]->setFixedSize(600, 300);

        if(i != 0) {
            barChartArr[i]->hide();

            stackedBarChartArr[i]->hide();
        }

        gLayout->addWidget(barChartArr[i], 1, 0);
        gLayout->addWidget(stackedBarChartArr[i], 2, 0);
    }
}

bool Dashboard::eventFilter(QObject *watched, QEvent *event)
{
    if(timeSpanBox){
        if(timeSpanBox == watched && event->type() == QEvent::KeyPress){
           QKeyEvent *key = static_cast<QKeyEvent *>(event);
           if(!key->text().isEmpty()) {
               return true;
           }
        }
    }
    return QObject::eventFilter(watched, event);
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

void Dashboard::newDayComes()
{
    for(int i = 0; i < 4; ++i) {
        delete barChartArr[i];
        delete stackedBarChartArr[i];
    }
    for(int i = 0; i < 5; ++i) {
        delete appUsageChartArr[i];
        delete pieChartArr[i];
    }
    createCharts();
}

void Dashboard::loadData()
{
    for(int i = 0; i < 4; i++) {
        if(!barChartArr[i]->isHidden()) {
            barChartArr[i]->reloadChart(i + 1);
        }

        if(!stackedBarChartArr[i]->isHidden()) {
            stackedBarChartArr[i]->reloadChart(i + 1);
        }
    }

    for(int i = 0; i < 5; i++) {
        if(!pieChartArr[i]->isHidden()) {
            pieChartArr[i]->reloadChart();
        }

        if(!appUsageChartArr[i]->isHidden()) {
            appUsageChartArr[i]->reloadChart();
        }
    }
}

void Dashboard::comboBoxChanged(int index)
{
    for(int i = 0; i < 4; ++i) {
        if(i + 1 == index) {
            barChartArr[i]->show();
            barChartArr[i]->reloadChart(i + 1);

            stackedBarChartArr[i]->show();
            stackedBarChartArr[i]->reloadChart(i + 1);
        }
        else {
            barChartArr[i]->hide();

            stackedBarChartArr[i]->hide();
        }
    }

    for(int i = 0; i < 5; ++i) {
        if(i == index) {
            pieChartArr[i]->show();
            pieChartArr[i]->reloadChart();

            appUsageChartArr[i]->show();
            appUsageChartArr[i]->reloadChart();
        }
        else {
            pieChartArr[i]->hide();

            appUsageChartArr[i]->hide();
        }
    }

    if(index == 0) {
        for(int i = 0; i < 4; i++) {
            if(!stackedBarChartArr[i]->isHidden()) {
                stackedBarChartArr[i]->hide();
                barChartArr[i]->hide();
            }
        }
    }
}
