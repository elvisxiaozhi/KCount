#include "mostpressed.h"
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>
#include "database.h"
#include "signalemitter.h"
#include <QTime>
#include "barchart.h"

MostPressed::MostPressed(QWidget *parent, int mode, QString title) : QWidget(parent)
{
    mostPressed = Database::returnKeyVec(mode);
    currentHour = QTime::currentTime().toString("h").toInt();

    setMainLayout(title);
    setWindowStyleSheet();

    connect(Emitter::Instance(), &SignalEmitter::keyPressed, this, &MostPressed::keyPressed);
    connect(switchBtn, &CustomButton::clicked, [this](){ this->hide(); emit switchBtnClicked(); });
    connect(showMoreBtn, &CustomButton::clicked, [this](){ showMoreBtn->hide(); showLessBtn->show(); scrollArea->show(); contWidget->hide(); });
    connect(showLessBtn, &CustomButton::clicked, [this](){ showMoreBtn->show(); showLessBtn->hide(); scrollArea->hide(); contWidget->show(); });
}

void MostPressed::setMainLayout(QString lblTitle)
{
    mainVLayout = new QVBoxLayout(this);
    setLayout(mainVLayout);

    contWidget = new QWidget(this);
    contWidget->setFixedHeight(200); //need to set fix height

    contVLayout = new QVBoxLayout;
    contVLayout->setContentsMargins(0, 0, 0, 0);
    contVLayout->setSpacing(0);

    contWidget->setLayout(contVLayout);

    title = new QLabel(this);
    title->setText(lblTitle);
    title->setObjectName("Title");
    title->setAlignment(Qt::AlignCenter);

    switchBtn = new CustomButton(this);
    switchBtn->setIcon(QIcon(":/icons/switch.png"));

    showMoreBtn = new CustomButton(this);
    showMoreBtn->setIcon(QIcon(":/icons/show_more.png"));

    showLessBtn = new CustomButton(this);
    showLessBtn->setIcon(QIcon(":/icons/show_less.png"));
    showLessBtn->hide();

    lblHLayout = new QHBoxLayout();

    lblHLayout->addWidget(switchBtn);
    lblHLayout->addStretch();
    lblHLayout->addWidget(title);
    lblHLayout->addStretch();
    lblHLayout->addWidget(showMoreBtn);
    lblHLayout->addWidget(showLessBtn);

    contents.resize(5);
    for(int i = 0; i < contents.size(); ++i) {
        contents[i] = new Label(0, 20);
        contents[i]->setFixedHeight(40);
        contVLayout->addWidget(contents[i]);
    }
    setContents();

    createScrollWidget();

    mainVLayout->addLayout(lblHLayout);
    mainVLayout->addWidget(contWidget);
    mainVLayout->addWidget(scrollArea);
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
            contents[i]->setLabelColor(0);
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

void MostPressed::createScrollWidget()
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

void MostPressed::createScrollConts()
{
    for(int i = 0; i < mostPressed.size() - scrollConts.size(); ++i) {
        Label *contLbl = new Label(0, 20);
        scrollContVLayout->addWidget(contLbl);
        scrollConts.push_back(contLbl);
    }

    for(int i = 0; i < scrollConts.size(); ++i) {
        scrollConts[i]->setText(mostPressed[i].first + ": " + QString::number(mostPressed[i].second));
        scrollConts[i]->setLabelColor(mostPressed[i].second);
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
    currentHour = QTime::currentTime().toString("h").toInt();
}

void MostPressed::keyPressed(QString pressedKey)
{
    ++BarChart::dailyMap[currentHour];
    ++BarChart::weeklyMap[BarChart::weeklyMap.size() - 1];
    ++BarChart::monthlyMap[BarChart::monthlyMap.size() - 1];
    ++BarChart::yearlyMap[BarChart::yearlyMap.size() - 1];

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

    //update each time when key is pressed, or the number will suddently change too much
    setContents();
    createScrollConts();
}
