#include "mostpressed.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include "database.h"
#include "signalemitter.h"

MostPressed::MostPressed(QWidget *parent) : QWidget(parent)
{
    setWindowStyleSheet();

    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);
    contVLayout = new QVBoxLayout;
    contVLayout->setSpacing(0);

    title = new QLabel(this);
    title->setText("Most Pressed");
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    contents.resize(5);
    for(int i = 0; i < contents.size(); ++i) {
        contents[i] = new QLabel(this);
        contents[i]->setAlignment(Qt::AlignCenter);
        contents[i]->setObjectName("Contents");
        contents[i]->setFixedHeight(40);
        contVLayout->addWidget(contents[i]);
    }
    setContents();

    mainVLayout->addWidget(title);
    mainVLayout->addLayout(contVLayout);

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &MostPressed::keyPressed);
}

void MostPressed::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background: #FFE4E1; }"
                "QLabel#Title { font: 20px; color: #666666; }"
                "QLabel#Contents { font: 20px; color: #FDFEFE; background: #FAD7A0; border: 1px solid #F1C40F; }"
                );
}

void MostPressed::setContents()
{
    qDebug() << mostPressed;
    if(mostPressed.isEmpty()) {
        for(int i = 0; i < 5; ++i) {
            contents[i]->setText("");
        }
    }
    if(mostPressed.size() > 5) {
        for(int i = 0; i < 5; ++i) {
            contents[i]->setText(mostPressed[i].first + ": " + QString::number(mostPressed[i].second));
        }
    }
    else {
        for(int i = 0; i < mostPressed.size(); ++i) {
            contents[i]->setText(mostPressed[i].first + ": " + QString::number(mostPressed[i].second));
        }
        for(int i = 0; i < 5 - mostPressed.size(); ++i) {
            contents[4 - i]->setText(""); //note the 4 - i
        }
    }
}

void MostPressed::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void MostPressed::keyPressed(QString pressedKey)
{
    auto it = std::find_if(mostPressed.begin(), mostPressed.end(),
        [pressedKey](const std::pair<QString, unsigned long int> &element){ return element.first == pressedKey; });
    if(it != mostPressed.end()) {
        ++(*it).second;
    }
    else {
        mostPressed.push_back(std::make_pair(pressedKey, 1));
    }

    std::sort(mostPressed.begin(), mostPressed.end(),
              [](const std::pair<QString, unsigned long int> &a, const std::pair<QString, unsigned long int> &b){ return a.second > b.second; });

    setContents();
}
