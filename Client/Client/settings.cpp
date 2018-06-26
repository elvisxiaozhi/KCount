#include "settings.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

QSettings Settings::startOnBootSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    setWindowStyleSheet();
    setWindowLayout();
}

void Settings::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #eeeeee; }"
                );
}

void Settings::setWindowLayout()
{
    mainVLayout = new QVBoxLayout(this);

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Window);
    scrollArea->setFrameShadow(QFrame::Plain);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(1000, 450);

    scrollWidget = new QWidget(this);

    scrollVLayout = new QVBoxLayout;

    scrollWidget->setLayout(scrollVLayout);

    mainVLayout->addWidget(scrollArea);
    this->setLayout(mainVLayout);

    scrollArea->setWidget(scrollWidget);  //needs to be in the last
}

void Settings::paintEvent(QPaintEvent *event)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    //paint the title
    painter.setPen(QColor(255,115,115));
    painter.setFont(QFont("Futura", 20));
    painter.drawText(QRect(50, 50, event->rect().width(), event->rect().height()), "Settings");
}
