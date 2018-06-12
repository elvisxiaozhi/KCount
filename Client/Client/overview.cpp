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
            [=](int index){
        mostPressed->reloadData(index);
        totalPressed->reloadData(index); /*note most first, then total*/
        mouseClick->reloadData(index);
        mostUsed->reloadData(index);
    });
}

void Overview::updateDatabase()
{
    mostPressed->updateDatabase();
    mouseClick->updateDatabase();
    mostUsed->updateDatabase();
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

//   //paint notification icon
//    QIcon icon(":/Resources/Icons/notification.png");
//    QRect iconRect(800, 55, 30, 30);
//    icon.paint(&painter, iconRect);
}

void Overview::timeout()
{
    timer->start(1000 * 60 * 60); //1 sec * 60 (= 1 minute) * 60 (= 1 hour) and it starts in every hour
    updateDatabase();
    Database::timeout();
}

void Overview::loadData()
{
    mostUsed->setData();
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
    mostUsed = new MostUsed(this);
    mostUsed->setFixedSize(300, 300);

    totalPressed = new TotalPressed(this);
    totalPressed->setFixedSize(300, 300);

    mostPressed = new MostPressed(this);
    mostPressed->setFixedSize(300, 300);
    mostPressed->hide();

    mouseClick = new MouseClick(this);
    mouseClick->setFixedSize(300, 300);

    lblGLayout->addWidget(mostUsed, 0, 0);
    lblGLayout->addWidget(totalPressed, 0, 1);
    lblGLayout->addWidget(mostPressed, 0, 1);
    lblGLayout->addWidget(mouseClick, 0, 2);

    connect(totalPressed, &TotalPressed::switchBtnClicked, [this](){ mostPressed->show(); });
    connect(mostPressed, &MostPressed::switchBtnClicked, [this](){ totalPressed->show(); });
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
