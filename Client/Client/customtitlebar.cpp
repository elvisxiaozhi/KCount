#include "customtitlebar.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include <windows.h>
#include <QWindow>

CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);

    checkedAct = NULL;
    hoveredAct = NULL;

    addAction("—", QIcon(""));
    addAction("×", QIcon(""));

    setStyleSheet("QWidget { background-color: #F3D6E4; }");
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
        //35, 35 or whatever it fits, they have to be less than its width and height 45, 45
        //or sometimes the hover effect will stay
        QRect actRect(posX, 8, 35, 35);
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

            painter.setPen(QColor(255,255,255)); //change text color when hover

            if(action->text() == "—") {
                painter.fillRect(QRect(960, 0, 45, 45), QColor(0,255,255)); //set background color
            }
            if(action->text() == "×") {
                painter.fillRect(QRect(1005, 0, 45, 45), QColor(233, 75, 60)); //set background color
            }
        }
        else {
            painter.setPen(QColor(128,128,128)); //change the text color to normal grey when not hover
        }

        QRect textRect(posX, 8, event->rect().width(), event->rect().height());
        painter.drawText(textRect, action->text());

        posX += 45;
    }


    if(hoveredOnNotify) {
        painter.fillRect(QRect(915, 0, 45, 45), QColor(0,255,255)); //set background color
    }
    //paint icon
    QIcon icon(":/Resources/Icons/notification.png");
    QRect iconRect(915 + 10, 10, 25, 25); //plus 12, so the icon can align center
    icon.paint(&painter, iconRect);
}

void CustomTitleBar::mousePressEvent(QMouseEvent *event)
{
//    startPos = event->pos();

    if (event->buttons().testFlag(Qt::LeftButton)) //use the native windows api, the issue that window suddently move will not show, do not know why
    {
        HWND hWnd = ::GetAncestor((HWND)(window()->windowHandle()->winId()), GA_ROOT);
        POINT pt;
        ::GetCursorPos(&pt);
        ::ReleaseCapture();
        ::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, POINTTOPOINTS(pt));
    }

    QAction *action = actionAt(event->pos());
    checkedAct = action;
    if(checkedAct != NULL) {
        int index = std::find(actList.begin(), actList.end(), checkedAct) - actList.begin();
        emit actionChanged(index);
    }
//    update(); //update must be in the last, or the window will suddently move to another place
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent *event)
{
//    if(event->buttons() == Qt::LeftButton) {
//        QPoint delta = event->pos() - startPos;
//        QWidget *w = window();
//        if(w) {
//            w->move(w->pos()+ delta);
//        }
//    }

    if(event->x() < 960 && event->x() >= 915) {
        hoveredOnNotify = true;
    }
    else {
        hoveredOnNotify = false;
    }

    QAction *action = actionAt(event->pos());
    hoveredAct = action;
    update(); //update must be in the last, or the window will suddently move to another place
}
