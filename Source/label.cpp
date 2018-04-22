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

    hourAct = new QAction("Hour", viewMenu);
    hourAct->setCheckable(true);

    dayAct = new QAction("Day", viewMenu);
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
    dayAct->setChecked(true);

    viewMenu->addAction(hourAct);
    viewMenu->addAction(dayAct);
    viewMenu->addAction(monthAct);
    viewMenu->addAction(yearAct);

    connect(this, &Label::customContextMenuRequested, [=](){ contextMenu->exec(QCursor::pos()); });
    connect(hourAct, &QAction::changed, this, &Label::emitViewModeSignal);
    connect(dayAct, &QAction::changed, this, &Label::emitViewModeSignal);
}

void Label::emitViewModeSignal()
{
    if(hourAct->isChecked()) {
        emit viewNodeChanged(1);
    }
    if(dayAct->isChecked()) {
        emit viewNodeChanged(2);
    }
}
