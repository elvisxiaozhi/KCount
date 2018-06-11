#include "totalpressed.h"
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include "signalemitter.h"
#include <QDebug>
#include "database.h"
#include <QSpacerItem>

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

    swicthLbl = new CustomLabel(this);

    lblHLayout = new QHBoxLayout();

    content = new Label(totalPressedTimes, 70);
    content->setText(QString::number(totalPressedTimes));
    content->setFixedHeight(200);

    QSpacerItem *spacerItem = new QSpacerItem(45, 1, QSizePolicy::Fixed, QSizePolicy::Fixed); //used to fix the title in the absolute center

    lblHLayout->addSpacerItem(spacerItem);
    lblHLayout->addWidget(title);
    lblHLayout->addWidget(swicthLbl);
    mainVLayout->addLayout(lblHLayout);
    mainVLayout->addWidget(content);

    setWindowStyleSheet();

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &TotalPressed::keyPressed);
}

void TotalPressed::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #b0e0e6; }"
                "QLabel#Title { font: 20px; color: #666666; }"
                );
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
    content->setLabelColor(totalPressedTimes);
}

void TotalPressed::keyPressed(QString)
{
    totalPressedTimes++;
    content->setText(QString::number(totalPressedTimes));
    content->setLabelColor(totalPressedTimes);
}
