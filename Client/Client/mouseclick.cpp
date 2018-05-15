#include "mouseclick.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include "signalemitter.h"
#include "database.h"

MouseClick::MouseClick(QWidget *parent) : QWidget(parent)
{
    leftClickedTimes = Database::returnLeftClickTimes(1);
    rightClickedTimes = Database::returnRightClickTimes(1);
    tempLeftClickedTimes = 0;
    tempRightClickedTimes = 0;

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

    contHLayout->addWidget(leftClickCont);
    contHLayout->addWidget(rightClickCont);
    mainVLayout->addWidget(title);
    mainVLayout->addLayout(contHLayout);

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

void MouseClick::leftClicked()
{
    leftClickedTimes++;
    tempLeftClickedTimes++;
    leftClickCont->setText(QString("Left:<br><br> %1").arg(leftClickedTimes));
}

void MouseClick::rightClicked()
{
    rightClickedTimes++;
    tempRightClickedTimes++;
    rightClickCont->setText(QString("right:<br><br> %1").arg(rightClickedTimes));
}
