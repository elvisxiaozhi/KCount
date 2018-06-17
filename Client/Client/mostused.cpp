#include "mostused.h"
#include <QLabel>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include "signalemitter.h"
#include "database.h"
#include <QCoreApplication>

MostUsed::MostUsed(QWidget *parent, int mode) : QWidget(parent)
{
    timer.start();
    mostUsedVec = Database::returnAppVec(mode);
    QStringList currentAppName = qApp->applicationFilePath().split("/");
    lastAppName = QString(currentAppName.at(currentAppName.size() - 1)).split(".exe").first();

    setMainLayout();
    setWindowStyleSheet();

    connect(Emitter::Instance(), &SignalEmitter::appChanged, this, &MostUsed::appChanged);
    connect(showMoreBtn, &CustomButton::clicked, [this](){ showMoreBtn->hide(); showLessBtn->show(); scrollArea->show(); contWidget->hide(); });
    connect(showLessBtn, &CustomButton::clicked, [this](){ showMoreBtn->show(); showLessBtn->hide(); scrollArea->hide(); contWidget->show(); });
}

//setData is called whenever the time span icon clicked or over view page is clicked
void MostUsed::setData()
{
    std::sort(mostUsedVec.begin(), mostUsedVec.end(),
              [](const std::pair<QString, unsigned long int> &a, const std::pair<QString, unsigned long int> &b){ return a.second > b.second; });

    setContents();
    createScrollConts();
}

void MostUsed::setMainLayout()
{
    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);

    contWidget = new QWidget(this);
    contWidget->setFixedHeight(200);

    contVLayout = new QVBoxLayout;
    contVLayout->setContentsMargins(0, 0, 0, 0);
    contVLayout->setSpacing(0);

    contWidget->setLayout(contVLayout);

    QLabel *title = new QLabel(this);
    title->setText("Most Used");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    showMoreBtn = new CustomButton(this);
    showMoreBtn->setIcon(QIcon(":/Resources/Icons/show_more.png"));

    showLessBtn = new CustomButton(this);
    showLessBtn->setIcon(QIcon(":/Resources/Icons/show_less.png"));
    showLessBtn->hide();

    headerHLayout = new QHBoxLayout();

    QSpacerItem *spacerItem = new QSpacerItem(45, 1, QSizePolicy::Fixed, QSizePolicy::Fixed); //used to fix the title in the absolute center

    headerHLayout->addSpacerItem(spacerItem);
    headerHLayout->addWidget(title);
    headerHLayout->addWidget(showMoreBtn);
    headerHLayout->addWidget(showLessBtn);

    contents.resize(5);
    for(int i = 0; i < contents.size(); ++i) {
        contents[i] = new Label(0, 20);
        contents[i]->setFixedHeight(40);
        contVLayout->addWidget(contents[i]);
    }
    setContents();

    createScrollWidget();

    mainVLayout->addLayout(headerHLayout);
    mainVLayout->addWidget(contWidget);
    mainVLayout->addWidget(scrollArea);
}

void MostUsed::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background: #7FFFD4; }"
                "QLabel#Title { font: 20px; color: #666666; }"
                );
}

void MostUsed::setContents()
{
    if(mostUsedVec.isEmpty()) {
        for(int i = 0; i < 5; ++i) {
            contents[i]->setText("");
            contents[i]->setAppUsageLblColor(0);
        }
    }
    if(mostUsedVec.size() > 5) {
        for(int i = 0; i < 5; ++i) {
            setLblText(contents[i], mostUsedVec[i].first, mostUsedVec[i].second);
            contents[i]->setAppUsageLblColor(mostUsedVec[i].second);
        }
    }
    else {
        for(int i = 0; i < mostUsedVec.size(); ++i) {
            setLblText(contents[i], mostUsedVec[i].first, mostUsedVec[i].second);
            contents[i]->setAppUsageLblColor(mostUsedVec[i].second);
        }
        for(int i = 0; i < 5 - mostUsedVec.size(); ++i) {
            contents[4 - i]->setText(""); //note the 4 - i
            contents[4 - i]->setAppUsageLblColor(0);
        }
    }
}

