#include "label.h"
#include <QDebug>
#include <QMenu>

Label::Label()
{
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setStyleSheet("QLabel { background-color: #FAD7A0; color: #FDFEFE; font-size: 50px; }");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setContextMenu();
}

void Label::setLblColor(int pressedTimes)
{
    if(pressedTimes >= 100 && pressedTimes < 500) {
        setStyleSheet("QLabel { background-color: #82E0AA; color: #FDFEFE; font-size: 50px; }"); //green bg color
    }
    if(pressedTimes >= 500 && pressedTimes < 1000) {
        setStyleSheet("QLabel { background-color: #5DADE2; color: #FDFEFE; font-size: 50px; }"); //blue bg color
    }
    if(pressedTimes >= 1000 && pressedTimes < 5000) {
        setStyleSheet("QLabel { background-color: #7D3C98; color: #FDFEFE; font-size: 50px; }"); //purple bg color
    }
    if(pressedTimes >= 5000) {
        setStyleSheet("QLabel { background-color: #E74C3C; color: #FDFEFE; font-size: 50px; }"); //red bg color
    }
    if(pressedTimes >= 10000) {
        setStyleSheet("QLabel { background-color: #A93226; color: #FDFEFE; font-size: 50px; }"); //deep red bg color
    }
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

    weekAct = new QAction("Week", viewMenu);
    weekAct->setCheckable(true);

    monthAct = new QAction("Month", viewMenu);
    monthAct->setCheckable(true);

    yearAct = new QAction("Year", viewMenu);
    yearAct->setCheckable(true);

    QActionGroup *viewChoiceGroup = new QActionGroup(viewMenu);
    viewChoiceGroup->addAction(hourAct);
    viewChoiceGroup->addAction(dayAct);
    viewChoiceGroup->addAction(weekAct);
    viewChoiceGroup->addAction(monthAct);
    viewChoiceGroup->addAction(yearAct);
    dayAct->setChecked(true);

    viewMenu->addAction(hourAct);
    viewMenu->addAction(dayAct);
    viewMenu->addAction(weekAct);
    viewMenu->addAction(monthAct);
    viewMenu->addAction(yearAct);

    connect(this, &Label::customContextMenuRequested, [=](){ contextMenu->exec(QCursor::pos()); });
    connect(hourAct, &QAction::triggered, this, &Label::emitViewModeSignal); //note the triggered signal, not changed signal
    connect(dayAct, &QAction::triggered, this, &Label::emitViewModeSignal); //using changed, emitViewModeSignal will be called twice
    connect(weekAct, &QAction::triggered, this, &Label::emitViewModeSignal);
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
    if(weekAct->isChecked()) {
        emit viewNodeChanged(3);
    }
    if(monthAct->isChecked()) {
        emit viewNodeChanged(4);
    }
    if(yearAct->isChecked()) {
        emit viewNodeChanged(5);
    }
}
