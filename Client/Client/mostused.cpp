#include "mostused.h"
#include <QLabel>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include "signalemitter.h"

MostUsed::MostUsed(QWidget *parent) : QWidget(parent)
{
    timer.start();

    setWindowStyleSheet();

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);
    contVLayout = new QVBoxLayout;
    contVLayout->setSpacing(0);

    QLabel *title = new QLabel(this);
    title->setText("Most Used");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    mainVLayout->addWidget(title);

    connect(Emitter::Instance(), &SignalEmitter::appChanged, this, &MostUsed::appChanged);
}

void MostUsed::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background: #FFE4E1; }"
                "QLabel#Title { font: 20px; color: #666666; }"
                );
}

void MostUsed::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void MostUsed::appChanged(QString processName)
{
    QRegExp regEx("\\\\");
    QStringList processNameList = processName.split(regEx);
    QString appName = processNameList.at(processNameList.size() - 1);
    float elapsedTime = timer.elapsed();

    auto it = std::find_if(
                mostUsedVec.begin(), mostUsedVec.end(),
                [appName](const std::pair<QString, float> &element){ return element.first == appName; }
                );
    if(it != mostUsedVec.end()) {
        (*it).second += elapsedTime;
    }
    else {
        mostUsedVec.push_back(std::make_pair(appName, elapsedTime));
    }

    qDebug() << appName << "Used time: " << elapsedTime / 1000;
    qDebug() << mostUsedVec;

    timer.start(); //output first, then restart the timer
}