void MostUsed::setLblText(Label *label, QString appName, int usedTime)
{
    if(usedTime / 3600 > 0) {
        label->setText(appName + ": " + QString::number(usedTime / 3600) + "h " + QString::number((usedTime % 3600) / 60) + "m");
    }
    else {
        label->setText(appName + ": " + QString::number(usedTime / 60) + "m " + QString::number(usedTime % 60) + "s");
    }
}

void MostUsed::createScrollWidget()
{
    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Window);
    scrollArea->setFrameShadow(QFrame::Plain);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->hide();

    scrollWidget = new QWidget(this);

    scrollContVLayout = new QVBoxLayout();
    scrollContVLayout->setContentsMargins(0, 0, 0, 0);
    scrollContVLayout->setSpacing(0);

    scrollWidget->setLayout(scrollContVLayout);
    scrollArea->setWidget(scrollWidget);

    createScrollConts();
}

void MostUsed::createScrollConts()
{
    for(int i = 0; i < mostUsedVec.size() - scrollConts.size(); ++i) {
        Label *contLbl = new Label(0, 20);
        scrollContVLayout->addWidget(contLbl);
        scrollConts.push_back(contLbl);
    }

    for(int i = 0; i < scrollConts.size(); ++i) {
        scrollConts[i]->setAppUsageLblColor(mostUsedVec[i].second);

        QString appName = mostUsedVec[i].first;
        int usedTime = mostUsedVec[i].second;
        if(usedTime / 3600 > 0) {
            scrollConts[i]->setText(appName + ": " + QString::number(usedTime / 3600) + "h " + QString::number((usedTime % 3600) / 60) + "m");
        }
        else {
            scrollConts[i]->setText(appName + ": " + QString::number(usedTime / 60) + "m " + QString::number(usedTime % 60) + "s");
        }
    }
}

bool MostUsed::hasAppReachedLimit()
{
    std::sort(mostUsedVec.begin(), mostUsedVec.end(),
              [](const std::pair<QString, unsigned long int> &a, const std::pair<QString, unsigned long int> &b){ return a.second > b.second; });

    if(mostUsedVec[0].second >= 10800) {
        if(std::find(alertedApp.begin(), alertedApp.end(), mostUsedVec[0].first) != alertedApp.end()) {
            return false;
        }
        else {
            alertedApp.push_back(mostUsedVec[0].first);
            return true;
        }
    }

    return false;
}

void MostUsed::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void MostUsed::updateDatabase()
{
    Database::updateUsedAppToDB(tempAppMap);
    tempAppMap.clear(); //after updating datebase, clear the map, so it can store new data for the next hour
}

void MostUsed::appChanged(QString processName)
{
    QRegExp regEx("\\\\");
    QStringList processNameList = processName.split(regEx);
    QString appName = QString(processNameList.at(processNameList.size() - 1)).split(".exe").first();
    int elapsedTime = timer.elapsed();
    int usedTime = 0;
    if(timer.elapsed() % 1000 >= 500) {
        usedTime = std::ceil(elapsedTime / 1000);
    }
    else {
        usedTime = std::floor(elapsedTime / 1000);
    }

    if(tempAppMap.contains(lastAppName)) {
        int newValue = tempAppMap.value(lastAppName) + usedTime;
        tempAppMap.insert(lastAppName, newValue);
    }
    else {
        tempAppMap.insert(lastAppName, usedTime);
    }

    auto it = std::find_if(
                mostUsedVec.begin(), mostUsedVec.end(),
                [this](const std::pair<QString, int> &element){ return element.first == lastAppName; }
                );
    if(it != mostUsedVec.end()) {
        (*it).second += usedTime;
    }
    else {
        mostUsedVec.push_back(std::make_pair(lastAppName, usedTime));
    }

    if(hasAppReachedLimit()) {
        emit limitAppAlert(mostUsedVec[0].first);
    }

    timer.start(); //output first, then restart the timer
    lastAppName = appName;
}
