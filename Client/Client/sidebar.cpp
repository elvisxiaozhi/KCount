#include "sidebar.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>

const int actionHeight = 90;

Sidebar::Sidebar(QWidget *parent) : QWidget(parent)
{

}

QSize Sidebar::minimumSizeHint() const
{
    return actionHeight * QSize(1, actList.size());
}

QAction *Sidebar::addAction(const QString &text, const QIcon &icon)
{
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    actList.push_back(action);
    update();
    return action;
}

void Sidebar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    qDebug() << event->rect().width();

    int yPos = 0;

    painter.fillRect(rect(), QColor(240, 248, 255));
    for(auto action: actList)
    {
        QRect actionRect(0, yPos, 90, 90); //icon size

        painter.setPen(QColor(51, 51, 51)); //set text color
        QSize size = painter.fontMetrics().size(Qt::TextSingleLine, action->text()); //text size
        QRect actionTextRect(QPoint(actionRect.width()/2 - size.width()/2, actionRect.bottom()-size.height()-5), size); //text position
        painter.drawText(actionTextRect, action->text()); //add text

        QRect actionIconRect(0, yPos + 10, actionRect.width(), actionRect.height()-2*actionTextRect.height()-10);
        QIcon actionIcon(action->icon());
        actionIcon.paint(&painter, actionIconRect); //add icon

        yPos += actionRect.height();
    }
}
