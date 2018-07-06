#include "applimits.h"
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>
#include "notification.h"
#include <QDebug>
#include "settings.h"

AppLimits::AppLimits(QWidget *parent) : QWidget(parent)
{
    setWidgetStyleSheet();

    connect(this, &AppLimits::delBtnClicked, this, &AppLimits::delLimitedApp);
}

void AppLimits::createMainLayout()
{
    this->hide();

    mainVLayout = new QVBoxLayout(this);

    tabWidget = new QTabWidget(this);

    limitedTab = new QWidget(tabWidget);
    allowedTab = new QWidget(tabWidget);

    tabWidget->addTab(limitedTab, "Limited");
    tabWidget->addTab(allowedTab, "Allowed");

    mainVLayout->addWidget(tabWidget);
    setLayout(mainVLayout);

    createTabConts();
    createBtmLayout();
}

void AppLimits::setWidgetStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: white; }"
                "QTabBar::tab { background-color: #E8F8F5; margin-right: 5px; min-width: 50px; padding: 10px 20px; }"
                "QTabBar::tab:selected { background: #007ACC; color: #fff; border-top: 2px solid #F1C40F; border-left: 2px solid #F1C40F; border-right: 2px solid #F1C40F; }"
                "QTabBar::tab:hover { font: bold; background-color: #BB8FCE; }"
                "QTabBar::tab:pressed { background-color: #EC7063 }"
                "QTabWidget::pane { border: 2px solid #FF5A5F; }"
                "QPushButton { background-color: #f0f8ff; font-size: 15px; border-radius: 2px; border: 1px solid #808080; padding: 6px 10px; margin: 5px 2px;}"
                "QPushButton:hover { border: 2px solid #111111; margin: 5px 1px; }"
                "QPushButton:pressed { background-color: #EC7063 }"
                );
}

void AppLimits::createTabConts()
{
    tabVLayout = new QVBoxLayout(limitedTab);

    QLabel *mLbl = new QLabel(limitedTab);
    mLbl->setText(tr("Limited apps down below will be cleared in the next day"));
    mLbl->setAlignment(Qt::AlignCenter);
    mLbl->setFixedHeight(30);
    mLbl->setStyleSheet("QLabel { background-color: white; }");

    listsVLayout = new QVBoxLayout();

    btmVLayout = new QVBoxLayout();

    tabVLayout->addWidget(mLbl);
    tabVLayout->addWidget(createCBLayout());
    tabVLayout->addLayout(listsVLayout);
}

QWidget *AppLimits::createCBLayout()
{
    QWidget *mWidget = new QWidget(limitedTab);
    mWidget->setStyleSheet("QWidget { background-color: white; }");

    QHBoxLayout *hLayout = new QHBoxLayout(mWidget);

    checkBox = new QCheckBox(mWidget);

    QLabel *mLbl = new QLabel(mWidget);
    mLbl->setText(tr("Limits app usage when reaches: "));

    limitsEdit = new QLineEdit(mWidget);
    limitsEdit->setFixedSize(20, 20);
    limitsEdit->setValidator(new QIntValidator(0, 24, limitsEdit));

    QLabel *lblUnit = new QLabel(mWidget);
    lblUnit->setText(tr("hour(s)"));

    hLayout->addWidget(checkBox);
    hLayout->addWidget(mLbl);
    hLayout->addWidget(limitsEdit);
    hLayout->addWidget(lblUnit);
    hLayout->addStretch();
    mWidget->setLayout(hLayout);

    initWidgets();

    connect(checkBox, &QCheckBox::clicked, [this](bool checked){ Settings::settings.setValue("Settings/isLimitsOn", checked); });
    connect(limitsEdit, &QLineEdit::textChanged, [this](QString text){ Settings::settings.setValue("Settings/ShowLimitsTime", text); });

    return mWidget;
}

