#include "overview.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QTime>
#include <QKeyEvent>
#include <database.h>

Overview::Overview(QWidget *parent) : QWidget(parent)
{
    setWindowLayout();
    setWindowStyleSheet();
    setTimeSpanBox();
    setLbls();

    setTimer(); //set the time, so the database will save automatically in each hour

    connect(this, &Overview::loadingData, this, &Overview::loadData);
    connect(timeSpanBox, QOverload<int>::of(&QComboBox::activated),
            [=](int index){ comboBoxChanged(index); });
}

void Overview::updateDatabase()
{
    //because each of them has five objects, and each object has a temp vector that can used to save to database
    //only need one of them to be saved to database, here I chose the index 1(daily)
    mostPressedArr[1]->updateDatabase();
    mouseClickArr[1]->updateDatabase();
    mostUsedArr[1]->updateDatabase();
}

void Overview::setWindowLayout()
{
    mainVLayout = new QVBoxLayout(this);
    timeSpanHLayout = new QHBoxLayout; //do not set parent

    scrollArea = new QScrollArea(this);
    scrollArea->setBackgroundRole(QPalette::Window);
    scrollArea->setFrameShadow(QFrame::Plain);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFixedSize(1000, 450);

    scrollWidget = new QWidget(this);

    lblGLayout = new QGridLayout;

    scrollWidget->setLayout(lblGLayout);

    mainVLayout->addLayout(timeSpanHLayout);
    mainVLayout->addWidget(scrollArea);
    this->setLayout(mainVLayout);

    scrollArea->setWidget(scrollWidget); //needs to be in the last
}

void Overview::paintEvent(QPaintEvent *event)
{
    //to make the custom widget able to set style sheet
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

    //paint the title
    painter.setPen(QColor(255,115,115));
    painter.setFont(QFont("Futura", 20));
    painter.drawText(QRect(50, 50, event->rect().width(), event->rect().height()), "Overview");
}

void Overview::timeout()
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    updateDatabase(); //note must update first
    Database::timeout(); //when database timeout, the current hour will change

    //delete old widgets
    if(QTime::currentTime().toString("h").toInt() == 0) {
        for(int i = 0; i < 5; ++i) {
            delete mostUsedArr[i];
            delete totalPressedArr[i];
            delete mostPressedArr[i];
            delete mouseClickArr[i];
        }
        setLbls(); //add new widgets for the new day
    }
}

void Overview::loadData()
{
    //loading app usage data when clicked overview page
    for(int i = 0; i < 5; ++i) {
        if(mostUsedArr[i]->isVisible()) {
            mostUsedArr[i]->setData();
        }
    }
}

void Overview::comboBoxChanged(int index)
{
    for(int i = 0; i < 5; ++i) {
        if(i == index) {
            mostUsedArr[i]->show();
            mostUsedArr[i]->setData();

            totalPressedArr[i]->show();
            mouseClickArr[i]->show();
        }
        else {
            mostUsedArr[i]->hide();
            totalPressedArr[i]->hide();
            mostPressedArr[i]->hide();
            mouseClickArr[i]->hide();
        }
    }
}

void Overview::setWindowStyleSheet()
{
    setStyleSheet(
                "QWidget { background-color: #eeeeee; }"
                "QComboBox { border: 1px solid gray; border-radius: 8px; padding: 8px 8px 8px 8px; min-width: 3em; background: #faebd7; }"
                "QComboBox:editable { background: #ffe4e1; }" //set arrow background color
                "QComboBox:on { background: #fa8072; }" //change the combo box bgcolor when opened
                "QComboBox::drop-down {"
                "border-left-width: 1px; border-left-color: darkgray; border-left-style: solid;/* just a single line */"
                "border-top-right-radius: 3px; border-bottom-right-radius: 3px; /* same radius as the QComboBox */ "
                "}" //change the shape of drop down menu
                "QComboBox QAbstractItemView { border: 2px solid darkgray; selection-background-color: #FFC0CB; }" //change the selection bgcolor
                "QComboBox::down-arrow { image: url(:/Resources/Icons/down-arrow.png); }"
                "QComboBox::down-arrow:on { image: url(:/Resources/Icons/up-arrow.png); }"
                "QLabel#SpanTextLbl { background: #D3FFCE; padding: 8px 8px 8px 8px; border: 1px solid gray; border-radius: 8px; }"
                "QScrollBar:vertical { background: #faebd7; width: 4px; }"
                "QScrollBar::handle:vertical { background: #ff7373; }"
                );
}

void Overview::setTimeSpanBox()
{
    QLabel *spanTextLbl = new QLabel(this);
    spanTextLbl->setText("Time Span: ");
    spanTextLbl->setObjectName("SpanTextLbl");

    timeSpanBox = new QComboBox(this);
    timeSpanBox->installEventFilter(this);
    timeSpanBox->addItems({tr("Hour"), tr("Day"), tr("Week"), tr("Month"), tr("Year")});
    timeSpanBox->setCurrentText("Day");

    timeSpanHLayout->addStretch();
    timeSpanHLayout->addWidget(spanTextLbl);
    timeSpanHLayout->addWidget(timeSpanBox);
}

void Overview::setLbls()
{
    for(int i = 0; i < 5; ++i) {
        mostUsedArr[i] = new MostUsed(this, i);
        mostUsedArr[i]->setFixedSize(300, 300);
        if(i != 1) {
            mostUsedArr[i]->hide();
        }

        totalPressedArr[i] = new TotalPressed(this, i);
        totalPressedArr[i]->setFixedSize(300, 300);
        if(i != 1) {
            totalPressedArr[i]->hide();
        }

        mostPressedArr[i] = new MostPressed(this, i);
        mostPressedArr[i]->setFixedSize(300, 300);
        mostPressedArr[i]->hide();

        mouseClickArr[i] = new MouseClick(this, i);
        mouseClickArr[i]->setFixedSize(300, 300);
        if(i != 1) {
            mouseClickArr[i]->hide();
        }

        lblGLayout->addWidget(mostUsedArr[i], 0, 0);
        lblGLayout->addWidget(totalPressedArr[i], 0, 1);
        lblGLayout->addWidget(mostPressedArr[i], 0, 1);
        lblGLayout->addWidget(mouseClickArr[i], 0, 2);

        connect(totalPressedArr[i], &TotalPressed::switchBtnClicked, [this, i](){ mostPressedArr[i]->show(); });
        connect(mostPressedArr[i], &MostPressed::switchBtnClicked, [this, i](){ totalPressedArr[i]->show(); });
    }
}

void Overview::setTimer()
{
    QStringList currentTimeStringList = QTime::currentTime().toString("hh:mm:ss").split(":");
    timer = new QTimer(this);
    timer->start(1000 * 60 * 60 - 1000 * 60 * QString(currentTimeStringList[1]).toInt() - 1000 * QString(currentTimeStringList[2]).toInt()); //1 sec * 60 (= 1 minute) * 60 (= 1 hour)
    connect(timer, &QTimer::timeout, this, &Overview::timeout);
}

bool Overview::eventFilter(QObject *watched, QEvent *event)
{
    if(timeSpanBox){
        if(timeSpanBox == watched && event->type() == QEvent::KeyPress){
           QKeyEvent *key = static_cast<QKeyEvent *>(event);
           if(!key->text().isEmpty()) {
               return true;
           }
        }
    }
    return QObject::eventFilter(watched, event);
}
