#include "customtitlebar.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("QWidget { background-color: red; }");
    setFixedSize(1050, 40);
}

void CustomTitleBar::paintEvent(QPaintEvent *)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

//    painter.fillRect(QRect(0, 0, event->rect().width(), 40), QColor(195,151,151));
}
