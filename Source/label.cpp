#include "label.h"
#include <QDebug>
#include <QMenu>

Label::Label()
{
    setAlignment(Qt::AlignCenter);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    setStyleSheet("QLabel { background-color: #FAD7A0; color: #FDFEFE; font-size: 50px; font-family: Comic Sans MS; border-radius: 15px; border: 2px solid #F1C40F; }");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setContextMenu();
}

void Label::setLblColor(int pressedTimes)
{
    if(pressedTimes < 100) {
        setStyleSheet("QLabel { background-color: #FAD7A0; color: #FDFEFE; font-size: 50px; font-family: Comic Sans MS; border-radius: 15px; border: 2px solid #F1C40F; }"); //default style sheet
    }
    if(pressedTimes >= 100 && pressedTimes < 500) {
        setStyleSheet("QLabel { background-color: #82E0AA; color: #FDFEFE; font-size: 50px; font-family: Comic Sans MS; border-radius: 15px; border: 2px solid #F1C40F; }"); //green bg color
    }
    if(pressedTimes >= 500 && pressedTimes < 1000) {
        setStyleSheet("QLabel { background-color: #5DADE2; color: #FDFEFE; font-size: 50px; font-family: Comic Sans MS; border-radius: 15px; border: 2px solid #F1C40F; }"); //blue bg color
    }
    if(pressedTimes >= 1000 && pressedTimes < 5000) {
        setStyleSheet("QLabel { background-color: #7D3C98; color: #FDFEFE; font-size: 50px; font-family: Comic Sans MS; border-radius: 15px; border: 2px solid #F1C40F; }"); //purple bg color
    }
    if(pressedTimes >= 5000) {
        setStyleSheet("QLabel { background-color: #E74C3C; color: #FDFEFE; font-size: 50px; font-family: Comic Sans MS; border-radius: 15px; border: 2px solid #F1C40F; }"); //red bg color
    }
    if(pressedTimes >= 10000) {
        setStyleSheet("QLabel { background-color: #A93226; color: #FDFEFE; font-size: 50px; font-family: Comic Sans MS; border-radius: 15px; border: 2px solid #F1C40F; }"); //deep red bg color
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
        emit viewModeChanged(0);
    }
    if(dayAct->isChecked()) {
        emit viewModeChanged(1);
    }
    if(weekAct->isChecked()) {
        emit viewModeChanged(2);
    }
    if(monthAct->isChecked()) {
        emit viewModeChanged(3);
    }
    if(yearAct->isChecked()) {
        emit viewModeChanged(4);
    }
}

void Label::setActionChecked(int checkedAction)
{
    if(checkedAction == 0) {
        hourAct->setChecked(true);
    }
    if(checkedAction == 1) {
        dayAct->setChecked(true);
    }
    if(checkedAction == 2) {
        weekAct->setChecked(true);
    }
    if(checkedAction == 3) {
        monthAct->setChecked(true);
    }
    if(checkedAction == 4) {
        yearAct->setChecked(true);
    }
}
