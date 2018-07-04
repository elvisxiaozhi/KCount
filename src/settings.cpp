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
//    createLimitsWidget(); //after creating tool btn, make create its widget

    appLimits = new AppLimits(this);
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
    limitsTabVLayout->addWidget(createLimitCBLayout());
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

QWidget *Settings::createLimitCBLayout()
{
    QWidget *mWidget = new QWidget(limitedTab);
    mWidget->setStyleSheet("QWidget { background-color: white; }");

    QHBoxLayout *hLayout = new QHBoxLayout(mWidget);

    limitsCheckBox = new QCheckBox(mWidget);
    limitsCheckBox->setChecked(true);

    QLabel *label = new QLabel(mWidget);
    label->setText(tr("Limits app usage when reaches: "));

    limitsEdit = new QLineEdit(mWidget);
    limitsEdit->setText("3");
    limitsEdit->setFixedSize(20, 20);
    limitsEdit->setValidator(new QIntValidator(0, 24, limitsEdit));

    QLabel *labelUnit = new QLabel(mWidget);
    labelUnit->setText(tr("hour(s)"));

    hLayout->addWidget(limitsCheckBox);
    hLayout->addWidget(label);
    hLayout->addWidget(limitsEdit);
    hLayout->addWidget(labelUnit);
    hLayout->addStretch();
    mWidget->setLayout(hLayout);

    return mWidget;
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

void Settings::limitsBtnClicked(QString name, bool checked)
{
    for(int i = 0; i < btnVec.size(); ++i) {
        if(btnVec[i]->text() == name) {
            if(checked) {
                btnVec[i]->setIcon(QIcon(":/icons/up-arrow.png"));
                appLimits->show();
//                updateLimitsWidget(false);
            }
            else {
                btnVec[i]->setIcon(QIcon(":/icons/down-arrow.png"));
                appLimits->hide();
//                removeLimitsListWidget();
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

void Settings::showToolBtn(QString name, bool checked)
{
    if(name == "App Limits") {
        limitsBtnClicked(name, checked);
    }
}
