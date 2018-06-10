#include "customtitlebar.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    checkedAct = NULL;
    hoveredAct = NULL;

    addAction("—", QIcon(""));
    addAction("X", QIcon(""));

    setStyleSheet("QWidget { background-color: #C39797; }");
    setFixedSize(1050, 45);
}

QAction *CustomTitleBar::addAction(const QString &text, const QIcon &icon)
{
    QAction *action = new QAction(icon, text, this);
    action->setCheckable(true);
    actList.push_back(action);
    return action;
}

QAction *CustomTitleBar::actionAt(const QPoint &point)
{
    int posX = 960;
    for(auto action : actList) {
        QRect actRect(posX, 8, 45, 45); //has to be 50, 50
        if(actRect.contains(point)) {
            return action;
        }
        posX += 45;
    }
    return NULL;
}

void CustomTitleBar::paintEvent(QPaintEvent *event)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    QFont font("Times", 13);
    painter.setFont(font);

    int posX = 960 + 12; //move the icon to the center, so + 12
    for(auto action : actList) {
        if(action == hoveredAct) {
            QPen hoveredPen;
            hoveredPen.setWidth(5);
            hoveredPen.setBrush(QColor(255, 0, 0));
            painter.setPen(hoveredPen);

            if(action->text() == "—") {
                painter.fillRect(QRect(960, 0, 45, 45), QColor(0, 255, 0)); //set background color
            }
            if(action->text() == "X") {
                painter.fillRect(QRect(1005, 0, 45, 45), QColor(255, 255, 0)); //set background color
            }
        }

        QRect textRect(posX, 8, event->rect().width(), event->rect().height());
        painter.drawText(textRect, action->text());

        posX += 45;
    }
}

void CustomTitleBar::mousePressEvent(QMouseEvent *event)
{
    QAction *action = actionAt(event->pos());
    checkedAct = action;
    if(checkedAct != NULL) {
        int index = std::find(actList.begin(), actList.end(), checkedAct) - actList.begin();
        emit actionChanged(index);
    }
//    update();
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    QAction *action = actionAt(event->pos());
    hoveredAct = action;
    update();
}
