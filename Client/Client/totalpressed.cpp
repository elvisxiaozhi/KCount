#include "totalpressed.h"
#include <QStyleOption>
#include <QPainter>
#include <QPaintEvent>

TotalPressed::TotalPressed(QWidget *parent) : QWidget(parent)
{
    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);

    title = new QLabel(this);
    title->setText("Total Pressed");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    content = new QLabel(this);
    content->setText(QString::number(0));
    content->setObjectName("Content");
    content->setAlignment(Qt::AlignCenter);
    content->setFixedHeight(200);

    mainVLayout->addWidget(title);
    mainVLayout->addWidget(content);

    setWindowStyleSheet();
}

void TotalPressed::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #b0e0e6; }"
                "QLabel#Title { font: 20px; color: #666666; }"
                "QLabel#Content { font: 70px; color: #FDFEFE; background-color: #FAD7A0; }"
                );
}

void TotalPressed::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
