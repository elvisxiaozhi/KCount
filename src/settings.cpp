#include "settings.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
#include "notification.h"

QSettings Settings::startOnBootSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    setWindowStyleSheet();
    setWindowLayout();
    scrollVLayout->addWidget(createToolBtn("App Limits"));

    appLimits = new AppLimits(this); //after creating tool btn, make create its widget
    appLimits->createMainLayout();
    scrollVLayout->addWidget(appLimits);

    scrollVLayout->addWidget(createToolBtn("About Nana"));

//    connect(this, &Settings::delBtnClicked, this, &Settings::deleteBtnClicked);
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
    scrollVLayout->setSpacing(0);
    scrollVLayout->setContentsMargins(30, 0, 0, 0);

    scrollWidget->setLayout(scrollVLayout);

    mainVLayout->addWidget(scrollArea);
    this->setLayout(mainVLayout);

    scrollArea->setWidget(scrollWidget);  //needs to be in the last
}

QToolButton *Settings::createToolBtn(QString name)
{
    QToolButton *toolBtn = new QToolButton(scrollWidget);
    toolBtn->setMinimumSize(970, 10);
    toolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolBtn->setIcon(QIcon(":/icons/down-arrow.png"));
    toolBtn->setText(name);
    toolBtn->setLayoutDirection(Qt::RightToLeft);
    toolBtn->setCheckable(true);
    btnVec.push_back(toolBtn);

    connect(toolBtn, &QToolButton::clicked, [this, toolBtn](bool checked){ showToolBtn(toolBtn->text(), checked); });

    return toolBtn;
}

//void Settings::createLimitsBottomWidget()
//{
//    QHBoxLayout *limitsAddHLayout = new QHBoxLayout();

//    limitedAddBtn = new QPushButton(limitsWidget);
//    limitedAddBtn->setText("Add");
//    limitedAddBtn->setObjectName("LimitsBottomBtn");

//    okBtn = new QPushButton(limitsWidget);
//    okBtn->setText("OK");
//    okBtn->hide();
//    okBtn->setObjectName("LimitsBottomBtn");

//    cancelBtn = new QPushButton(limitsWidget);
//    cancelBtn->setText("Cancel");
//    cancelBtn->hide();
//    cancelBtn->setObjectName("LimitsBottomBtn");

//    QSpacerItem *leftItem = new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
//    limitsAddHLayout->addSpacerItem(leftItem);
//    limitsAddHLayout->addWidget(limitedAddBtn);
//    limitsAddHLayout->addStretch();
//    limitsAddHLayout->addWidget(okBtn);
//    limitsAddHLayout->addWidget(cancelBtn);
//    limitsBottomVLayout->addLayout(limitsAddHLayout);

//    limitsTabVLayout->addStretch();
//    limitsTabVLayout->addLayout(limitsBottomVLayout);

//    connect(limitedAddBtn, &QPushButton::clicked, this, &Settings::limitedAddBtnClicked);
//    connect(okBtn, &QPushButton::clicked, this, &Settings::okBtnClicked);
//    connect(cancelBtn, &QPushButton::clicked, [this](){ okBtn->hide(); cancelBtn->hide(); limitedAddBtn->show(); removeLimitsListWidget(); updateLimitsWidget(false); });
//}

void Settings::removeLimitsListWidget()
{
    delete limitedListWidget; //delete the parent, the children will be deleted as well
    lineEditVec.clear();
    deleteBtnVec.clear();
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

void Settings::limitsBtnClicked(QString name, bool checked)
{
    for(int i = 0; i < btnVec.size(); ++i) {
        if(btnVec[i]->text() == name) {
            if(checked) {
                btnVec[i]->setIcon(QIcon(":/icons/up-arrow.png"));
                appLimits->show();
                appLimits->updateLimitedList(false);
            }
            else {
                btnVec[i]->setIcon(QIcon(":/icons/down-arrow.png"));
                appLimits->hide();
                appLimits->removeLimitedList();
            }
        }
    }
}

void Settings::deleteBtnClicked(int index)
{
    int i = 0;
    for(auto mapKey : Notification::xmlMap.keys()) {
        if(i == index) {
            if(Notification::xmlMap.value(mapKey) == "False") {
                Notification::deleteRegKey(lineEditVec[index]->text());
            }
            else {
                Notification::deleteRegValue(lineEditVec[index]->text());
            }
            Notification::xmlMap.remove(mapKey);
        }
        ++i;
    }

    Notification::writeXml();

    removeLimitsListWidget();
//    updateLimitsWidget(false);
}

void Settings::okBtnClicked()
{
    okBtn->hide();
    cancelBtn->hide();
    limitedAddBtn->show();

    Notification::createRegistry(lineEditVec[lineEditVec.size() - 1]->text());
    removeLimitsListWidget();
//    updateLimitsWidget(false);
}

void Settings::showToolBtn(QString name, bool checked)
{
    if(name == "App Limits") {
        limitsBtnClicked(name, checked);
    }
}
