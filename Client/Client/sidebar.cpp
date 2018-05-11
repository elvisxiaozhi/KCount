#include "sidebar.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#include <QIcon>

Sidebar::Sidebar()
{
    //hide dock widget title bar
    QWidget *titleBarWidget = new QWidget(this);
    setTitleBarWidget(titleBarWidget);
    this->titleBarWidget()->hide();
    setMinimumSize(150, 100);

    this->addAction(tr("Overview"), QIcon(":/Resources/Icons/home_24px.png"));
    this->addAction(tr("Dashboard"), QIcon(":/Resources/Icons/home_24px.png"));
    this->addAction(tr("Users"), QIcon(":/Resources/Icons/home_24px.png"));
    this->addAction(tr("Settings"), QIcon(":/Resources/Icons/home_24px.png"));
}

QAction *Sidebar::addAction(const QString &text, const QIcon &icon)
{
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    actList.push_back(action);
    return action;
}

void Sidebar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(), QColor(240, 248, 255));
    int posY = 10;
    for(auto action : actList) {
        QIcon icon(action->icon());
        QRect iconRect(10, posY, 30, 30);
        icon.paint(&painter, iconRect);
        QRect textRect(50, posY + 10, event->rect().width(), event->rect().height());
        painter.drawText(textRect, action->text());
        posY += 30;
    }
}