void AppLimits::createBtmLayout()
{
    QHBoxLayout *hLayout = new QHBoxLayout();

    addBtn = new QPushButton(limitedTab);
    addBtn->setText(tr("Add"));

    okBtn = new QPushButton(limitedTab);
    okBtn->setText(tr("OK"));
    okBtn->hide();

    cancelBtn = new QPushButton(limitedTab);
    cancelBtn->setText(tr("Cancel"));
    cancelBtn->hide();

    QSpacerItem *leftItem = new QSpacerItem(10, 1, QSizePolicy::Fixed, QSizePolicy::Fixed);
    hLayout->addSpacerItem(leftItem);
    hLayout->addWidget(addBtn);
    hLayout->addStretch();
    hLayout->addWidget(okBtn);
    hLayout->addWidget(cancelBtn);

    btmVLayout->addLayout(hLayout);
    tabVLayout->addStretch();
    tabVLayout->addLayout(btmVLayout);

    connect(addBtn, &QPushButton::clicked, this, &AppLimits::addBtnClicked);
    connect(okBtn, &QPushButton::clicked, [this](){ Notification::createRegistry(lineEditVec[lineEditVec.size() - 1]->text()); addOrCxlClicked(); });
    connect(cancelBtn, &QPushButton::clicked, this, &AppLimits::addOrCxlClicked);
}

void AppLimits::initWidgets()
{
    if(Settings::settings.value("Settings/isLimitsOn").isValid()) {
        if(Settings::settings.value("Settings/isLimitsOn") == true) {
            checkBox->setChecked(true);
        }
        else {
            checkBox->setChecked(false);
        }
    }
    else {
        checkBox->setChecked(true);
        Settings::settings.setValue("Settings/isLimitsOn", true);
    }

    if(Settings::settings.value("Settings/ShowLimitsTime").isValid()) {
        limitsEdit->setText(Settings::settings.value("Settings/ShowLimitsTime").toString());
    }
    else {
        limitsEdit->setText("3");
        Settings::settings.setValue("Settings/ShowLimitsTime", limitsEdit->text());
    }
}

void AppLimits::updateLimitedList(bool clicked)
{
    limitedList = new QWidget(limitedTab);

    QVBoxLayout *vLayout = new QVBoxLayout();

    Notification::readXml();

    for(auto mapKey : Notification::xmlMap.keys()) {
        QHBoxLayout *hLayout = new QHBoxLayout();

        QLineEdit *lineEdit = new QLineEdit(limitedList);
        lineEdit->setText(mapKey);
        lineEdit->setFixedHeight(25);
        lineEdit->setStyleSheet("QLineEdit { background-color: white; }"
                                "QLineEdit:focus { border: 2px solid #FF5A5F; }");
        lineEditVec.push_back(lineEdit);

        QPushButton *btn = new QPushButton(limitedList);
        btn->setText("Delete");
        btn->setObjectName(QString::number(std::distance(Notification::xmlMap.begin(), Notification::xmlMap.find(mapKey))));
        btn->setStyleSheet("QPushButton { background-color: #f0f8ff; font-size: 15px; border-radius: 2px; border: 1px solid #808080; padding: 5px 5px; margin: 5px 2px;}"
                           ".QPushButton:hover { background-color: #AAAAAA; font-size: 16px; }"
                           ".QPushButton:pressed { background-color: #EC7063 }");
        delBtnVec.push_back(btn);

        hLayout->addWidget(lineEdit);
        hLayout->addWidget(btn);

        vLayout->addLayout(hLayout);

        connect(btn, &QPushButton::clicked, [this, btn](){ emit delBtnClicked(btn->objectName().toInt()); });
    }

    if(clicked) {
        QHBoxLayout *hLayout = new QHBoxLayout();

        QLineEdit *lineEdit = new QLineEdit(limitedList);
        lineEdit->setText("");
        lineEdit->setFixedSize(837, 25);
        lineEdit->setFocus(Qt::OtherFocusReason);
        lineEdit->setStyleSheet("QLineEdit { background-color: white; }"
                                "QLineEdit:focus { border: 2px solid #FF5A5F; }");
        lineEditVec.push_back(lineEdit);

        hLayout->addWidget(lineEdit);
        hLayout->addStretch();

        vLayout->addLayout(hLayout);
    }

    listsVLayout->addStretch();
    limitedList->setLayout(vLayout);
    listsVLayout->addWidget(limitedList);
}

void AppLimits::removeLimitedList()
{
    delete limitedList; //delete the parent, the children will be deleted as well

    lineEditVec.clear();
    delBtnVec.clear();
}

void AppLimits::paintEvent(QPaintEvent *)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void AppLimits::addBtnClicked()
{
    addBtn->hide();
    okBtn->show();
    cancelBtn->show();

    removeLimitedList();
    updateLimitedList(true);
}

void AppLimits::delLimitedApp(int index)
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
    removeLimitedList();
    updateLimitedList(false);
}

void AppLimits::addOrCxlClicked()
{
    okBtn->hide();
    cancelBtn->hide();
    addBtn->show();

    removeLimitedList();
    updateLimitedList(false);
}
