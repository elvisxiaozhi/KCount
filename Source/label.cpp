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

    monthAct = new QAction("Month", viewMenu);
    monthAct->setCheckable(true);

    yearAct = new QAction("Year", viewMenu);
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
    connect(hourAct, &QAction::triggered, this, &Label::emitViewModeSignal); //note the triggered signal, not changed signal
    connect(dayAct, &QAction::triggered, this, &Label::emitViewModeSignal); //using changed, emitViewModeSignal will be called twice
    connect(monthAct, &QAction::triggered, this, &Label::emitViewModeSignal);
    connect(yearAct, &QAction::triggered, this, &Label::emitViewModeSignal);
}

void Label::emitViewModeSignal()
{
    if(hourAct->isChecked()) {
        emit viewNodeChanged(1);
    }
    if(dayAct->isChecked()) {
        emit viewNodeChanged(2);
    }
    if(monthAct->isChecked()) {
        emit viewNodeChanged(3);
    }
    if(yearAct->isChecked()) {
        emit viewNodeChanged(4);
    }
}
