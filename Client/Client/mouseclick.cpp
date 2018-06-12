#include "mouseclick.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include "signalemitter.h"
#include "database.h"
#include <QDebug>
#include "stackedbarchart.h"
#include <QTime>

MouseClick::MouseClick(QWidget *parent) : QWidget(parent)
{
    leftClickedTimes = Database::returnClickedTimes("LeftClick", 1);
    rightClickedTimes = Database::returnClickedTimes("RightClick", 1);
    tempLeftClickedTimes = 0;
    tempRightClickedTimes = 0;
    currentHour = QTime::currentTime().toString("h").toInt();

    setWindowStyleSheet();

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);
    contHLayout = new QHBoxLayout;
    contHLayout->setSpacing(0);

    title = new QLabel(this);
    title->setText("Total Clicked");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    leftClickCont = new QLabel(this);
    leftClickCont->setObjectName("Content");
    leftClickCont->setAlignment(Qt::AlignCenter);
    leftClickCont->setFixedSize(140, 200);
    leftClickCont->setText(QString("Left:<br><br> %1").arg(leftClickedTimes));

    rightClickCont = new QLabel(this);
    rightClickCont->setObjectName("Content");
    rightClickCont->setAlignment(Qt::AlignCenter);
    rightClickCont->setFixedSize(140, 200);
    rightClickCont->setText(QString("right:<br><br> %1").arg(rightClickedTimes));

    QSpacerItem *spacerItem = new QSpacerItem(1, 14, QSizePolicy::Fixed, QSizePolicy::Fixed); //make the content a little bit higher

    contHLayout->addWidget(leftClickCont);
    contHLayout->addWidget(rightClickCont);
    mainVLayout->addWidget(title);
    mainVLayout->addLayout(contHLayout);
    mainVLayout->addSpacerItem(spacerItem);

    connect(Emitter::Instance(), &SignalEmitter::leftClicked, this, &MouseClick::leftClicked);
    connect(Emitter::Instance(), &SignalEmitter::rightClicked, this, &MouseClick::rightClicked);
}

void MouseClick::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background: #F9E79F; }"
                "QLabel#Title { font: 20px; color: #666666; }"
                "QLabel#Content { font: 20px; color: #FDFEFE; background: #008080; border: 2px solid #ff1493; }"
                );
}

void MouseClick::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void MouseClick::updateDatabase()
{
    Database::updateLeftClickToDB(tempLeftClickedTimes, tempRightClickedTimes);
    tempLeftClickedTimes = 0;
    tempRightClickedTimes = 0;
    currentHour = QTime::currentTime().toString("h").toInt();
}

void MouseClick::reloadData(int index)
{
    updateDatabase();
    leftClickedTimes = Database::returnClickedTimes("LeftClick", index);
    leftClickCont->setText(QString("Left:<br><br> %1").arg(leftClickedTimes));
    rightClickedTimes = Database::returnClickedTimes("RightClick", index);
    rightClickCont->setText(QString("Left:<br><br> %1").arg(rightClickedTimes));
}

void MouseClick::leftClicked()
{
    ++leftClickedTimes;
    ++tempLeftClickedTimes;

    ++StackedBarChart::dailyMap[currentHour].first;
    ++StackedBarChart::weeklyMap[StackedBarChart::weeklyMap.size() - 1].first;
    ++StackedBarChart::monthlyMap[StackedBarChart::monthlyMap.size() - 1].first;
    ++StackedBarChart::yearlyMap[StackedBarChart::yearlyMap.size() - 1].first;

    leftClickCont->setText(QString("Left:<br><br> %1").arg(leftClickedTimes));
}

void MouseClick::rightClicked()
{
    ++rightClickedTimes;
    ++tempRightClickedTimes;

    ++StackedBarChart::dailyMap[currentHour].second;
    ++StackedBarChart::weeklyMap[StackedBarChart::weeklyMap.size() - 1].second;
    ++StackedBarChart::monthlyMap[StackedBarChart::monthlyMap.size() - 1].second;
    ++StackedBarChart::yearlyMap[StackedBarChart::yearlyMap.size() - 1].second;

    rightClickCont->setText(QString("right:<br><br> %1").arg(rightClickedTimes));
}
