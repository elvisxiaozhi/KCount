#include "label.h"
#include <QDebug>
#include <QMenu>

Label::Label()
{
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setStyleSheet("QLabel { background-color: #FAD7A0; color: #8E44AD; font-size: 50px; }");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setContextMenu();
}

void Label::setContextMenu()
{
    QMenu *contextMenu = new QMenu(this);

    QMenu *viewMenu = new QMenu("View", contextMenu);
    contextMenu->addMenu(viewMenu);

    QAction *hourAct = new QAction("Hour", viewMenu);
    hourAct->setCheckable(true);

    QAction *dayAct = new QAction("Day", viewMenu);
    dayAct->setCheckable(true);

    QAction *monthAct = new QAction("Month", viewMenu);
    monthAct->setCheckable(true);

    QAction *yearAct = new QAction("Year", viewMenu);
    yearAct->setCheckable(true);

    QActionGroup *viewChoiceGroup = new QActionGroup(viewMenu);
    viewChoiceGroup->addAction(hourAct);
    viewChoiceGroup->addAction(dayAct);
    viewChoiceGroup->addAction(monthAct);
    viewChoiceGroup->addAction(yearAct);
    hourAct->setChecked(true);

    viewMenu->addAction(hourAct);
    viewMenu->addAction(dayAct);
    viewMenu->addAction(monthAct);
    viewMenu->addAction(yearAct);

    connect(this, &Label::customContextMenuRequested, [this, contextMenu](){ contextMenu->exec(QCursor::pos()); });
}
