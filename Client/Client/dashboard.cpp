#include "dashboard.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent)
{
    setWindowStyleSheet();
}

void Dashboard::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #eeeeee; }"
                );

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
