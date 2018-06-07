#include "mostused.h"
#include <QLabel>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include "signalemitter.h"
#include "database.h"

MostUsed::MostUsed(QWidget *parent) : QWidget(parent)
{
    timer.start();
    mostUsedVec = Database::returnAppVec(1);

    setWindowStyleSheet();

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);
    contVLayout = new QVBoxLayout;
    contVLayout->setSpacing(0);

    QLabel *title = new QLabel(this);
    title->setText("Most Used");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    contents.resize(5);
    for(int i = 0; i < contents.size(); ++i) {
        contents[i] = new Label(0, 20);
        contents[i]->setFixedHeight(40);
        contVLayout->addWidget(contents[i]);
    }
    setContents();

    mainVLayout->addWidget(title);
    mainVLayout->addLayout(contVLayout);

    connect(Emitter::Instance(), &SignalEmitter::appChanged, this, &MostUsed::appChanged);
}

void MostUsed::setData()
{
    std::sort(mostUsedVec.begin(), mostUsedVec.end(),
              [](const std::pair<QString, unsigned long int> &a, const std::pair<QString, unsigned long int> &b){ return a.second > b.second; });

    setContents();
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
//    qDebug() <<  mostUsedVec[0].second << QString::number(mostUsedVec[0].second % 60) << mostUsedVec[0].second / 60;
    if(mostUsedVec.isEmpty()) {
        for(int i = 0; i < 5; ++i) {
            contents[i]->setText("");
            contents[i]->setLabelColor(0);
        }
    }
    if(mostUsedVec.size() > 5) {
        for(int i = 0; i < 5; ++i) {
            contents[i]->setText(mostUsedVec[i].first + ": " + QString::number(mostUsedVec[i].second));
            contents[i]->setLabelColor(mostUsedVec[i].second);
        }
    }
    else {
        for(int i = 0; i < mostUsedVec.size(); ++i) {
            contents[i]->setText(mostUsedVec[i].first + ": " + QString::number(mostUsedVec[i].second));
            contents[i]->setLabelColor(mostUsedVec[i].second);
        }
        for(int i = 0; i < 5 - mostUsedVec.size(); ++i) {
            contents[4 - i]->setText(""); //note the 4 - i
            contents[4 - i]->setLabelColor(0);
        }
    }
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

void MostUsed::reloadData(int index)
{
    updateDatabase();
    mostUsedVec.clear();
    mostUsedVec = Database::returnAppVec(index);
    setContents();
}

void MostUsed::appChanged(QString processName)
{
    QRegExp regEx("\\\\");
    QStringList processNameList = processName.split(regEx);
    QString appName = processNameList.at(processNameList.size() - 1);
    int elapsedTime = timer.elapsed();
    int usedTime = 0;
    if(timer.elapsed() % 1000 >= 500) {
        usedTime = std::ceil(elapsedTime / 1000);
    }
    else {
        usedTime = std::floor(elapsedTime / 1000);
    }

    if(tempAppMap.contains(appName)) {
        int newValue = tempAppMap.value(appName) + usedTime;
        tempAppMap.insert(appName, newValue);
    }
    else {
        tempAppMap.insert(appName, usedTime);
    }

    auto it = std::find_if(
                mostUsedVec.begin(), mostUsedVec.end(),
                [appName](const std::pair<QString, int> &element){ return element.first == appName; }
                );
    if(it != mostUsedVec.end()) {
        (*it).second += usedTime;
    }
    else {
        mostUsedVec.push_back(std::make_pair(appName, usedTime));
    }

    timer.start(); //output first, then restart the timer
}
