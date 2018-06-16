#include "notification.h"
#include <QDesktopWidget>
#include <QApplication>

Notification::Notification(QWidget *parent) : QDialog(parent)
{
    setFixedSize(300, 200);
    move(qApp->desktop()->geometry().bottomRight() - QPoint(300, 290));
}
