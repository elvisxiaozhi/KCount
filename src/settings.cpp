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
    createLimitsLayout();
    createLimitsWidget();

    connect(limitsBtn, &QToolButton::clicked, this, &Settings::limitsBtnClicked);
    connect(this, &Settings::delBtnClicked, this, &Settings::deleteBtnClicked);
}

void Settings::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #eeeeee; }"
                ".QToolButton { background-color: #3498DB; font-size: 15px; color: white; border-radius: 2px; border: 2px solid #FF5A5F; padding: 3px 5px; margin: 5px 2px; }"
                ".QToolButton:hover { background-color: #BB8FCE; font-size: 17px; }"
                ".QToolButton:pressed { background-color: #EC7063 }"
                "QWidget#LimitsWidget, #Tab { background-color: white; }"
                "QTabBar::tab { background-color: #E8F8F5; margin-right: 5px; min-width: 50px; padding: 10px 20px; }"
                "QTabBar::tab:selected { background: #007ACC; color: #fff; border-top: 2px solid #F1C40F; border-left: 2px solid #F1C40F; border-right: 2px solid #F1C40F; }"
                "QTabBar::tab:hover { font: bold; background-color: #BB8FCE; }"
                "QTabBar::tab:pressed { background-color: #EC7063 }"
                "QTabWidget::pane { border: 2px solid #FF5A5F; }"
                "QPushButton#LimitsBottomBtn { background-color: #f0f8ff; font-size: 15px; border-radius: 2px; border: 1px solid #808080; padding: 6px 10px; margin: 5px 2px;}"
                "QPushButton#LimitsBottomBtn:hover { border: 2px solid #111111; }"
                "QPushButton#LimitsBottomBtn:pressed { background-color: #EC7063 }"
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

void Settings::createLimitsLayout()
{
    limitsBtn = new QToolButton(scrollWidget);
    limitsBtn->setMinimumSize(970, 10);
    limitsBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    limitsBtn->setIcon(QIcon(":/icons/down-arrow.png"));
    limitsBtn->setText("App Limits");
    limitsBtn->setLayoutDirection(Qt::RightToLeft);
    limitsBtn->setCheckable(true);

    scrollVLayout->addWidget(limitsBtn);
}

void Settings::createLimitsWidget()
{
    limitsWidget = new QWidget(scrollWidget);
    limitsWidget->setObjectName("LimitsWidget");
    limitsWidget->hide();

    limitsVLayout = new QVBoxLayout(limitsWidget);

    tabWidget = new QTabWidget(limitsWidget);

    limitedTab = new QWidget(tabWidget);
    limitedTab->setObjectName("Tab");
    allowedTab = new QWidget(tabWidget);
    allowedTab->setObjectName("Tab");

    tabWidget->addTab(limitedTab, "Limited");
    tabWidget->addTab(allowedTab, "Allowed");

    limitsVLayout->addWidget(tabWidget);
    limitsWidget->setLayout(limitsVLayout);
    scrollVLayout->addWidget(limitsWidget);

    createLimitsTabConts();
}

void Settings::createLimitsTabConts()
{
    limitsTabVLayout = new QVBoxLayout(limitedTab);

    QLabel *limitsLbl = new QLabel(limitedTab);
    limitsLbl->setText(tr("Limited apps down below will be cleared in the next day"));
    limitsLbl->setAlignment(Qt::AlignCenter);
    limitsLbl->setFixedHeight(30);
    limitsLbl->setStyleSheet("QLabel { background-color: white; }");

    listsVLayout = new QVBoxLayout();

    limitsBottomVLayout = new QVBoxLayout();

    limitsTabVLayout->addWidget(limitsLbl);
    limitsTabVLayout->addLayout(listsVLayout);

    createLimitsBottomWidget(); //create limits tab bottom buttons
}

