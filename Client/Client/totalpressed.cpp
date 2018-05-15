#include "totalpressed.h"
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include "signalemitter.h"
#include <QDebug>
#include "database.h"

TotalPressed::TotalPressed(QWidget *parent)
    : QWidget(parent)
{
    totalPressedTimes = Database::returnTotalPressedTimes(1);

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);

    title = new QLabel(this);
    title->setText("Total Pressed");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    content = new QLabel(this);
    content->setText(QString::number(totalPressedTimes));
    content->setObjectName("Content");
    content->setAlignment(Qt::AlignCenter);
    content->setFixedHeight(200);

    mainVLayout->addWidget(title);
    mainVLayout->addWidget(content);

    setWindowStyleSheet();
    setContColor(totalPressedTimes);

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &TotalPressed::keyPressed);
}

void TotalPressed::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #b0e0e6; }"
                "QLabel#Title { font: 20px; color: #666666; }"
                "QLabel#Content { font: 70px; color: #FDFEFE; background: #FAD7A0; }"
                );
}

void TotalPressed::setContColor(const unsigned long int &pressedTimes)
{
    if(pressedTimes < 100) {
        content->setStyleSheet("QLabel#Content { background-color: #FAD7A0; color: #FDFEFE; font-size: 70px; }"); //default style sheet
    }
    if(pressedTimes >= 100 && pressedTimes < 500) {
        content->setStyleSheet("QLabel#Content { background-color: #82E0AA; color: #FDFEFE; font-size: 70px; }"); //green bg color
    }
    if(pressedTimes >= 500 && pressedTimes < 1000) {
        content->setStyleSheet("QLabel#Content { background-color: #5DADE2; color: #FDFEFE; font-size: 70px; }"); //blue bg color
    }
    if(pressedTimes >= 1000 && pressedTimes < 5000) {
        content->setStyleSheet("QLabel#Content { background-color: #7D3C98; color: #FDFEFE; font-size: 70px; }"); //purple bg color
    }
    if(pressedTimes >= 5000) {
        content->setStyleSheet("QLabel#Content { background-color: #E74C3C; color: #FDFEFE; font-size: 70px; }"); //red bg color
    }
    if(pressedTimes >= 10000) {
        content->setStyleSheet("QLabel#Content { background-color: #A93226; color: #FDFEFE; font-size: 70px; }"); //deep red bg color
    }
}

void TotalPressed::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void TotalPressed::reloadData(int index)
{
    totalPressedTimes = Database::returnTotalPressedTimes(index);
    content->setText(QString::number(totalPressedTimes));
    setContColor(totalPressedTimes);
}

void TotalPressed::keyPressed(QString)
{
    totalPressedTimes++;
    content->setText(QString::number(totalPressedTimes));
    setContColor(totalPressedTimes);
}
