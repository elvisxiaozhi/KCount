#include "overview.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QTime>

Overview::Overview(QWidget *parent) : QWidget(parent)
{
    setWindowLayout();
    setWindowStyleSheet();
    setTimeSpanBox();
    setLbls();

    setTimer(); //set the time, so the database will save automatically in each hour

    connect(this, &Overview::updateDatabase, mostPressed, &MostPressed::updateDatabase);
}

void Overview::setWindowLayout()
{
    mainVLayout = new QVBoxLayout(this);
    timeSpanHLayout = new QHBoxLayout; //do not set parent
    lblGLayout = new QGridLayout;

    mainVLayout->addLayout(timeSpanHLayout);
    mainVLayout->addLayout(lblGLayout);
    this->setLayout(mainVLayout);
}

void Overview::paintEvent(QPaintEvent *event)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    //paint the title
    painter.setPen(QColor(255,115,115));
    painter.setFont(QFont("Futura", 20));
    painter.drawText(QRect(50, 50, event->rect().width(), event->rect().height()), "Overview");

//    //paint notification icon
    QIcon icon(":/Resources/Icons/notification.png");
    QRect iconRect(800, 55, 30, 30);
    icon.paint(&painter, iconRect);
}

void Overview::updateTimer() const
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    emit updateDatabase();
}

void Overview::setWindowStyleSheet()
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

void Overview::setTimeSpanBox()
{
    QLabel *spanTextLbl = new QLabel(this);
    spanTextLbl->setText("Time Span: ");
    spanTextLbl->setObjectName("SpanTextLbl");

    timeSpanBox = new QComboBox(this);
    timeSpanBox->addItem(tr("Hour"));
    timeSpanBox->addItem(tr("Day"));
    timeSpanBox->addItem(tr("Week"));
    timeSpanBox->addItem(tr("Month"));
    timeSpanBox->addItem(tr("Year"));

    timeSpanHLayout->addStretch();
    timeSpanHLayout->addWidget(spanTextLbl);
    timeSpanHLayout->addWidget(timeSpanBox);
}

void Overview::setLbls()
{
    totalPressed = new TotalPressed(this);
    totalPressed->setMaximumSize(300, 300);

    mostPressed = new MostPressed(this);
    mostPressed->setMaximumSize(300, 300);

    lblGLayout->addWidget(totalPressed, 0, 0);
    lblGLayout->addWidget(mostPressed, 0, 1);
}

void Overview::setTimer()
{
    QStringList currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    timer = new QTimer(this);
    timer->start(1000 * 60 * 60 - 1000 * 60 * QString(currentTimeStringList[1]).toInt() - 1000 * QString(currentTimeStringList[2]).toInt()); //1 sec * 60 (= 1 minute) * 60 (= 1 hour)
    connect(timer, &QTimer::timeout, this, &Overview::updateTimer);
}