void Settings::createLimitsBottomWidget()
{
    QHBoxLayout *limitsAddHLayout = new QHBoxLayout();

    limitedAddBtn = new QPushButton(limitsWidget);
    limitedAddBtn->setText("Add");
    limitedAddBtn->setObjectName("LimitsBottomBtn");

    okBtn = new QPushButton(limitsWidget);
    okBtn->setText("OK");
    okBtn->hide();
    okBtn->setObjectName("LimitsBottomBtn");

    cancelBtn = new QPushButton(limitsWidget);
    cancelBtn->setText("Cancel");
    cancelBtn->hide();
    cancelBtn->setObjectName("LimitsBottomBtn");

    QSpacerItem *leftItem = new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
    limitsAddHLayout->addSpacerItem(leftItem);
    limitsAddHLayout->addWidget(limitedAddBtn);
    limitsAddHLayout->addStretch();
    limitsAddHLayout->addWidget(okBtn);
    limitsAddHLayout->addWidget(cancelBtn);
    limitsBottomVLayout->addLayout(limitsAddHLayout);

    limitsTabVLayout->addStretch();
    limitsTabVLayout->addLayout(limitsBottomVLayout);

    connect(limitedAddBtn, &QPushButton::clicked, this, &Settings::limitedAddBtnClicked);
    connect(okBtn, &QPushButton::clicked, this, &Settings::okBtnClicked);
    connect(cancelBtn, &QPushButton::clicked, [this](){ okBtn->hide(); cancelBtn->hide(); limitedAddBtn->show(); removeLimitsListWidget(); updateLimitsWidget(false); });
}

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

void Settings::updateLimitsWidget(bool clicked)
{
    limitedListWidget = new QWidget(limitedTab);
    limitedListWidget->setObjectName("LimitsWidget");
    listsVLayout->addWidget(limitedListWidget);

    QVBoxLayout *limitedListVLayout = new QVBoxLayout();
    limitedListWidget->setLayout(limitedListVLayout);

    Notification::readXml();

    for(auto mapKey : Notification::xmlMap.keys()) {
        QHBoxLayout *hLayout = new QHBoxLayout();

        QLineEdit *lineEdit = new QLineEdit(limitedListWidget);
        lineEdit->setText(mapKey);
        lineEdit->setFixedHeight(25);
        lineEdit->setStyleSheet("QLineEdit { background-color: white; }"
                                "QLineEdit:focus { border: 2px solid #FF5A5F; }");
        lineEditVec.push_back(lineEdit);

        QPushButton *btn = new QPushButton(limitedListWidget);
        btn->setText("Delete");
        btn->setObjectName(QString::number(std::distance(Notification::xmlMap.begin(), Notification::xmlMap.find(mapKey))));
        btn->setStyleSheet("QPushButton { background-color: #f0f8ff; font-size: 15px; border-radius: 2px; border: 1px solid #808080; padding: 5px 5px; margin: 5px 2px;}"
                           ".QPushButton:hover { background-color: #AAAAAA; font-size: 16px; }"
                           ".QPushButton:pressed { background-color: #EC7063 }");
        deleteBtnVec.push_back(btn);

        hLayout->addWidget(lineEdit);
        hLayout->addWidget(btn);

        limitedListVLayout->addLayout(hLayout);

        connect(btn, &QPushButton::clicked, [this, btn](){ emit delBtnClicked(btn->objectName().toInt()); });
    }

    if(clicked) {
        QHBoxLayout *hLayout = new QHBoxLayout();

        QLineEdit *lineEdit = new QLineEdit(limitedListWidget);
        lineEdit->setText("");
        lineEdit->setFixedSize(837, 25);
        lineEdit->setFocus(Qt::OtherFocusReason);
        lineEdit->setStyleSheet("QLineEdit { background-color: white; }"
                                "QLineEdit:focus { border: 2px solid #FF5A5F; }");
        lineEditVec.push_back(lineEdit);

        hLayout->addWidget(lineEdit);
        hLayout->addStretch();

        limitedListVLayout->addLayout(hLayout);
    }

    limitedListVLayout->addStretch();
}

void Settings::limitsBtnClicked(bool checked)
{
    if(checked) {
        limitsBtn->setIcon(QIcon(":/icons/up-arrow.png"));
        limitsWidget->show();

        updateLimitsWidget(false);
    }
    else {
        limitsBtn->setIcon(QIcon(":/icons/down-arrow.png"));
        limitsWidget->hide();

        removeLimitsListWidget();
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
    updateLimitsWidget(false);
}

void Settings::limitedAddBtnClicked()
{
    okBtn->show();
    cancelBtn->show();
    limitedAddBtn->hide();

    removeLimitsListWidget();
    updateLimitsWidget(true);
}

void Settings::okBtnClicked()
{
    okBtn->hide();
    cancelBtn->hide();
    limitedAddBtn->show();

    Notification::createRegistry(lineEditVec[lineEditVec.size() - 1]->text());
    removeLimitsListWidget();
    updateLimitsWidget(false);
}
