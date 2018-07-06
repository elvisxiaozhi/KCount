#include "settings.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>

QSettings Settings::startOnBootSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
QSettings Settings::settings("My Computer", "Nana"); //must set computer and app names to initiate QSettings

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    setWindowStyleSheet();
    setWindowLayout();
    createToolBtns();
}

void Settings::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #eeeeee; }"
                ".QToolButton { background-color: #3498DB; font-size: 15px; color: white; border-radius: 2px; border: 2px solid #FF5A5F; padding: 3px 5px; margin: 5px 2px; }"
                ".QToolButton:hover { background-color: #BB8FCE; font-size: 17px; }"
                ".QToolButton:pressed { background-color: #EC7063 }"
                "QScrollBar:vertical { background: #faebd7; width: 4px; }"
                "QScrollBar::handle:vertical { background: #ff7373; }"
                );
}

void Settings::setWindowLayout()
{
    mainVLayout = new QVBoxLayout(this);

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Window);
    scrollArea->setFrameShadow(QFrame::Plain);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(1000, 450);

    scrollWidget = new QWidget(this);

    scrollVLayout = new QVBoxLayout;
    scrollVLayout->setSpacing(0);
    scrollVLayout->setContentsMargins(30, 0, 0, 0);

    scrollWidget->setLayout(scrollVLayout);

    mainVLayout->addWidget(scrollArea);
    setLayout(mainVLayout);

    scrollArea->setWidget(scrollWidget);  //needs to be in the last
}

QToolButton *Settings::createToolBtn(QString name)
{
    QToolButton *toolBtn = new QToolButton(scrollWidget);
    toolBtn->setFixedSize(970, 40);
    toolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBtn->setIcon(QIcon(":/icons/down-arrow.png"));
    toolBtn->setText(name);
    toolBtn->setLayoutDirection(Qt::RightToLeft);
    toolBtn->setCheckable(true);
    btnVec.push_back(toolBtn);

    connect(toolBtn, &QToolButton::clicked, [this, toolBtn](bool checked){ toolBtnClicked(toolBtn->text(), checked); });

    return toolBtn;
}

void Settings::createToolBtns()
{
    scrollVLayout->addWidget(createToolBtn("App Limits"));

    appLimits = new AppLimits(this); //after creating tool btn, make create its widget
    appLimits->createMainLayout();
    scrollVLayout->addWidget(appLimits);

    scrollVLayout->addWidget(createToolBtn("Reset"));
    reset = new Reset(this);
    scrollVLayout->addWidget(reset);

    scrollVLayout->addWidget(createToolBtn("About Nana"));

    about = new About(this);
    scrollVLayout->addWidget(about);

    scrollVLayout->addStretch();
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

void Settings::toolBtnClicked(QString name, bool checked)
{
    for(int i = 0; i < btnVec.size(); ++i) {
        if(btnVec[i]->text() == name) {
            if(checked) {
                btnVec[i]->setIcon(QIcon(":/icons/up-arrow.png"));
            }
            else {
                btnVec[i]->setIcon(QIcon(":/icons/down-arrow.png"));
            }
        }
    }

    if(name == "App Limits") {
        if(checked) {
            appLimits->show();
            appLimits->updateLimitedList(false);
        }
        else {
            appLimits->hide();
            appLimits->removeLimitedList();
        }
    }
    if(name == "Reset") {
        if(checked) {
            reset->show();
        }
        else {
            reset->hide();
        }
    }
    if(name == "About Nana") {
        if(checked) {
            about->show();
        }
        else {
            about->hide();
        }
    }
}
