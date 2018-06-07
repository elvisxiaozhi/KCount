#include "sidebar.h"
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QIcon>

Sidebar::Sidebar(QWidget *parent) : QDockWidget(parent)
{
    //hide dock widget title bar
    QWidget *titleBarWidget = new QWidget(this);
    setTitleBarWidget(titleBarWidget);
    this->titleBarWidget()->hide();
    setMinimumSize(200, 100);

    setMouseTracking(true);

    this->addAction(tr("Overview"), QIcon(":/Resources/Icons/home_24px.png"));
    this->addAction(tr("Dashboard"), QIcon(":/Resources/Icons/dashboard_24px.png"));
    this->addAction(tr("Users"), QIcon(":/Resources/Icons/users_24px.png"));
    this->addAction(tr("Settings"), QIcon(":/Resources/Icons/settings_24px.png"));

    checkedAct = actList[0];
    hoveredAct = NULL;
}

QAction *Sidebar::addAction(const QString &text, const QIcon &icon)
{
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    actList.push_back(action);
    return action;
}

QAction *Sidebar::actionAt(const QPoint &point)
{
    int posY = 10;
    for(auto action : actList) {
        QRect actRect(0, posY, rect().width(), 30);
        if(actRect.contains(point)) {
            return action;
        }
        posY += 50;
    }
    return NULL;
}

void Sidebar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    //paint the title
    painter.setPen(QColor(255,115,115));
    painter.setFont(QFont("Futura", 20));
    painter.drawText(QRect(50, 50, event->rect().width(), event->rect().height()), "Nana");

    painter.fillRect(rect(), QColor(240, 248, 255)); //set background color
    int posY = 10;
    for(auto action : actList) {
        if(action == checkedAct) {
            QPen checkedPen;  // creates a default pen
            checkedPen.setWidth(5);
            checkedPen.setBrush(Qt::red);
            painter.setPen(checkedPen);
            painter.drawLine(0, posY, 0, posY + 30);

            QFont checkedFont("Times", 10, QFont::Bold);
            painter.setFont(checkedFont);
            painter.setPen(QColor(102,102,102));
        }
        else {
            if(action == hoveredAct) {
                QFont uncheckedFont("Times", 10, QFont::Bold);
                painter.setFont(uncheckedFont);

                QPen hoveredPen;
                hoveredPen.setWidth(5);
                hoveredPen.setBrush(QColor(255, 192, 203));
                painter.setPen(hoveredPen);
                painter.drawLine(0, posY, 0, posY + 30);
            }
            else {
                QFont uncheckedFont("Times", 10);
                painter.setFont(uncheckedFont);

                QPen uncheckedPen;
                uncheckedPen.setBrush(QColor(128,128,128));
                painter.setPen(uncheckedPen);
            }
        }

        QIcon icon(action->icon());
        QRect iconRect(10, posY, 30, 30);
        icon.paint(&painter, iconRect);
        QRect textRect(50, posY + 10, event->rect().width(), event->rect().height());
        painter.drawText(textRect, action->text());

        posY += 50;
    }
}

void Sidebar::mousePressEvent(QMouseEvent *event)
{
    QAction *action = actionAt(event->pos());
    checkedAct = action;
    if(checkedAct != NULL) {
        int index = std::find(actList.begin(), actList.end(), checkedAct) - actList.begin();
        emit actionChanged(index);
    }
    update();
}

void Sidebar::mouseMoveEvent(QMouseEvent *event)
{
    QAction *action = actionAt(event->pos());
    hoveredAct = action;
    update();
}
