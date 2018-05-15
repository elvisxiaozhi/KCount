#include "mouseclick.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include "signalemitter.h"

MouseClick::MouseClick(QWidget *parent)
    : QWidget(parent), leftClickedTimes(0), rightClickedTimes(0)
{
    setWindowStyleSheet();

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);
    contHLayout = new QHBoxLayout;
    contHLayout->setSpacing(0);

    title = new QLabel(this);
    title->setText("Total Clicked");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    leftClickedCont = new QLabel(this);
    leftClickedCont->setObjectName("Content");
    leftClickedCont->setAlignment(Qt::AlignCenter);
    leftClickedCont->setFixedSize(140, 200);
    leftClickedCont->setText(QString("Left:<br><br> %1").arg(leftClickedTimes));

    rightClickedCont = new QLabel(this);
    rightClickedCont->setObjectName("Content");
    rightClickedCont->setAlignment(Qt::AlignCenter);
    rightClickedCont->setFixedSize(140, 200);
    rightClickedCont->setText(QString("right:<br><br> %1").arg(rightClickedTimes));

    contHLayout->addWidget(leftClickedCont);
    contHLayout->addWidget(rightClickedCont);
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
    leftClickedCont->setText(QString("Left:<br><br> %1").arg(leftClickedTimes));
}

void MouseClick::rightClicked()
{
    rightClickedTimes++;
    rightClickedCont->setText(QString("right:<br><br> %1").arg(rightClickedTimes));
}
