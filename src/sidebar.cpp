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
    this->titleBarWidget()->hide(); //need to add this pointer
    setMinimumSize(200, 100);

    setMouseTracking(true);

    addAction(tr("Overview"), QIcon(":/icons/home_24px.png"));
    addAction(tr("Dashboard"), QIcon(":/icons/dashboard_24px.png"));
    addAction(tr("Users"), QIcon(":/icons/users_24px.png"));
    addAction(tr("Settings"), QIcon(":/icons/settings_24px.png"));

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
    int posY = 110;
    for(auto action : actList) {
        QRect actRect(0, posY, 195, 50); //set width to 195 and height to 50, or the hover effect will sometimes show and the checked effect will disappear
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

    //draw menu
    painter.fillRect(rect(), QColor(240, 248, 255)); //set background color
    int posY = 110;
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

    //paint icon
    QIcon icon(":/icons/letter-n.png");
    QRect iconRect(20, 39, 35, 35);
    icon.paint(&painter, iconRect);

    //paint the title and it has to be after menu is drew
    painter.setPen(QColor(195,151,151));
    painter.setFont(QFont("Futura", 25));
    painter.drawText(QRect(65, 30, 100, 50), "Nana");
}

void Sidebar::mousePressEvent(QMouseEvent *event)
{
    QAction *action = actionAt(event->pos());

    //specify the sidebar clickable area,
    //so to make sure that the sidebar menu will only change in the clickable area
    if(event->pos().y() >= 110 && event->pos().y() <= 310) {
        checkedAct = action;
    }
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
