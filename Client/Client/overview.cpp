#include "overview.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

Overview::Overview(QWidget *parent) : QWidget(parent)
{
}

void Overview::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPainter painter(this);
    painter.setPen(QColor(255,115,115));
    painter.setFont(QFont("Futura", 20));
    painter.drawText(QRect(50, 50, event->rect().width(), event->rect().height()), "Overview");
}

void Overview::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { bgcolor: #f5f5f5; }"
                );
}
