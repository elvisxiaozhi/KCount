#include "settings.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QLabel>
#include <QXmlStreamReader>
#include <QFile>
#include "database.h"
#include <QHBoxLayout>

QSettings Settings::startOnBootSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    setWindowStyleSheet();
    setWindowLayout();
    createLimitsLayout();
    createLimitsWidget();

    connect(limitsBtn, &QToolButton::clicked, [this](bool checked) {
        if(checked) {
            limitsBtn->setIcon(QIcon(":/Resources/Icons/up-arrow.png"));
            limitsWidget->show();
            updateLimitsWidget();
        }
        else {
            limitsBtn->setIcon(QIcon(":/Resources/Icons/down-arrow.png"));
            limitsWidget->hide();
            delete limitedListVLayout;
            for(int i = 0; i < lineEditVec.size(); ++i) {
                delete lineEditVec[i];
                delete deleteBtnVec[i];
            }
            lineEditVec.clear();
            deleteBtnVec.clear();
            delete limitsAddHLayout;
        }
    });
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
    limitsBtn->setIcon(QIcon(":/Resources/Icons/down-arrow.png"));
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

    limitsTabVLayout->addWidget(limitsLbl);
}

void Settings::readXml()
{
    limitedAppVec.clear();

    QXmlStreamReader xmlReader;
    QFile file(Database::dataPath + "/LimitedApp.xml");
    file.open(QIODevice::ReadOnly);
    xmlReader.setDevice(&file);
    QString appName;
    while(!xmlReader.atEnd()) {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartElement) {
            if(xmlReader.name() == "Name") {
                appName = xmlReader.readElementText();
                limitedAppVec.push_back(appName);
            }
        }
    }
    file.close();
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

void Settings::updateLimitsWidget()
{
    limitedListVLayout = new QVBoxLayout();
    limitsTabVLayout->addLayout(limitedListVLayout);

    readXml();

    for(int i = 0; i < limitedAppVec.size(); ++i) {
        QHBoxLayout *hLayout = new QHBoxLayout();

        QLineEdit *lineEdit = new QLineEdit(limitsWidget);
        lineEdit->setText(limitedAppVec[i]);
        lineEdit->setStyleSheet("QLineEdit { background-color: white; }");
        lineEditVec.push_back(lineEdit);

        QPushButton *btn = new QPushButton(limitsWidget);
        btn->setText("Delete");
        deleteBtnVec.push_back(btn);

        hLayout->addWidget(lineEdit);
        hLayout->addWidget(btn);

        limitedListVLayout->addLayout(hLayout);
    }

    limitedListVLayout->addStretch();

    limitsAddHLayout = new QHBoxLayout;

    QPushButton *limitedAddBtn = new QPushButton(limitsWidget);
    limitedAddBtn->setText("Add");

    limitsAddHLayout->addWidget(limitedAddBtn);
    limitsAddHLayout->addStretch();
    limitedListVLayout->addLayout(limitsAddHLayout);
}
