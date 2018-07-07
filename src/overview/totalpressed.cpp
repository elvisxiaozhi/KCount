#include "totalpressed.h"
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>
#include "core/signalemitter.h"
#include <QDebug>
#include "core/database.h"

TotalPressed::TotalPressed(QWidget *parent, int mode, QString lblTitle)
    : QWidget(parent)
{
    totalPressedTimes = Database::returnTotalPressedTimes(mode);

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);

    title = new QLabel(this);
    title->setText(lblTitle);
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    switchBtn = new CustomButton(this);
    switchBtn->setIcon(QIcon(":/icons/switch.png"));

    lblHLayout = new QHBoxLayout();

    content = new Label(totalPressedTimes, 70);
    content->setText(QString::number(totalPressedTimes));
    content->setFixedHeight(200);

    QSpacerItem *spacerItem = new QSpacerItem(45, 1, QSizePolicy::Fixed, QSizePolicy::Fixed); //used to fix the title in the absolute center

    lblHLayout->addSpacerItem(spacerItem);
    lblHLayout->addWidget(title);
    lblHLayout->addWidget(switchBtn);

    mainVLayout->addLayout(lblHLayout);
    mainVLayout->addWidget(content);

    setWindowStyleSheet();

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &TotalPressed::keyPressed);
    connect(switchBtn, &CustomButton::clicked, [this](){ this->hide(); emit switchBtnClicked(); });
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

void TotalPressed::keyPressed(QString)
{
    totalPressedTimes++;
    content->setText(QString::number(totalPressedTimes));
    content->setLabelColor(totalPressedTimes);
}
