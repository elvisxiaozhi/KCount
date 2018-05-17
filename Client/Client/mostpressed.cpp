#include "mostpressed.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include "database.h"
#include "signalemitter.h"

MostPressed::MostPressed(QWidget *parent) : QWidget(parent)
{
    mostPressed = Database::returnKeyVec(1);

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
        contents[i] = new Label(mostPressed[i].second, 20);
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
                );
}

void MostPressed::setContents()
{
    if(mostPressed.isEmpty()) {
        for(int i = 0; i < 5; ++i) {
            contents[i]->setText("");
            contents[i]->setLabelColor(mostPressed[i].second);
        }
    }
    if(mostPressed.size() > 5) {
        for(int i = 0; i < 5; ++i) {
            contents[i]->setText(mostPressed[i].first + ": " + QString::number(mostPressed[i].second));
            contents[i]->setLabelColor(mostPressed[i].second);
        }
    }
    else {
        for(int i = 0; i < mostPressed.size(); ++i) {
            contents[i]->setText(mostPressed[i].first + ": " + QString::number(mostPressed[i].second));
            contents[i]->setLabelColor(mostPressed[i].second);
        }
        for(int i = 0; i < 5 - mostPressed.size(); ++i) {
            contents[4 - i]->setText(""); //note the 4 - i
            contents[4 - i]->setLabelColor(0);
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

void MostPressed::updateDatabase()
{
    Database::updatePressedKeyToDB(tempKeyMap);
    tempKeyMap.clear(); //after updating datebase, clear the map, so it can store new data for the next hour
}

void MostPressed::reloadData(int index)
{
    updateDatabase();
    mostPressed.clear();
    mostPressed = Database::returnKeyVec(index);
    setContents();
}

void MostPressed::keyPressed(QString pressedKey)
{
    if(tempKeyMap.contains(pressedKey)) { //if the map has already stored the pressed key
        unsigned long int newValue = tempKeyMap.value(pressedKey) + 1; //then the map key value + 1 pressed time
        tempKeyMap.insert(pressedKey, newValue); //and insert new data to the map
    }
    else {
        tempKeyMap.insert(pressedKey, 1); //then the insert the pressed key with a 1 time pressed value to the map
    }

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
