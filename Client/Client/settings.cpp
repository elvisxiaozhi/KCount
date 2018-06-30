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
    createLimitsLayout();
}

void Settings::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #eeeeee; }"
                ".QToolButton { background-color: #3498DB; font-size: 15px; color: white; border-radius: 2px; border: 2px solid #FF5A5F; padding: 3px 5px; margin: 5px 2px; }"
                ".QToolButton:hover { background-color: #BB8FCE; font-size: 17px; }"
                ".QToolButton:pressed { background-color: #EC7063 }"
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

    tabWidget = new QTabWidget(scrollWidget);
    tabWidget->hide();

    mainVLayout->addWidget(scrollArea);
    this->setLayout(mainVLayout);

    scrollArea->setWidget(scrollWidget);  //needs to be in the last
}

void Settings::createLimitsLayout()
{
    limitsBtn = new QToolButton(scrollWidget);
    limitsBtn->setMinimumSize(970, 10);
    limitsBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    limitsBtn->setIcon(QIcon(":/Resources/Icons/down-arrow.png"));
    limitsBtn->setText("App Limits");
    limitsBtn->setLayoutDirection(Qt::RightToLeft);
    limitsBtn->setCheckable(true);

    scrollVLayout->addWidget(limitsBtn);
    scrollVLayout->addWidget(tabWidget); //tab widget needs to be added under the limitsBtn

    connect(limitsBtn, &QToolButton::clicked, [this](bool checked) {
        if(checked) {
            limitsBtn->setIcon(QIcon(":/Resources/Icons/up-arrow.png"));
            tabWidget->show();
        }
        else {
            limitsBtn->setIcon(QIcon(":/Resources/Icons/down-arrow.png"));
            tabWidget->hide();
        }
    });
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
