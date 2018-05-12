#include "overview.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

Overview::Overview(QWidget *parent) : QWidget(parent)
{
    setWindowLayout();
    setWindowStyleSheet();
    setTimeSpanBox();
}

void Overview::setWindowLayout()
{
    mainVLayout = new QVBoxLayout(this);
    timeSpanHLayout = new QHBoxLayout(); //do not set parent

    mainVLayout->addLayout(timeSpanHLayout);
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

void Overview::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #d3ffce; }"
                );
}

void Overview::setTimeSpanBox()
{
    QLabel *spanTextLbl = new QLabel(this);
    spanTextLbl->setText("Time Span: ");

    timeSpanBox = new QComboBox(this);
    timeSpanBox->addItem(tr("Day"));
    timeSpanBox->addItem(tr("Week"));

    timeSpanHLayout->addStretch();
    timeSpanHLayout->addWidget(spanTextLbl);
    timeSpanHLayout->addWidget(timeSpanBox);
}
