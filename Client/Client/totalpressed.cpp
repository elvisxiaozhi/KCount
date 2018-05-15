#include "totalpressed.h"
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include "signalemitter.h"
#include <QDebug>
#include "database.h"

TotalPressed::TotalPressed(QWidget *parent)
    : QWidget(parent), totalPressedTimes(Database::returnTotalPressedTimes())
{
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

void TotalPressed::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void TotalPressed::keyPressed(QString)
{
    totalPressedTimes++;
    content->setText(QString::number(totalPressedTimes));
}
