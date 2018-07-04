#include "applimits.h"
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QHBoxLayout>

AppLimits::AppLimits(QWidget *parent) : QWidget(parent)
{
    setWidgetStyleSheet();
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
                "QPushButton:hover { border: 2px solid #111111; }"
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
    checkBox->setChecked(true);

    QLabel *mLbl = new QLabel(mWidget);
    mLbl->setText(tr("Limits app usage when reaches: "));

    limitsEdit = new QLineEdit(mWidget);
    limitsEdit->setText("3");
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
}

void AppLimits::paintEvent(QPaintEvent *)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
